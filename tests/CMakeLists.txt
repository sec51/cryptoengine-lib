# add the test executable to execute the tests
add_executable (CryptoEngineTest main.cpp
        file_utils/file_utils_test.cpp
        hkdf/hkdf_test.cpp
        conversions/conversions.cpp message/message.cpp engine/cryptoengine.cpp hex/hex.cpp)

target_link_libraries(CryptoEngineTest LINK_PUBLIC CryptoEngine gtest gtest_main)
