# Wrapper around the vendored CommonLibSSE-NG clang-cl toolchain that pins the
# static CRT for the plugin *and* for every vcpkg dependency. Without this the
# final link fails with /failifmismatch (MT_StaticRelease vs MD_DynamicRelease),
# and a dynamic-CRT plugin crashes inside SKSE::Init on Proton/Wine prefixes that
# ship an ancient MSVCP140.dll.
set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>" CACHE STRING "" FORCE)

include("${CMAKE_CURRENT_LIST_DIR}/../ImprovedCamera/vendor/CommonLibSSE-NG/cmake/toolchain-linux-clangcl.cmake")
