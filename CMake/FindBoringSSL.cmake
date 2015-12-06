# - Try to find boringssl include dirs and libraries
#
# Usage of this module as follows:
#
#     find_package(BoringSSL)
#
# Variables used by this module, they can change the default behaviour and need
# to be set before calling find_package:
#
#  BoringSSL_ROOT_DIR          Set this variable to the root installation of
#                            boringssl if the module has problems finding the
#                            proper installation path.
#
# Variables defined by this module:
#
#  BORINGSSL_FOUND             System has boringssl, include and library dirs found
#  BoringSSL_INCLUDE_DIR       The boringssl include directories.
#  BoringSSL_LIBRARIES         The boringssl libraries.
#  BoringSSL_CRYPTO_LIBRARY    The boringssl crypto library.
#  BoringSSL_SSL_LIBRARY       The boringssl ssl library.

find_path(BoringSSL_ROOT_DIR
        NAMES include/openssl/ssl.h include/openssl/base.h include/openssl/hkdf.h
        HINTS ${BoringSSL_ROOT_DIR}
        )

find_path(BoringSSL_INCLUDE_DIR
        NAMES openssl/ssl.h openssl/base.h openssl/hkdf.h
        HINTS ${BoringSSL_ROOT_DIR}/include
        )

find_library(BoringSSL_SSL_LIBRARY
        NAMES libssl.a
        HINTS ${BoringSSL_ROOT_DIR}/build/ssl
        )

find_library(BoringSSL_CRYPTO_LIBRARY
        NAMES libcrypto.a
        HINTS ${BoringSSL_ROOT_DIR}/build/crypto
        )

set(BoringSSL_LIBRARIES ${BoringSSL_SSL_LIBRARY} ${BoringSSL_CRYPTO_LIBRARY}
        CACHE STRING "BoringSSL SSL and crypto libraries" FORCE)

include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(BoringSSL DEFAULT_MSG
        BoringSSL_LIBRARIES
        BoringSSL_INCLUDE_DIR
        )

mark_as_advanced(
        BoringSSL_ROOT_DIR
        BoringSSL_INCLUDE_DIR
        BoringSSL_LIBRARIES
        BoringSSL_CRYPTO_LIBRARY
        BoringSSL_SSL_LIBRARY
)