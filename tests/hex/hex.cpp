//
// Created by silenteh on 25/10/15.
//

#include "gtest/gtest.h"
#include "hex.h"

namespace {

    class HexTest : public ::testing::Test {

// The fixture for testing class Foo.
    protected:
        // You can remove any or all of the following functions if its body
        // is empty.

        HexTest() {
            // You can do set-up work for each test here.
        }

        virtual ~HexTest() {
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


    // Tests that the Foo::Bar() method does Abc.
    TEST(Hex, Encode) {

        std::cout << "=========================================" << std::endl;

        // Encoding
        std::string sentence("the quick brown fox jumps over the lazy dog");

        std::string hex_encoded = Hex::Encode(sentence);

        ASSERT_NE(sentence, hex_encoded);

        std::cout << hex_encoded << std::endl;

        std::string hex_decoded = Hex::Decode(hex_encoded);

        std::cout << hex_decoded << std::endl;

        ASSERT_EQ(sentence, hex_decoded);

        std::cout << "=========================================" << std::endl;


    }
    // Tests that the Foo::Bar() method does Abc.
    TEST(Hex, EncodeLikeKey) {
        std::cout << "=========================================" << std::endl;

        // Encoding
        std::string sentence("this is a test public key");

        std::string hex_encoded = Hex::Encode(sentence);

        std::cout << hex_encoded << std::endl;

        std::string hex_decoded = Hex::Decode(hex_encoded);

        std::cout << hex_decoded << std::endl;

        ASSERT_EQ(sentence, hex_decoded);
        std::cout << "=========================================" << std::endl;


    }
}
