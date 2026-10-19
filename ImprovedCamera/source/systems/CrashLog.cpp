/*
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/.
 */

/*
 * Diagnostic crash reporter.
 *
 * The load order's CrashLogger does not load on 1.7.104, so a plugin crash leaves no trace
 * at all -- the game just exits to the desktop. This vectored exception handler fills that
 * gap for this plugin: on a fatal exception it writes the faulting address, the registers
 * and every plausible return address found on the stack, each labelled <module>+<rva>, into
 * ImprovedCameraSE.crash.log next to the plugin's own log.
 *
 * It deliberately avoids the CRT, the heap and spdlog. An access violation can happen while
 * any of those hold a lock, so only kernel32 calls (CreateFileA/WriteFile) and hand written
 * formatting are used, and the whole report is composed in a static buffer first.
 */

// Precompiled Header
#include "PCH.h"

#include "systems/CrashLog.h"

#include "plugin.h"

#include <cstdint>

#include <KnownFolders.h>
#include <ShlObj.h>
#include <winternl.h>

namespace {

	constexpr std::size_t kMaxModules = 256;
	constexpr std::size_t kBufferSize = 32768;
	constexpr std::size_t kMaxStackEntries = 1024;
	constexpr std::size_t kMaxPrintedFrames = 64;

	// LDR_DATA_TABLE_ENTRY, only the fields needed here.
	struct LdrEntry {
		LIST_ENTRY     inLoadOrderLinks;
		LIST_ENTRY     inMemoryOrderLinks;
		LIST_ENTRY     inInitializationOrderLinks;
		void*          dllBase;
		void*          entryPoint;
		std::uint32_t  sizeOfImage;
		std::uint32_t  pad0;
		UNICODE_STRING fullDllName;
		UNICODE_STRING baseDllName;
	};

	struct ModuleInfo {
		std::uintptr_t base{ 0 };
		std::uintptr_t end{ 0 };
		char           name[48]{};
	};

	ModuleInfo      g_modules[kMaxModules]{};
	std::size_t     g_moduleCount = 0;
	char            g_logPath[MAX_PATH]{};
	char            g_buffer[kBufferSize]{};
	std::size_t     g_bufferLen = 0;

	void AppendChar(char a_char)
	{
		if (g_bufferLen < kBufferSize - 1)
			g_buffer[g_bufferLen++] = a_char;
	}

	void AppendStr(const char* a_text)
	{
		if (!a_text)
			return;
		for (const char* p = a_text; *p; ++p)
			AppendChar(*p);
	}

	void AppendHex(std::uint64_t a_value, std::size_t a_minDigits = 0)
	{
		char         digits[16]{};
		std::size_t  count = 0;
		do {
			const auto nibble = static_cast<unsigned>(a_value & 0xF);
			digits[count++] = static_cast<char>(nibble < 10 ? '0' + nibble : 'A' + nibble - 10);
			a_value >>= 4;
		} while (a_value && count < 16);
		while (count < a_minDigits)
			digits[count++] = '0';
		while (count)
			AppendChar(digits[--count]);
	}

	void AppendDec(std::uint64_t a_value)
	{
		char         digits[20]{};
		std::size_t  count = 0;
		do {
			digits[count++] = static_cast<char>('0' + (a_value % 10));
			a_value /= 10;
		} while (a_value && count < 20);
		while (count)
			AppendChar(digits[--count]);
	}

	void NewLine()
	{
		AppendChar('\r');
		AppendChar('\n');
	}

	// Local wall clock, so several runs can be told apart in the same file.
	void AppendTime()
	{
		SYSTEMTIME st{};
		GetLocalTime(&st);

		AppendDec(st.wDay);
		AppendChar('/');
		AppendDec(st.wMonth);
		AppendChar('/');
		AppendDec(st.wYear);
		AppendChar(' ');
		AppendDec(st.wHour);
		AppendChar(':');
		AppendDec(st.wMinute);
		AppendChar(':');
		AppendDec(st.wSecond);
		AppendChar('.');
		AppendChar(static_cast<char>('0' + st.wMilliseconds / 100));
		AppendChar(static_cast<char>('0' + (st.wMilliseconds / 10) % 10));
		AppendChar(static_cast<char>('0' + st.wMilliseconds % 10));
	}

