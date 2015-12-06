# - Try to find LibCryptoEngine include dirs and libraries
#
# Usage of this module as follows:
#
#     find_package(LibCryptoEngine)
#
# Variables used by this module, they can change the default behaviour and need
# to be set before calling find_package:
#
#  LibCryptoEngine_ROOT_DIR         Set this variable to the root installation of
#                                   LibCryptoEngine if the module has problems finding the
#                                   proper installation path.
#
# Variables defined by this module:
#
#  LibCryptoEngine_FOUND             System has LibCryptoEngine, include and library dirs found
#  LibCryptoEngine_INCLUDE_DIR       The LibCryptoEngine include directories.
#  LibCryptoEngine_LIBRARY           The LibCryptoEngine libraries.



find_path(LibCryptoEngine_ROOT_DIR
        NAMES include/verification_engine.h include/cryptoengine.h lib/libCryptoEngine.a
        HINTS ${LibCryptoEngine_ROOT_DIR}
        )

find_path(LibCryptoEngine_INCLUDE_DIR
        NAMES verification_engine.h cryptoengine.h
        HINTS ${LibCryptoEngine_ROOT_DIR}/include
        PATHS ${LibCryptoEngine_ROOT_DIR}/include
        )

find_library(LibCryptoEngine_LIBRARY
        NAMES LibCryptoEngine.a
        HINTS ${LibCryptoEngine_ROOT_DIR}/lib
        PATHS ${LibCryptoEngine_ROOT_DIR}/lib
        )

include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(LibCryptoEngine DEFAULT_MSG
        LibCryptoEngine_LIBRARY
        LibCryptoEngine_INCLUDE_DIR
        )

mark_as_advanced(
        LibCryptoEngine_ROOT_DIR
        LibCryptoEngine_INCLUDE_DIR
        LibCryptoEngine_LIBRARY
)