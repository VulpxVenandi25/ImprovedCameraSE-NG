cmake_minimum_required(VERSION 3.22)

set(NAME "ImprovedCameraSE")
set(VERSION 1.1.2)

macro(set_from_environment VARIABLE)
	if (NOT DEFINED ${VARIABLE} AND DEFINED ENV{${VARIABLE}})
		set(${VARIABLE} $ENV{${VARIABLE}})
	endif ()
endmacro()

set_from_environment(VCPKG_ROOT)

set(IC_ROOT "${CMAKE_CURRENT_SOURCE_DIR}/ImprovedCamera")
set(CommonLibPath "${IC_ROOT}/vendor/CommonLibSSE-NG")
set(CommonLibName "CommonLibSSE")

if (DEFINED VCPKG_ROOT)
	set(CMAKE_TOOLCHAIN_FILE "${VCPKG_ROOT}/scripts/buildsystems/vcpkg.cmake")
	set(VCPKG_TARGET_TRIPLET "x64-windows-static" CACHE STRING "")
else ()
	message(WARNING "VCPKG_ROOT is not set. Continuing without vcpkg.")
endif ()

project(${NAME} VERSION ${VERSION} LANGUAGES C CXX)

if (PROJECT_SOURCE_DIR STREQUAL PROJECT_BINARY_DIR)
	message(FATAL_ERROR "In-source builds are not allowed.")
endif ()

# ---- CommonLibSSE-NG options (consumed by add_subdirectory below) ----
set(BUILD_TESTS OFF CACHE BOOL "")
set(SKSE_SUPPORT_XBYAK ON CACHE BOOL "")
set(ENABLE_SKYRIM_SE ON CACHE BOOL "")
set(ENABLE_SKYRIM_AE ON CACHE BOOL "")
set(ENABLE_SKYRIM_VR OFF CACHE BOOL "")
set(COMMONLIB_ENABLE_IPO OFF CACHE BOOL "")

set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>" CACHE STRING "")

# ---- CommonLibSSE-NG ----
if (IS_DIRECTORY ${CommonLibPath})
	add_subdirectory(${CommonLibPath} ${CommonLibName} EXCLUDE_FROM_ALL)
else ()
	message(FATAL_ERROR "CommonLibSSE-NG not found at ${CommonLibPath}.")
endif ()

# ---- Vendor: Dear ImGui (core; the DX11/Win32 backends are compiled in vendor.cpp) ----
add_library(imgui STATIC
	${IC_ROOT}/vendor/imgui/imgui.cpp
	${IC_ROOT}/vendor/imgui/imgui_demo.cpp
	${IC_ROOT}/vendor/imgui/imgui_draw.cpp
	${IC_ROOT}/vendor/imgui/imgui_tables.cpp
	${IC_ROOT}/vendor/imgui/imgui_widgets.cpp
)
target_include_directories(imgui PUBLIC ${IC_ROOT}/vendor/imgui)
target_compile_features(imgui PRIVATE cxx_std_20)

# ---- Vendor: MinHook ----
add_library(minhook STATIC
	${IC_ROOT}/vendor/minhook/src/buffer.c
	${IC_ROOT}/vendor/minhook/src/hook.c
	${IC_ROOT}/vendor/minhook/src/trampoline.c
	${IC_ROOT}/vendor/minhook/src/hde/hde32.c
	${IC_ROOT}/vendor/minhook/src/hde/hde64.c
)
target_include_directories(minhook
	PUBLIC ${IC_ROOT}/vendor/minhook/include
	PRIVATE ${IC_ROOT}/vendor/minhook/src)
target_compile_definitions(minhook PRIVATE WIN32 _LIB STRICT)

# ---- Vendor: xbyak (header-only, installed by vcpkg; required by the PCH) ----
find_package(xbyak CONFIG REQUIRED)

# ---- ImprovedCameraSE ----
file(GLOB_RECURSE IC_SOURCES CONFIGURE_DEPENDS "${IC_ROOT}/source/*.cpp")

add_library(${NAME} SHARED
	${IC_SOURCES}
	${IC_ROOT}/PCH/PCH.cpp
	${IC_ROOT}/resource/plugin.rc
)

target_compile_definitions(${NAME} PRIVATE
	WIN32_LEAN_AND_MEAN
	NOMINMAX
	ENABLE_SKYRIM_SE=1
	ENABLE_SKYRIM_AE=1
	SKSE_SUPPORT_XBYAK=1
	# xbyak pulls in clang's <cpuid.h> (5-arg __cpuid macro), which collides with
	# stb_image's MSVC-style __cpuid(info, 1). stb only decodes the logo PNG here,
	# so dropping its SSE2 path is harmless.
	STBI_NO_SIMD
)

target_include_directories(${NAME} PRIVATE
	${IC_ROOT}/include
	${IC_ROOT}/PCH
	${IC_ROOT}/vendor
	${IC_ROOT}/vendor/glm
	${IC_ROOT}/vendor/mINI/src
	${IC_ROOT}/vendor/minhook/include
)

target_link_libraries(${NAME}
	PUBLIC CommonLibSSE::CommonLibSSE
	PRIVATE imgui minhook xbyak::xbyak
)

# Keep a linker map beside the binary: the crash report records faults as
# <module>+<rva>, and this map turns those RVAs back into symbol names.
target_link_options(${NAME} PRIVATE "/MAP:${CMAKE_BINARY_DIR}/bin/ImprovedCameraSE.map")

target_precompile_headers(${NAME} PRIVATE "${IC_ROOT}/PCH/PCH.h")

set_target_properties(${NAME} PROPERTIES
	OUTPUT_NAME "ImprovedCameraSE"
	RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin"
)

# Mirror the .vcxproj PostBuild: drop the runtime assets next to the DLL.
add_custom_command(TARGET ${NAME} POST_BUILD
	COMMAND ${CMAKE_COMMAND} -E copy_directory
		"${CMAKE_CURRENT_SOURCE_DIR}/assets/ImprovedCameraSE"
		"$<TARGET_FILE_DIR:${NAME}>/ImprovedCameraSE"
	COMMENT "Copying runtime assets next to ImprovedCameraSE.dll")