	void Flush()
	{
		if (!g_bufferLen || !g_logPath[0])
			return;

		const auto handle = CreateFileA(g_logPath, FILE_APPEND_DATA, FILE_SHARE_READ | FILE_SHARE_WRITE,
			nullptr, OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL, nullptr);
		if (handle != INVALID_HANDLE_VALUE) {
			DWORD written = 0;
			WriteFile(handle, g_buffer, static_cast<DWORD>(g_bufferLen), &written, nullptr);
			CloseHandle(handle);
		}
		g_bufferLen = 0;
	}

	void DescribeAddress(std::uintptr_t a_address)
	{
		for (std::size_t i = 0; i < g_moduleCount; ++i) {
			if (a_address >= g_modules[i].base && a_address < g_modules[i].end) {
				AppendStr(g_modules[i].name);
				AppendChar('+');
				AppendStr("0x");
				AppendHex(a_address - g_modules[i].base);
				return;
			}
		}
		AppendStr("0x");
		AppendHex(a_address, 16);
	}

	bool InsideModule(std::uintptr_t a_address)
	{
		for (std::size_t i = 0; i < g_moduleCount; ++i) {
			if (a_address >= g_modules[i].base && a_address < g_modules[i].end)
				return true;
		}
		return false;
	}

	// ASCII rendering of a UTF-16 module name, base name only.
	void NarrowName(char* a_out, std::size_t a_outSize, const wchar_t* a_name, std::size_t a_length)
	{
		if (!a_outSize)
			return;
		a_out[0] = '\0';
		if (!a_name)
			return;

		const std::size_t chars = a_length / sizeof(wchar_t);
		std::size_t       written = 0;
		for (std::size_t i = 0; i < chars && written + 1 < a_outSize; ++i) {
			const auto wc = a_name[i];
			a_out[written++] = (wc >= 0x20 && wc < 0x7F) ? static_cast<char>(wc) : '?';
		}
		a_out[written] = '\0';
	}

	void AddModuleRange(std::uintptr_t a_base, std::size_t a_size, const wchar_t* a_name, std::size_t a_nameLength)
	{
		if (!a_base || !a_size || g_moduleCount >= kMaxModules)
			return;

		auto& info = g_modules[g_moduleCount++];
		info.base = a_base;
		info.end = a_base + a_size;
		NarrowName(info.name, sizeof(info.name), a_name, a_nameLength);
	}

	// Snapshot of the loaded modules straight out of the PEB, taken while the exception is
	// handled so that plugins loaded after SKSEPlugin_Load are present as well.
	void SnapshotModules()
	{
		g_moduleCount = 0;

		const auto* peb = NtCurrentTeb()->ProcessEnvironmentBlock;
		if (!peb || !peb->Ldr)
			return;

		const auto* head = &peb->Ldr->InMemoryOrderModuleList;
		const auto* node = head->Flink;
		for (std::size_t i = 0; node && node != head && g_moduleCount < kMaxModules; ++i) {
			const auto* entry = reinterpret_cast<const LdrEntry*>(
				reinterpret_cast<const std::uint8_t*>(node) - offsetof(LdrEntry, inMemoryOrderLinks));

			AddModuleRange(reinterpret_cast<std::uintptr_t>(entry->dllBase), entry->sizeOfImage,
				entry->baseDllName.Buffer, entry->baseDllName.Length);

			node = node->Flink;
		}
	}

	void PrintRegister(const char* a_name, std::uint64_t a_value)
	{
		AppendStr(a_name);
		AppendStr("=0x");
		AppendHex(a_value, 16);
		AppendStr("  ");
	}

