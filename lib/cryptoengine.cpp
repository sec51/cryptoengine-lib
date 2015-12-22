//
// Created by silenteh on 25/10/15.
//

#include <exception>
#include <iostream>
#include <sstream>

#include "cryptoengine.h"
#include "cryptoengine_conversions.h"
#include "openssl/rand.h"

// NaCl
#include "crypto_box.h"
#include "crypto_secretbox.h"
#include "crypto_sign.h"
#include "crypto_hash.h"
#include "crypto_verify_32.h"



class CryptoEngineInitializationFailure: public std::exception
{
    virtual const char* what() const throw()
    {
        return "Could not initialize the crypto engine object properly! It's not safe to use it! Leave the building !";
    }
} g_crypto_engine_initialization_failure;

class CryptoEngineEncryptionFailure: public std::exception
{
    virtual const char* what() const throw()
    {
        return "Could not encrypt!";
    }
} g_crypto_engine_encryption_failure;


// CryptoEngine constructor
CryptoEngine::CryptoEngine(const std::string &context):context_(context), file_utils_(context), hkdf_(context){

    try {

        if(context.empty()) {
            std::cout << "Cannot initialize the crypto engine with an empty context." << std::endl;
            throw g_crypto_engine_initialization_failure;
        }

        // Create the standard keys folder
        // IMPORTANT DO NOT REMOVE !!!!!
        // THE FOLDER CREATION CAN FAIL !!!!
        if(!file_utils_.CreateKeysFolder()) {
            std::cout << "Could not create the folder where to store the keys." << std::endl;
            throw g_crypto_engine_initialization_failure;
        }

        // ****************************************************
        // ######## HKDF MASTER KEY
        // Always generate a new HKDF master key when the crypto engine is initialized

        // create the nonce master key
        // init a uint8_t buffer
        uint8_t buffer_hkdf_master_key[KEY_SIZE];


        // read KEY_SIZE bytes into the buffer
        int hkdf_random_result = RAND_bytes(buffer_hkdf_master_key, KEY_SIZE);
        if (1 != hkdf_random_result) {
            std::cout << "Could not generate the HKDF master key. Not enough entropy" << std::endl;
            throw g_crypto_engine_initialization_failure;
        }

        nonce_master_key_ = std::string(reinterpret_cast<const char *>(buffer_hkdf_master_key), KEY_SIZE);

        if (nonce_master_key_.size() != KEY_SIZE) {
            std::cout << "The loaded HKDF master key is empty or too short." << std::endl;
            throw g_crypto_engine_initialization_failure;
        }


        // ****************************************************
        // ######## SALT KEY
        // Always generate a new SALT key when the crypto engine is initialized

        // create the nonce master key
        // init a uint8_t buffer
        uint8_t buffer_salt_key[KEY_SIZE];

        // read KEY_SIZE bytes into the buffer
        int salt_random_result = RAND_bytes(buffer_salt_key, KEY_SIZE);
        if (1 != salt_random_result) {
            std::cout << "Could not generate the salt key. Not enough entropy" << std::endl;
            throw g_crypto_engine_initialization_failure;
        }

        salt_ = std::string(reinterpret_cast<const char *>(buffer_salt_key), KEY_SIZE);

        if (salt_.size() != KEY_SIZE) {
            std::cout << "The loaded salt key is empty or too short." << std::endl;
            throw g_crypto_engine_initialization_failure;
        }


        // ****************************************************
        // ######## SECRET KEY
        // if the key already exists then load it
        if(file_utils_.KeyExists(kSecret)) {
            // read the key back
            secret_key_ = file_utils_.ReadKey(kSecret);
            // check the size of the key which should match the predefined one
            if (secret_key_.size() != KEY_SIZE) {
                std::cout << "The loaded secret key is empty or too short." << std::endl;
                throw g_crypto_engine_initialization_failure;
            }

        } else { // otherwise generate one

            // create the nonce master key
            // init a uint8_t buffer
            uint8_t buffer_secret_key[KEY_SIZE];

            // read KEY_SIZE bytes into the buffer
            int secret_random_result = RAND_bytes(buffer_secret_key, KEY_SIZE);
            if (1 != secret_random_result) {
                std::cout << "Could not generate the secret key. Not enough entropy" << std::endl;
                throw g_crypto_engine_initialization_failure;
            }

            secret_key_ = std::string(reinterpret_cast<const char *>(buffer_secret_key), KEY_SIZE);

            if (secret_key_.size() != KEY_SIZE) {
                std::cout << "The loaded secret key is empty or too short." << std::endl;
                throw g_crypto_engine_initialization_failure;
            }

            if (!file_utils_.WriteKey(secret_key_,kSecret)) {
                std::cout << "Could not store the secret key." << std::endl;
                throw g_crypto_engine_initialization_failure;
            }
        }

        // ****************************************************
        // ######## PUBLIC/PRIVATE KEY

        if(file_utils_.KeyExists(kPrivate) && file_utils_.KeyExists(kPublic)) {
            // read the keys back
            private_key_ = file_utils_.ReadKey(kPrivate);
            // check the size of the key which should match the predefined one
            if (private_key_.size() != KEY_SIZE) {
                std::cout << "The loaded private key is empty or too short." << std::endl;
                throw g_crypto_engine_initialization_failure;
            }

            public_key_ = file_utils_.ReadKey(kPublic);
            // check the size of the key which should match the predefined one
            if (public_key_.size() != KEY_SIZE) {
                std::cout << "The loaded public key is empty or too short." << std::endl;
                throw g_crypto_engine_initialization_failure;
            }

        } else { // otherwise generate them

            // generate the private/public key
            public_key_ = crypto_box_keypair(&private_key_);

            if (private_key_.size() != KEY_SIZE) {
                std::cout << "The loaded private key is empty or too short." << std::endl;
                throw g_crypto_engine_initialization_failure;
            }

            if (public_key_.size() != KEY_SIZE) {
                std::cout << "The loaded public key is empty or too short." << std::endl;
                throw g_crypto_engine_initialization_failure;
            }

            if (!file_utils_.WriteKey(private_key_,kPrivate)) {
                std::cout << "Could not store the private key." << std::endl;
                throw g_crypto_engine_initialization_failure;
            }
            if (!file_utils_.WriteKey(public_key_, kPublic)) {
                std::cout << "Could not store the public key." << std::endl;
                throw g_crypto_engine_initialization_failure;
            }

        }

        // ****************************************************
        // ######## PUBLIC/PRIVATE SIGNING KEY

        if(file_utils_.KeyExists(kPrivateSign) && file_utils_.KeyExists(kPublicSign)) {
            // read the keys back
            sign_private_key_ = file_utils_.ReadKey(kPrivateSign);
            // check the size of the key which should match the predefined one
            if (sign_private_key_.size() != SIGNING_KEY_SIZE) {
                std::cout << "The loaded private signing key is empty or too short." << std::endl;
                throw g_crypto_engine_initialization_failure;
            }

            sign_public_key_ = file_utils_.ReadKey(kPublicSign);
            // check the size of the key which should match the predefined one
            if (sign_public_key_.size() != KEY_SIZE) {
                std::cout << "The loaded public signing key is empty or too short." << std::endl;
                throw g_crypto_engine_initialization_failure;
            }

        } else { // otherwise generate them

            // generate the private/public signing key
            sign_public_key_ = crypto_sign_keypair(&sign_private_key_);

            if (sign_private_key_.size() != SIGNING_KEY_SIZE) {
                std::cout << "The loaded private signing key is empty or too short." << std::endl;
                throw g_crypto_engine_initialization_failure;
            }

            if (sign_public_key_.size() != KEY_SIZE) {
                std::cout << "The loaded public signing key is empty or too short." << std::endl;
                throw g_crypto_engine_initialization_failure;
            }

            if (!file_utils_.WriteKey(sign_private_key_,kPrivateSign)) {
                std::cout << "Could not store the private signing key." << std::endl;
                throw g_crypto_engine_initialization_failure;
            }
            if (!file_utils_.WriteKey(sign_public_key_, kPublicSign)) {
                std::cout << "Could not store the publicsigning key." << std::endl;
                throw g_crypto_engine_initialization_failure;
            }

        }

    } catch (std::exception &e) {
        std::cout << e.what() << std::endl;
        throw g_crypto_engine_initialization_failure;
    }
}

