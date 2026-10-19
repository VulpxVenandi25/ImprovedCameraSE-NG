# Builds dependencies with clang-cl targeting the MSVC ABI and the static CRT.
# See cmake/toolchain-linux-clangcl-static.cmake for why the static CRT matters.
set(VCPKG_TARGET_ARCHITECTURE x64)
set(VCPKG_CRT_LINKAGE static)
set(VCPKG_LIBRARY_LINKAGE static)

# Do not set VCPKG_CMAKE_SYSTEM_NAME to "Windows": vcpkg only sets
# VCPKG_TARGET_IS_WINDOWS when it is undefined or empty, so naming it
# silently unsets that. The chainloaded toolchain does the real targeting.
set(VCPKG_CHAINLOAD_TOOLCHAIN_FILE "${CMAKE_CURRENT_LIST_DIR}/../toolchain-linux-clangcl-static.cmake")
