//
// Created by silenteh on 25/10/15.
//

#ifndef CRYPTOENGINE_FILEUTILS_H
#define CRYPTOENGINE_FILEUTILS_H

// Sizes of the keys and the nonce
#define KEY_SIZE 32
#define SIGNING_KEY_SIZE 64
#define NONCE_SIZE 24

// Salt information
// this is the master key for deriving the salt used in the HKDF
#define NONCE_MASTER_KEY_NAME "_nonce.key"
#define SALT_KEY_NAME "_salt.key"
#define SALT_KEY_SIZE KEY_SIZE

// Symmetric
#define SECRET_KEY_NAME "_secret.key"

// Asymmetric
#define PUBLIC_KEY_NAME "_public.key"
#define PRIVATE_KEY_NAME "_private.key"
// Signing
#define PUBLIC_SIGN_KEY_NAME "_public_sign.key"
#define PRIVATE_SIGN_KEY_NAME "_private_sign.key"

// predefined name for storing the keys
#define KEYS_FOLDER "keys"

// this is definitely not cross-platform: folder separator
#define PATH_SEPARATOR "/"

#include <string>

enum KeyType { kHKDFMaster, kSalt, kSecret, kPublic, kPrivate, kPublicSign, kPrivateSign };

class FileUtils {
private:
    // the context prefix coming from the CryptoEngine
    std::string context_;
    bool FileFolderExists(const std::string &name);
    // private helper name which concatenates strings to form the correct key full name based on the key type
    std::string BuildKeyName(KeyType key_type);


public:

    FileUtils(std::string context);

    // check if the specific key exists
    bool KeyExists(const KeyType key_type);

    // creates the keys folder locally and sets the permissions to 400
    bool CreateKeysFolder();

    // check whether the KEYS folder exists
    bool FolderExists();

    // writes the key content inside the keys folder and sets the permissions to 400
    bool WriteKey(const std::string &data, const KeyType key_type);

    // read the key back starting from the key name
    std::string ReadKey(const KeyType key_type);

    // Delete a specific key file
    bool DeleteKey(const KeyType key_type);

};


#endif //CRYPTOENGINE_FILEUTILS_H
