## Setup the CryptoEngineLib target
add_library (CryptoEngineLib OBJECT
        cryptoengine.h
        cryptoengine.cpp
        cryptoengine_hkdf_utils.h cryptoengine_hkdf_utils.cpp
        cryptoengine_file_utils.h cryptoengine_file_utils.cpp
        base64.cpp base64.h
        cryptoengine_message.cpp cryptoengine_message.h conversions.cpp conversions.h
        cryptoengine_encrypted_message.h cryptoengine_encrypted_message.cpp verification_engine.cpp verification_engine.h
        )


if(LIBNACL_FOUND)
    include_directories(${LibNaCl_INCLUDE_DIR})
    target_include_directories(CryptoEngineLib PUBLIC ${LibNaCl_INCLUDE_DIR})
endif()

if(BORINGSSL_ENABLED)
    include_directories(BEFORE SYSTEM ${BoringSSL_INCLUDE_DIR})
    target_include_directories(CryptoEngineLib BEFORE PUBLIC ${BoringSSL_INCLUDE_DIR})
else()
    include_directories("BEFORE ${OPENSSL_INCLUDE_DIR}")
    target_include_directories (CryptoEngineLib BEFORE PUBLIC ${OPENSSL_INCLUDE_DIR})
endif()