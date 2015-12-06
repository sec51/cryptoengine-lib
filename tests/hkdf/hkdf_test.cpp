//
// Created by silenteh on 24/10/15.
//

#ifndef CRYPTOENGINE_HKDF_TEST_H
#define CRYPTOENGINE_HKDF_TEST_H

#include <exception>

#include "gtest/gtest.h"
#include "openssl/rand.h"

#include "cryptoengine_hkdf_utils.h"
#include "cryptoengine.h"


namespace {

    class HKDFTest : public ::testing::Test {

// The fixture for testing class Foo.
    protected:
        // You can remove any or all of the following functions if its body
        // is empty.

        HKDFTest() {
            // You can do set-up work for each test here.
        }

        virtual ~HKDFTest() {
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
    TEST(HKDF, deriveNonce) {

        std::string context("test-context");

        HKDFUtils hkdf(context);
        FileUtils file_utils(context);


        // otherwise create a new salt key reading from /dev/urandom via boringssl
        // init a uint8_t buffer
        uint8_t buffer_master[KEY_SIZE];
        // init a uint8_t buffer
        uint8_t buffer_salt[KEY_SIZE];

        // read SALT_KEY_SIZE bytes into the buffer
        int random_result = RAND_bytes(buffer_master, KEY_SIZE);
        int random_result_salt = RAND_bytes(buffer_salt, KEY_SIZE);

        // means it could not get enough random bytes - return
        ASSERT_TRUE(random_result == 1);

        ASSERT_TRUE(random_result_salt == 1);

        ASSERT_EQ(sizeof(buffer_master), KEY_SIZE);
        ASSERT_EQ(sizeof(buffer_salt), KEY_SIZE);

        // if we've got here then write the random bytes on the SALT_KEY_NAME file
        std::string master_key = std::string(reinterpret_cast<const char *>(buffer_master), KEY_SIZE);
        std::string salt_key = std::string(reinterpret_cast<const char *>(buffer_salt), KEY_SIZE);


        try {
            std::vector<std::string> previous;
            for (int i = 0; i < 100; ++i) {
                std::string nonce = hkdf.DeriveNonce(master_key, salt_key);

                // check if the vector contains the same nonce already generated
                //std::cout << "Loop number: " << i << std::endl;

                // make sure the output is not the same as one of the keys
                ASSERT_NE(master_key, nonce);
                ASSERT_NE(salt_key, nonce);

                for (auto & element : previous) {
                    // check if the nonce was the same as the previous one
                    ASSERT_NE(element, nonce);
                }


                // assign the nonce to the previous
                previous.push_back(nonce);

                //std::cout << nonce << std::endl;
                //std::cout << "Nonce size: " << nonce.size() << std::endl;
                //std::cout << "Nonce: " << nonce.data() << std::endl;
                ASSERT_EQ(nonce.size(), NONCE_SIZE);
            }
        } catch (std::exception &e) {
            std::cout << e.what() << std::endl;
            throw e;
        }
    }
}

#endif //CRYPTOENGINE_HKDF_TEST_H
