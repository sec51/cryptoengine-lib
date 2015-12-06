//
// Created by silenteh on 21/11/15.
//

#include "verification_engine.h"
#include "cryptoengine_file_utils.h"

#include <exception>
#include <iostream>


class VerificationEngineInitializationFailure: public std::exception
{
    virtual const char* what() const throw()
    {
        return "Could not initialize the verification engine object properly! It's not safe to use it! Leave the building !";
    }
} g_verification_engine_initialization_failure;

VerificationEngine::VerificationEngine(const std::string &context) {

    try {

        if (context.empty()) {
            std::cout << "Cannot initialize the verification crypto engine with an empty context." << std::endl;
            throw g_verification_engine_initialization_failure;
        }

        // file utils
        FileUtils file_utils_(context);

        // Create the standard keys folder
        // IMPORTANT DO NOT REMOVE !!!!!
        // THE FOLDER CREATION CAN FAIL !!!!
        if (!file_utils_.FolderExists()) {
            std::cout << "Could not access the folder where to load the keys from." << std::endl;
            throw g_verification_engine_initialization_failure;
        }

        // try to load the PUBLIC key
        if(file_utils_.KeyExists(kPublic)) {

            public_key_ = file_utils_.ReadKey(kPublic);
            // check the size of the key which should match the predefined one
            if (public_key_.size() != KEY_SIZE) {
                std::cout << "The loaded public key is empty or too short." << std::endl;
                throw g_verification_engine_initialization_failure;
            }

        } else {
            std::cout << "The public key for the context " << context << " does not exist." << std::endl;
            throw g_verification_engine_initialization_failure;
        }

        // try to load the PUBLIC SIGNING
        if(file_utils_.KeyExists(kPublicSign)) {

            sign_public_key_ = file_utils_.ReadKey(kPublicSign);
            // check the size of the key which should match the predefined one
            if (public_key_.size() != KEY_SIZE) {
                std::cout << "The loaded signing public key is empty or too short for the context " << context << std::endl;
                throw g_verification_engine_initialization_failure;
            }

        } else {
            std::cout << "The signing public key for the context " << context << " does not exist." << std::endl;
            throw g_verification_engine_initialization_failure;
        }

    } catch (std::exception &e) {
        std::cout << e.what() << std::endl;
        throw g_verification_engine_initialization_failure;
    }
}

VerificationEngine::VerificationEngine(const std::string &public_key, const std::string &signing_public_key) {
    if (public_key.empty()) {
        std::cout << "Cannot initialize the verification crypto engine with an empty public key." << std::endl;
        throw g_verification_engine_initialization_failure;
    }

    if (signing_public_key.empty()) {
        std::cout << "Cannot initialize the verification crypto engine with an empty signing public key." << std::endl;
        throw g_verification_engine_initialization_failure;
    }

    public_key_ = public_key;
    sign_public_key_ = signing_public_key;
}