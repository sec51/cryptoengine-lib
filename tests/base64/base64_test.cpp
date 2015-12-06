//
// Created by silenteh on 25/10/15.
//

#include "gtest/gtest.h"
#include <string>
#include <iostream>

#include "base64.h"

namespace {

    class Base64Test : public ::testing::Test {

// The fixture for testing class Foo.
    protected:
        // You can remove any or all of the following functions if its body
        // is empty.

        Base64Test() {
            // You can do set-up work for each test here.
        }

        virtual ~Base64Test() {
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
    TEST(Base64, Encode) {

        std::cout << "=========================================" << std::endl;

        // Encoding
        std::string sentence("the quick brown fox jumps over the lazy dog");
        Base64 encoder;
        std::string base64_encoded = encoder.Encode(sentence);

        std::cout << base64_encoded << std::endl;

        std::string base64_decoded = encoder.Decode(base64_encoded, sentence.size());

        std::cout << base64_decoded << std::endl;

        ASSERT_EQ(sentence, base64_decoded);

        std::cout << "=========================================" << std::endl;


    }
    // Tests that the Foo::Bar() method does Abc.
    TEST(Base64, EncodeLikeKey) {
        std::cout << "=========================================" << std::endl;

        // Encoding
        std::string sentence("this is a test public key");
        Base64 encoder;
        std::string base64_encoded = encoder.Encode(sentence);

        std::cout << base64_encoded << std::endl;

        std::string base64_decoded = encoder.Decode(base64_encoded, sentence.size());

        std::cout << base64_decoded << std::endl;

        ASSERT_EQ(sentence, base64_decoded);
        std::cout << "=========================================" << std::endl;


    }
}
