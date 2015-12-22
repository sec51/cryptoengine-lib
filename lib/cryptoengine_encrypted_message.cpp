//
// Created by silenteh on 20/11/15.
//

#include "cryptoengine_encrypted_message.h"
#include "cryptoengine.h"
#include "cryptoengine_conversions.h"
#include <iostream>
#include <cassert>

class EncryptedMessageInitializationFailure: public std::exception
{
    virtual const char* what() const throw()
    {
        return "Could not initialize the encrypted message properly!";
    }
} g_encrypted_message_initialization_failure;

// constructor
EncryptedMessage::EncryptedMessage(const std::string &nonce, const std::string &encrypted_message) {
    if (nonce.size() == 0) {
        std::cout << "Nonce cannot be empty" << std::endl;
        throw g_encrypted_message_initialization_failure;
    }

    if(encrypted_message.size() == 0) {
        std::cout << "Message cannot be empty" << std::endl;
        throw g_encrypted_message_initialization_failure;
    }

    // also makes sure it foes not overflow

    length_ = 0;
    length_ +=  UINT64_SIZE;     // N bytes of message total length
    length_ += nonce.size();    // Nonce size
    length_ += encrypted_message.size();  // Message size

    // assign the nonce
    nonce_ = nonce;

    // assign the message
    message_= encrypted_message;
}


// converts the message to a byte array
std::vector<unsigned char> EncryptedMessage::ToBytes() const{

    if (length_ <= 0) {
        std::cout << "Cannot serialize the message to bytes. Total message length is not valid" << std::endl;
        throw g_encrypted_message_initialization_failure;
    }

    // allocate the exact value of char we will store in the vector, so no expansion is needed
    std::vector<unsigned char> data(length_, 0);

    // convert the total size
    std::array<unsigned char, UINT64_SIZE> length_bytes = Conversions::SmallEndianToUint64(length_);

    // add it to the data array
    for(int i = 0; i < UINT64_SIZE; ++i) {
        data.at(i) = length_bytes[i];
    }

    size_t offset = UINT64_SIZE;

    // ====================================================================

    // store the nonce
    for(int i = 0; i < nonce_.size(); ++i) {
        data.at(offset + i) = static_cast<const unsigned char>(nonce_[i]);
    }

    // ====================================================================

    offset += nonce_.size();

    for(int i = 0; i < message_.size(); ++i) {
        data.at(offset + i) = static_cast<const unsigned char>(message_[i]);
    }

    // ====================================================================

    return data;
}


// This message assumes the bytes passed have been decrypted already
EncryptedMessage EncryptedMessage::FromBytes(const std::vector<unsigned char> &data) {

    // make sure the vector is no empty
    if (data.empty()) {
        std::cout << "Cannot create a message from an empty vector" << std::endl;
        throw g_encrypted_message_initialization_failure;
    }

    // convert the total message size
    std::array<unsigned char, UINT64_SIZE> total_length_array;
    for (int i = 0; i < UINT64_SIZE; ++i) {
        total_length_array[i] = data.at(i);
    }

    uint64_t total_length = Conversions::SmallEndianFromUint(total_length_array);

    if (total_length <= 0 || total_length > MAX_UINT64) {
        std::cout << "Error parsing total message length" << std::endl;
        throw g_encrypted_message_initialization_failure;
    }

    // Extract the nonce, by defining the offset
    size_t offset =  UINT64_SIZE;       // N bytes of message total length

    // extract the nonce
    std::array<char, NONCE_SIZE> nonce_bytes;
    for(int i = 0; i < NONCE_SIZE; ++i) {
        nonce_bytes[i] = static_cast<char>(data.at(offset + i));
    }

    // create the nonce string
    std::string nonce(nonce_bytes.data(), NONCE_SIZE);

    // adjust the offset
    offset += NONCE_SIZE;

    // calculate the message length
    size_t message_length = total_length - offset;

    // Extract the data (slice the vector)
    std::vector<unsigned char> message_sliced_vector(data.begin() + offset, data.end());

    // cast the vector
    const char * message_bytes(reinterpret_cast<const char *>(message_sliced_vector.data()));

    // create the message string
    std::string message(message_bytes, message_length);

    EncryptedMessage m(nonce, message);

    assert(m.length() == total_length);
    assert(m.nonce() == nonce);
    assert(m.message() == message);

    return m;

}