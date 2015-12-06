//
// Created by silenteh on 20/11/15.
//

#ifndef CRYPTOENGINE_ENCRYPTED_MESSAGE_H
#define CRYPTOENGINE_ENCRYPTED_MESSAGE_H

#include <string>
#include <vector>

class EncryptedMessage {

private:
    uint64_t length_;                   // length of the message
    std::string nonce_;                 // the randomly created nonce. The nonce can be public.
    std::string message_;               // the encrypted message


public:
    // constructor
    EncryptedMessage(const std::string &nonce, const std::string &encrypted_message);

    // convert the message to bytes
    // This function assumes that the message has already been encrypted
    std::vector<unsigned char> ToBytes() const;

    // This message assumes the bytes passed have been decrypted already
    static EncryptedMessage FromBytes(const std::vector<unsigned char> &data);


    // Accessor(s)
    // The encrypted message transported
    inline std::string message() const{
        return message_;
    }

    // The nonce
    inline std::string nonce() const{
        return nonce_;
    }

    // the total length of the message for parsing
    inline uint64_t length() const{
        return length_;
    }

};


#endif //CRYPTOENGINE_ENCRYPTED_MESSAGE_H
