//
// Created by silenteh on 25/10/15.
//

#include "gtest/gtest.h"
#include <string>
#include <array>

#include "cryptoengine_file_utils.h"

namespace {

    class FileUtilsTest : public ::testing::Test {

// The fixture for testing class Foo.
    protected:
        // You can remove any or all of the following functions if its body
        // is empty.

        FileUtilsTest() {
            // You can do set-up work for each test here.
        }

        virtual ~FileUtilsTest() {
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
    TEST(FileUtils, CreateKeysFolder) {

        FileUtils utils("test");
        ASSERT_TRUE(utils.CreateKeysFolder());
        ASSERT_TRUE(utils.FolderExists());

    }

    // Tests that the Foo::Bar() method does Abc.
    TEST(FileUtils, WriteKey) {

        std::string key("this is a test public key !!!!!!"); // it needs to be 32 xhar long

        FileUtils utils("test");

        // create the 'keys' folder
        ASSERT_TRUE(utils.CreateKeysFolder());

        // check if the 'keys' folder exists
        ASSERT_TRUE(utils.FolderExists());

        // write a key file to it
        ASSERT_TRUE(utils.WriteKey(key, kPublic));

        // check if the key file exists
        ASSERT_TRUE(utils.KeyExists(kPublic));

        // read the key file back and compare
        std::string stored_key = utils.ReadKey( kPublic);

        // check that the key content is the same as we stored it
        ASSERT_EQ(key, stored_key);

        // delete the key file
        ASSERT_TRUE(utils.DeleteKey(kPublic));

    }
}
