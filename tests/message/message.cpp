//
// Created by silenteh on 02/11/15.
//

#include <limits>
#include <array>
#include <vector>
#include "gtest/gtest.h"

#include "cryptoengine_message.h"
#include "cryptoengine_file_utils.h"

namespace {

    class MessageTest : public ::testing::Test {

// The fixture for testing class Foo.
    protected:
        // You can remove any or all of the following functions if its body
        // is empty.

        MessageTest() {
            // You can do set-up work for each test here.
        }

        virtual ~MessageTest() {
            // You can do clean-up work that doesn't throw exceptions here.
        }

        // If the constructor and destructor are not enough for setting up
        // and cleaning up each test, you can define the following methods:

        virtual void SetUp() {
            // Code here will be called immediately after the constructor (right
            // before each test).
        }

        virtual void TearDown() {
            // Code here will be called immediately after each test (right
            // before the destructor).
        }

        // Objects declared here can be used by all tests in the test case for HKDF.
    };


    // Test int32_t BigEndian conversions
    TEST(Message, ToBytesString) {

        std::cout << "=========================================" << std::endl;

//        // create the nonce
//        std::array<char, NONCE_SIZE> nonce_bytes;
//        nonce_bytes.fill('7');
//        std::string nonce(nonce_bytes.data(),NONCE_SIZE);

        // create the message
        std::string text("The quick brown fox jumps over the lazy dog");
        Message m(text, 1);

        // convert to bytes string
        std::string message_bytes = m.ToBytesString();

        // convert back
        Message converted = Message::FromBytesString(message_bytes);


        std::cout << "Message: " << m.message() << std::endl;
        std::cout << "Version: " << m.version() << std::endl;
        std::cout << "Type: " << m.type() << std::endl;
        std::cout << "Total length:" << m.length() << std::endl;

        ASSERT_EQ(converted.message(), m.message());
        ASSERT_EQ(converted.version(), m.version());
        ASSERT_EQ(converted.type(), m.type());
        ASSERT_EQ(converted.length(), m.length());

        std::cout << "=========================================" << std::endl;

    }

}