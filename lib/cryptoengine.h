//
// Created by silenteh on 25/10/15.
//

// This files contain the encryption and decryption functions exported in this library

#ifndef CRYPTOENGINE_CRYPTO_H
#define CRYPTOENGINE_CRYPTO_H

// this needs to be a string in this implementation
#define TCP_MESSAGE_VERSION 0

#include <string>
#include <vector>

#include "cryptoengine_file_utils.h"
#include "cryptoengine_message.h"
#include "cryptoengine_encrypted_message.h"
#include "cryptoengine_hkdf_utils.h"
#include "verification_engine.h"


class CryptoEngine {

private:
    // the context used both for the HKDF and also to load the correct keys based on the hostname for instance
    // of the machines one wants to securely communicate with
    std::string context_;

    // these are the public and private keys used for asymmetric encryption
    std::string public_key_;
    std::string private_key_;

    // these are the public and private keys used for signing the message. This is for non-repudiation.
    std::string sign_public_key_;
    std::string sign_private_key_;

    // this is the secret key used for symmetric encryption
    // this for instance shuld be copied manually and securely from one server to the other
    std::string secret_key_;

    // the public key of the remote server
    std::string peer_public_key_;

    // the salt for deriving nonce
    std::string salt_;

    // the nonce master key for deriving more from it using HKDF
    std::string nonce_master_key_;

    FileUtils file_utils_;

    // HKDF to derive nonce
    HKDFUtils hkdf_ ;


public:

    // the constructor load or generates all the needed keys using the RAND_bytes function from BoringSSL
    // IMPORTANT:
    // also stores them in a file inside the `keys` folder in the form: {$context}_salt.key or {$context}_secret,key for instance
    // if it fails to generate or load any of the needed keys then it throws an exception defined in crypto.cpp
    CryptoEngine(const std::string &context);

    // Return the public key
    inline std::string PublicKey() {
        return public_key_;
    }

    // Return the Signing public key
    inline std::string SignPublicKey() {
        return sign_public_key_;
    }

    // ======== SYMMETRIC
    // Encrypt (authenticated encryption) with a randomly generated secret key
    // Sets automatically the type to zero
    EncryptedMessage Encrypt(const Message &message);

    // Encrypt (authenticated encryption) with a randomly generated secret key
    EncryptedMessage Encrypt(const std::string &clear_text);

    // Decrypt with the same key but from the message bytes
    Message Decrypt(const std::vector<unsigned char> message_bytes);

    // ============================================================================
    // ========ASYMMETRIC

    // Encrypt with the recipient public key
    // Sign with the private key
    // Message type is: 0
    EncryptedMessage EncryptWithPublicKey(const Message &message, const VerificationEngine &ver_engine);

    // Encrypt with the recipient public key
    // Sign with the private key
    EncryptedMessage EncryptWithPublicKey(const std::string &clear_test, const VerificationEngine &ver_engine);

    // === Decryption
    // Decrypt from the message bytes
    Message DecryptWithPublicKey(const std::vector<unsigned char> message_bytes, const VerificationEngine &ver_engine);


    // Encrypt with the recipient public key and sign with the sender signing private key
    // Sign with the sender signing private key
    EncryptedMessage EncryptWithPublicKeyAndSign(const Message &message, const VerificationEngine &ver_engine);

    // Messgae type is zero here
    EncryptedMessage EncryptWithPublicKeyAndSign(const std::string &clear_text, const VerificationEngine &ver_engine);


    // Decrypt with the same key but from the message bytes (from bytes)
    Message DecryptWithPublicKeyAndVerify(const std::vector<unsigned char> message_bytes, const VerificationEngine &ver_engine);





};


#endif //CRYPTOENGINE_CRYPTO_H