EncryptedMessage CryptoEngine::Encrypt(const Message &message) {


    if (secret_key_.empty()) {
        std::cout << "The secret key for symmetric encryption has not been initialized" << std::endl;
        throw g_crypto_engine_encryption_failure;
    }

    if (nonce_master_key_.empty()) {
        std::cout << "The master key for deriving the nonce is not valid" << std::endl;
        throw g_crypto_engine_encryption_failure;
    }

    if (context_.empty()) {
        std::cout << "The context identifier for deriving the nonce is not valid" << std::endl;
        throw g_crypto_engine_encryption_failure;
    }

    if (salt_.empty()) {
        std::cout << "The salt for deriving the nonce is not valid" << std::endl;
        throw g_crypto_engine_encryption_failure;
    }

    try {
        // generate the Nonce
        std::string nonce = hkdf_.DeriveNonce(nonce_master_key_, salt_);

        // Add the additional data (tcp_verion, type, message) to the string
        std::string full_clear_text(message.ToBytesString());

        // encrypt the message
        std::string cipher_text = crypto_secretbox(full_clear_text, nonce, secret_key_);

        // return the Message object so that it can be serialized to bytes
        return EncryptedMessage(nonce, cipher_text);
    } catch (std::exception& e) {
        std::cout << "CryptoEngine::Encrypt exception: " << e.what() << std::endl;
        throw g_crypto_engine_encryption_failure;
    }
}

