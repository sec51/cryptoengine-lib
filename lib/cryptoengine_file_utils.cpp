//
// Created by silenteh on 25/10/15.
//

#include "cryptoengine_file_utils.h"

#include <sys/stat.h>
#include <sstream>
#include <iostream>
#include <fstream>
#include <errno.h>
#include <stdio.h>
#include <exception>
#include <string.h>
#include "base64.h"

class FileUtilsInitializationFailure: public std::exception
{
    virtual const char* what() const throw()
    {
        return "Could not initialize the file utilities!";
    }
} g_file_utils_initialization_failure;

// the constructor creates the basic folder
// if this files it means we are in an undefined state....
FileUtils::FileUtils(std::string context) {
    if (context.empty()) {
        // throw exception
        std::cout << "The context identifier passed to file utilities helper is empty" << std::endl;
        throw g_file_utils_initialization_failure;
    }
    // assign the private var context_
    context_ = context;

    if(!CreateKeysFolder()) {
        std::cout << "File utlities helper could not create the keys folder" << std::endl;
        throw g_file_utils_initialization_failure;
    }
}

// generic method to check whether a file or folder exists
bool FileUtils::FileFolderExists(const std::string &name) {
    struct stat buffer;
    return (stat (name.c_str(), &buffer) == 0);
}

// check if the keys folder exists
bool FileUtils::FolderExists() {
    return FileFolderExists(KEYS_FOLDER);
}

// check if the specific file exists
bool FileUtils::KeyExists(KeyType key_type) {
    return FileFolderExists(BuildKeyName(key_type));
}

// creates the keys folder locally and sets the permissions to 400
bool FileUtils::CreateKeysFolder(){

    // if the folder already exists, nothing to do
    if (FolderExists()) {
        return true;
    }

    // create the folder with permissions: drwx
    const int dir_err = mkdir(KEYS_FOLDER, S_IRUSR | S_IWUSR | S_IXUSR);
    if (-1 == dir_err)
    {
        // if there was an error then print the reason
        std::cout << "Error creating directory! Error code: " << strerror(errno) << std::endl;
        return false;
    }
    return true;
};

// writes the key content inside the keys folder and sets the permissions to 400
bool FileUtils::WriteKey(const std::string &data, const KeyType key_type) {

    // check whether the same file exists
    if (KeyExists(key_type)) {

        // print the error
        std::cout << "Error saving the key file: " << BuildKeyName(key_type) << " ! The key already exists!";

        return false;
    }

    // Init the base64 object
    Base64 base64;

    // check whether the key size is correct
    int32_t key_size = KEY_SIZE;
    if (key_type == kPrivateSign) {
        key_size = SIGNING_KEY_SIZE;
    }

    if (data.size() != key_size) {
        std::cout << "Key size is wrong: " << BuildKeyName(key_type) << " got " <<  data.size()
        << " instead of " << key_size << std::endl;
        return false;
    }

    // encode the data_string to base64
    std::string base64_encoded_data = base64.Encode(data);

    // Build the string full path
    std::string key_file_full_path = BuildKeyName(key_type);

    // init the object which will write to the file
    std::ofstream key_file;

    // open the file for writing
    key_file.open (key_file_full_path);

    // set the permission of the file, before writing content to it.
    // set the file in READ ONLY MODE
    // after a key has been generated there is no reasons to tamper with !
    int permission_action_result = chmod(key_file_full_path.data(), S_IRUSR);
    if (-1 == permission_action_result) {

        // close the file
        key_file.close();

        // print the error
        std::cout << "Error changing the key file" << key_file_full_path << " permissions! Error code: " << strerror(errno) << std::endl;

        return false;
    }
    // write the key content
    key_file << base64_encoded_data;

    // close the file
    key_file.close();

    return true;

};

// read the key back
std::string FileUtils::ReadKey(const KeyType key_type) {

    std::string line("");
    std::ifstream key_file(BuildKeyName(key_type));

    if (key_file.is_open())
    {
        // read 1 songle line of the file
        // the key is 1 line long
        std::getline(key_file, line);

        // init the base64 encoder
        Base64 encoder;

        // decode the content of the line
        std::string key;
        switch (key_type) {
            case kPrivateSign:
                key = encoder.Decode(line, SIGNING_KEY_SIZE);
                break;
            default:
                key = encoder.Decode(line, KEY_SIZE);
        }

        // close the file, we do not need to read from it anymore
        key_file.close();

        // return the decoded content
        return key;
    }
    else {
        std::cout << "Unable to open key file " << BuildKeyName(key_type) << std::endl;
    }

    std::cout << "Should not have gotten here *****************";
    // return basically an empty string
    return line;
}


bool FileUtils::DeleteKey(const KeyType key_type) {

    // nothing to do here if the key do not exist
    if (!KeyExists(key_type)) {
        return true;
    }

    // Build the string full path
    std::string key_file_full_path = BuildKeyName(key_type);

    int delete_result = std::remove(key_file_full_path.data());
    if (-1 == delete_result) {
        std::cout << "Unable to delete the key file " << key_file_full_path << std::endl;
        return false;
    }

    return true;

}



std::string FileUtils::BuildKeyName(KeyType key_type) {

    // append together the string in the form: /keys/key_name
    std::ostringstream string_stream;
    string_stream << KEYS_FOLDER;
    string_stream << PATH_SEPARATOR;
    string_stream << context_;

    switch (key_type) {
        case kHKDFMaster:
            string_stream << NONCE_MASTER_KEY_NAME;
            return string_stream.str();
        case kSecret:
            string_stream << SECRET_KEY_NAME;
            return string_stream.str();
        case kPrivate:
            string_stream << PRIVATE_KEY_NAME;
            return string_stream.str();
        case kPublic:
            string_stream << PUBLIC_KEY_NAME;
            return string_stream.str();
        case kPrivateSign:
            string_stream << PRIVATE_SIGN_KEY_NAME;
            return string_stream.str();
        case kPublicSign:
            string_stream << PUBLIC_SIGN_KEY_NAME;
            return string_stream.str();
        case kSalt:
            string_stream << SALT_KEY_NAME;
            return string_stream.str();
    }
}