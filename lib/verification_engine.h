//
// Created by silenteh on 21/11/15.
//

/*
 * This class allows to load the peer public keys (public key and signing public key)
 * for decryption verification, both in public key and signature
 */

#ifndef CRYPTOENGINE_VERIFICATIONENGINE_H
#define CRYPTOENGINE_VERIFICATIONENGINE_H

#include <string>

#include "cryptoengine_file_utils.h"

class VerificationEngine {

private:

    std::string context_;

    // The signing public key
    std::string sign_public_key_;

    // The encryption public key
    std::string public_key_;


public:
    // Instantiate a new Verification Engine object
    // The context is the prefix of the file to load
    VerificationEngine(const std::string &context);

    // Instantiate a new Verification Engine object
    // This passes explicitely the keys
    VerificationEngine(const std::string &public_key, const std::string &signing_public_key);


    // Accessor(s)
    // The encrypted message transported
    inline std::string public_key() const{
        return public_key_;
    }

    // The nonce
    inline std::string signing_public_key() const{
        return sign_public_key_;
    }

};


#endif //CRYPTOENGINE_VERIFICATIONENGINE_H