EncryptedMessage CryptoEngine::Encrypt(const std::string &clear_text) {
    Message message(clear_text, 0);
    return Encrypt(message);
}

Message CryptoEngine::Decrypt(const std::vector<unsigned char> message_bytes) {

    // Parse from bytes to EcnryptedMessage object
    EncryptedMessage encrypted_msg = EncryptedMessage::FromBytes(message_bytes);

    // get the clear text, with the additional data prepended
    std::string clear_text_with_data(crypto_secretbox_open(encrypted_msg.message(), encrypted_msg.nonce(), secret_key_));

    // convert the string data to a real message: (version, type, clear_text)
    Message message = Message::FromBytesString(clear_text_with_data);

    return message;
}


// Decrypt with the same key but from the message bytes
EncryptedMessage CryptoEngine::EncryptWithPublicKey(const Message &message, const VerificationEngine &ver_engine) {

    // check the public key
    if (ver_engine.public_key().empty()) {
        std::cout << "The recipient public key is empty. Cannot encrypt here." << std::endl;
        throw g_crypto_engine_encryption_failure;
    }

    if (private_key_.empty()) {
        std::cout << "The secret key is empty. Cannot encrypt here." << std::endl;
        throw g_crypto_engine_encryption_failure;
    }

    if (nonce_master_key_.empty()) {
        std::cout << "The master key for deriving the nonce is not valid" << std::endl;
        throw g_crypto_engine_encryption_failure;
    }

    if (context_.empty()) {
        std::cout << "The context identifier for deriving the nonce is not valid" << std::endl;
        throw g_crypto_engine_encryption_failure;
    }

    if (salt_.empty()) {
        std::cout << "The salt for deriving the nonce is not valid" << std::endl;
        throw g_crypto_engine_encryption_failure;
    }

    // means the current crypto engine and the recipient crypto engine have the same public keys !
    // This is NOT SAFE - stop immediately
    if(crypto_verify_32 (reinterpret_cast<const unsigned char *>(public_key_.data()),
                         reinterpret_cast<const unsigned char *>(ver_engine.public_key().data())) == 0) {
        std::cout << "The engine public key and the recipient public key are the same !!! Did you copy the same files to two different peers ? This is NOT SAFE !! STOPPING !" << std::endl;
        throw g_crypto_engine_encryption_failure;
    }

    try {
        // derive the nonce
        std::string nonce = hkdf_.DeriveNonce(nonce_master_key_, salt_);

        // Add the additional data (tcp_verion, type, message) to the string
        std::string full_clear_text(message.ToBytesString());

        // encrypt with the recipient public key
        std::string cipher_text = crypto_box(full_clear_text, nonce, ver_engine.public_key(), private_key_);

        return EncryptedMessage(nonce, cipher_text);
    } catch (std::exception& e) {
        std::cout << "CryptoEngine::Encrypt exception: " << e.what() << std::endl;
        throw g_crypto_engine_encryption_failure;
    }
}

EncryptedMessage CryptoEngine::EncryptWithPublicKey(const std::string &clear_text, const VerificationEngine &ver_engine) {
    Message msg(clear_text, 0);
    return EncryptWithPublicKey(msg, ver_engine);
}

// Decrypt from the message bytes
Message CryptoEngine::DecryptWithPublicKey(const std::vector<unsigned char> message_bytes, const VerificationEngine &ver_engine) {

    // check the public key
    if (ver_engine.public_key().empty()) {
        std::cout << "The recipient public key is empty. Cannot encrypt here." << std::endl;
        throw g_crypto_engine_encryption_failure;
    }

    // from bytes to encrypted message
    EncryptedMessage encrypted_msg = EncryptedMessage::FromBytes(message_bytes);

    // get the clear text, with the additional data prepended
    std::string clear_text_with_data(crypto_box_open(encrypted_msg.message(), encrypted_msg.nonce(), ver_engine.public_key(), private_key_));

    // extract the additional data and build the message
    Message message = Message::FromBytesString(clear_text_with_data);

    return message;

}


