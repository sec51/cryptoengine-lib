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

if(${LibNaCl_ROOT_DIR})
    MESSAGE("-- Lib NaCl ROOT not set, looking into the system dirs")
    find_path(LibNaCl_ROOT_DIR
        NAMES include/crypto_box.h include/crypto_secretbox.h lib/libnacl.a
        HINTS ${LibNaCl_ROOT_DIR}
        )
else()
    if(${CMAKE_HOST_SYSTEM_PROCESSOR} STREQUAL "x86_64")
        MESSAGE("-- Lib NaCl ROOT defined: ${LibNaCl_ROOT_DIR}")
        find_path(LibNaCl_ROOT_DIR
                NAMES include/amd64/crypto_box.h include/amd64/crypto_secretbox.h lib/amd64/libnacl.a
                HINTS ${LibNaCl_ROOT_DIR}
                )
    else()
        MESSAGE("Lib NaCl ROOT not defined, looking into x86 folder")
        find_path(LibNaCl_ROOT_DIR
                NAMES include/x86/crypto_box.h include/x86/crypto_secretbox.h lib/x86/libnacl.a
                HINTS ${LibNaCl_ROOT_DIR}
                )
    endif()
endif()


find_path(LibNaCl_INCLUDE_DIR
        NAMES crypto_box.h crypto_secretbox.h
        HINTS ${LibNaCl_ROOT_DIR}/include ${LibNaCl_ROOT_DIR}/include/amd64 ${LibNaCl_ROOT_DIR}/include/x86
        PATHS ${LibNaCl_ROOT_DIR}/include ${LibNaCl_ROOT_DIR}/include/amd64 ${LibNaCl_ROOT_DIR}/include/x86
        )

find_library(LibNaCl_LIBRARY
        NAMES libnacl.a
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
        )

mark_as_advanced(
        LibNaCl_ROOT_DIR
        LibNaCl_INCLUDE_DIR
        LibNaCl_LIBRARY
        LibNaCl_LIBRARIES
)