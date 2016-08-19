# - Try to find LibNaCl include dirs and libraries
#
# Usage of this module as follows:
#
#     find_package(LibNaCl)
#
# Variables used by this module, they can change the default behaviour and need
# to be set before calling find_package:
#
#  LibNaCl_ROOT_DIR          Set this variable to the root installation of
#                            LibNaCl if the module has problems finding the
#                            proper installation path.
#
# Variables defined by this module:
#
#  LIBNACL_FOUND             System has LibNaCl, include and library dirs found
#  LIBNACL_INCLUDE_DIR       The LibNaCl include directories.
#  LIBNACL_LIBRARY           The LibNaCl libraries.
#  LIBNACL_LIBRARIES         The LibNaCl libraries.
#  LIBNACL_RANDOM            The LibNaCl randombytes.o object file.


find_path(LibNaCl_ROOT_DIR
        NAMES include/crypto_box.h include/crypto_secretbox.h lib/libnacl.a
        HINTS ${LibNaCl_ROOT_DIR}
        )

find_path(LibNaCl_INCLUDE_DIR
        NAMES crypto_box.h crypto_secretbox.h
        HINTS ${LibNaCl_ROOT_DIR}/include ${LibNaCl_ROOT_DIR}/include/nacl ${LibNaCl_ROOT_DIR}/include/amd64 ${LibNaCl_ROOT_DIR}/include/x86
        PATHS ${LibNaCl_ROOT_DIR}/include ${LibNaCl_ROOT_DIR}/include/nacl ${LibNaCl_ROOT_DIR}/include/amd64 ${LibNaCl_ROOT_DIR}/include/x86
        )

find_library(LibNaCl_LIBRARY
        NAMES libnacl.a
        HINTS ${LibNaCl_ROOT_DIR}/lib ${LibNaCl_ROOT_DIR}/lib/amd64 ${LibNaCl_ROOT_DIR}/lib/x86
        PATHS ${LibNaCl_ROOT_DIR}/lib ${LibNaCl_ROOT_DIR}/lib/amd64 ${LibNaCl_ROOT_DIR}/lib/x86
        )

find_file(LibNaCl_RANDOM
        NAMES randombytes.o
        HINTS ${LibNaCl_ROOT_DIR}/lib ${LibNaCl_ROOT_DIR}/lib/amd64 ${LibNaCl_ROOT_DIR}/lib/x86
        PATHS ${LibNaCl_ROOT_DIR}/lib ${LibNaCl_ROOT_DIR}/lib/amd64 ${LibNaCl_ROOT_DIR}/lib/x86
        )

set(LibNaCl_LIBRARIES ${LibNaCl_LIBRARY}
        CACHE STRING "LibNaCl crypto libraries" FORCE)

include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(LibNaCl DEFAULT_MSG
        LibNaCl_LIBRARIES
        LibNaCl_LIBRARY
        LibNaCl_INCLUDE_DIR
        LibNaCl_RANDOM
        )

mark_as_advanced(
        LibNaCl_ROOT_DIR
        LibNaCl_INCLUDE_DIR
        LibNaCl_LIBRARY
        LibNaCl_LIBRARIES
        LibNaCl_RANDOM
)