EncryptedMessage CryptoEngine::EncryptWithPublicKeyAndSign(const Message &message, const VerificationEngine &ver_engine) {

    // check the public key
    if (ver_engine.public_key().empty()) {
        std::cout << "The recipient public key is empty. Cannot encrypt here." << std::endl;
        throw g_crypto_engine_encryption_failure;
    }

    if (ver_engine.signing_public_key().empty()) {
        std::cout << "The recipient signing public key is empty. Cannot encrypt here." << std::endl;
        throw g_crypto_engine_encryption_failure;
    }

    if (private_key_.empty()) {
        std::cout << "The secret key is empty. Cannot encrypt here." << std::endl;
        throw g_crypto_engine_encryption_failure;
    }

    if (nonce_master_key_.empty()) {
        std::cout << "The master key for deriving the nonce is not valid" << std::endl;
        throw g_crypto_engine_encryption_failure;
    }

    if (context_.empty()) {
        std::cout << "The context identifier for deriving the nonce is not valid" << std::endl;
        throw g_crypto_engine_encryption_failure;
    }

    if (salt_.empty()) {
        std::cout << "The salt for deriving the nonce is not valid" << std::endl;
        throw g_crypto_engine_encryption_failure;
    }

    // means the current crypto engine and the recipient crypto engine have the same public keys !
    // This is NOT SAFE - stop immediately
    if(crypto_verify_32 (reinterpret_cast<const unsigned char *>(public_key_.data()),
                         reinterpret_cast<const unsigned char *>(ver_engine.public_key().data())) == 0) {
        std::cout << "The engine public key and the recipient public key are the same !!! Did you copy the same files to two different peers ? This is NOT SAFE !! STOPPING !" << std::endl;
        throw g_crypto_engine_encryption_failure;
    }

    // derive the nonce
    std::string nonce = hkdf_.DeriveNonce(nonce_master_key_, salt_);

    // calculate the hash of the data
    std::string hash(crypto_hash(message.ToBytesString()));

    // calculate the signature on the hash
    std::string signed_hash = crypto_sign(hash, sign_private_key_);

    // prepend the signature+hash to the message
    std::string signed_message_string = signed_hash + message.ToBytesString();

    // encrypt with the recipient public key
    std::string cipher_text = crypto_box(signed_message_string, nonce, ver_engine.public_key(), private_key_);

    return EncryptedMessage(nonce, cipher_text);
}

EncryptedMessage CryptoEngine::EncryptWithPublicKeyAndSign(const std::string &clear_text, const VerificationEngine &ver_engine) {
    Message msg(clear_text, 0);
    return EncryptWithPublicKeyAndSign(msg, ver_engine);
}

// Decrypt with the same key but from the message bytes (from bytes)
Message CryptoEngine::DecryptWithPublicKeyAndVerify(const std::vector<unsigned char> message_bytes, const VerificationEngine &ver_engine) {

    // check the public key
    if (ver_engine.public_key().empty()) {
        std::cout << "The recipient public key is empty. Cannot encrypt here." << std::endl;
        throw g_crypto_engine_encryption_failure;
    }

    if (ver_engine.signing_public_key().empty()) {
        std::cout << "The recipient signing public key is empty. Cannot encrypt here." << std::endl;
        throw g_crypto_engine_encryption_failure;
    }

    if (private_key_.empty()) {
        std::cout << "The secret key is empty. Cannot encrypt here." << std::endl;
        throw g_crypto_engine_encryption_failure;
    }

    if (nonce_master_key_.empty()) {
        std::cout << "The master key for deriving the nonce is not valid" << std::endl;
        throw g_crypto_engine_encryption_failure;
    }

    if (context_.empty()) {
        std::cout << "The context identifier for deriving the nonce is not valid" << std::endl;
        throw g_crypto_engine_encryption_failure;
    }

    if (salt_.empty()) {
        std::cout << "The salt for deriving the nonce is not valid" << std::endl;
        throw g_crypto_engine_encryption_failure;
    }

    // Convert bytes to encrypted message
    EncryptedMessage encrypted_msg = EncryptedMessage::FromBytes(message_bytes);

    // decrypt
    std::string clear_text_signed_message = crypto_box_open(encrypted_msg.message(), encrypted_msg.nonce(),
                                                            ver_engine.public_key(), private_key_);

    // take the signature out
    std::string signature = clear_text_signed_message.substr(0,128);

    // verify the signature - if it fails it throws an exception
    std::string signed_message = crypto_sign_open(signature,ver_engine.signing_public_key());

    // remove the signing part and the hash
    std::string message_with_data = clear_text_signed_message.substr(128,clear_text_signed_message.size());

    // Convert to message
    Message message = Message::FromBytesString(message_with_data);

    return message;

}