	LONG CALLBACK CrashHandler(EXCEPTION_POINTERS* a_info)
	{
		const auto* record = a_info->ExceptionRecord;
		const auto* context = a_info->ContextRecord;
		const auto  code = record->ExceptionCode;

		// Noise that would otherwise drown the report.
		switch (code) {
		case 0xE06D7363:  // C++ exception
		case 0x40010006:  // OutputDebugStringA
		case 0x4001000A:  // OutputDebugStringW
		case 0x406D1388:  // SetThreadName
			return EXCEPTION_CONTINUE_SEARCH;
		default:
			break;
		}

		g_bufferLen = 0;
		SnapshotModules();

		AppendStr("=== ");
		AppendTime();
		AppendStr(" ");
		AppendStr("exception 0x");
		AppendHex(code, 8);
		AppendStr(" at ");
		DescribeAddress(reinterpret_cast<std::uintptr_t>(record->ExceptionAddress));
		AppendStr("  (thread ");
		AppendDec(GetCurrentThreadId());
		AppendChar(')');
		NewLine();

		if (code == 0xC0000005 && record->NumberParameters >= 2) {
			const auto kind = record->ExceptionInformation[0];
			AppendStr("    access violation: ");
			AppendStr(kind == 1 ? "write" : (kind == 8 ? "execute" : "read"));
			AppendStr(" of 0x");
			AppendHex(record->ExceptionInformation[1], 16);
			NewLine();
		}

		AppendStr("    RIP="); DescribeAddress(context->Rip);
		AppendStr("  RSP=0x"); AppendHex(context->Rsp, 16);
		NewLine();
		PrintRegister("RAX", context->Rax);
		PrintRegister("RBX", context->Rbx);
		PrintRegister("RCX", context->Rcx);
		PrintRegister("RDX", context->Rdx);
		NewLine();
		PrintRegister("RSI", context->Rsi);
		PrintRegister("RDI", context->Rdi);
		PrintRegister("RBP", context->Rbp);
		NewLine();
		PrintRegister("R8 ", context->R8);
		PrintRegister("R9 ", context->R9);
		PrintRegister("R10", context->R10);
		PrintRegister("R11", context->R11);
		NewLine();
		PrintRegister("R12", context->R12);
		PrintRegister("R13", context->R13);
		PrintRegister("R14", context->R14);
		PrintRegister("R15", context->R15);
		NewLine();

		AppendStr("    stack (return address candidates):");
		NewLine();

		const auto* tib = reinterpret_cast<const NT_TIB*>(__readgsqword(0x30));
		const auto  stackBase = reinterpret_cast<std::uintptr_t>(tib->StackBase);
		const auto  stackLimit = reinterpret_cast<std::uintptr_t>(tib->StackLimit);
		auto        sp = context->Rsp;
		std::size_t printed = 0;

		for (std::size_t i = 0; i < kMaxStackEntries && sp >= stackLimit && sp + sizeof(std::uintptr_t) <= stackBase && printed < kMaxPrintedFrames; ++i, sp += sizeof(std::uintptr_t)) {
			const auto value = *reinterpret_cast<const std::uintptr_t*>(sp);
			if (!InsideModule(value))
				continue;
			AppendStr("      rsp+0x");
			AppendHex(sp - context->Rsp);
			AppendStr(" -> ");
			DescribeAddress(value);
			NewLine();
			++printed;
		}

		AppendStr("=== end of report");
		NewLine();
		NewLine();
		Flush();

		return EXCEPTION_CONTINUE_SEARCH;
	}

}

namespace Systems {

	void CrashLog::Install()
	{
		static bool installed = false;
		if (installed)
			return;
		installed = true;

		auto plugin = DLLMain::Plugin::Get();

		PWSTR documents{ nullptr };
		if (SUCCEEDED(SHGetKnownFolderPath(FOLDERID_Documents, KF_FLAG_DEFAULT, nullptr, &documents)) && documents) {
			char path[MAX_PATH]{};
			snprintf(path, sizeof(path), "%ls\\My Games\\%s\\SKSE\\%s.crash.log", documents,
				plugin->SkyrimSE()->WindowName().c_str(), plugin->Name().c_str());
			CoTaskMemFree(documents);

			strncpy(g_logPath, path, sizeof(g_logPath) - 1);
		}

		AddVectoredExceptionHandler(1, &CrashHandler);

		g_bufferLen = 0;
		AppendStr("=== ");
		AppendTime();
		AppendStr(" crash handler installed (");
		AppendStr(plugin->Name().c_str());
		AppendStr("), watching for exceptions ===");
		NewLine();
		Flush();
	}

}
