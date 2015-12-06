//
// Created by silenteh on 31/10/15.
//

#include "cryptoengine.h"
#include "cryptoengine_message.h"
#include <iostream>
#include "conversions.h"
#include <cassert>

#define MIN_MESSAGE_DATA_LEN 9

class MessageInitializationFailure: public std::exception
{
    virtual const char* what() const throw()
    {
        return "Could not initialize the message properly!";
    }
} g_message_initialization_failure;


// Constructor
Message::Message(const std::string &message, const int32_t message_type){

    version_ = TCP_MESSAGE_VERSION;

    if(message.empty()) {
        std::cout << "Message cannot be empty" << std::endl;
        throw g_message_initialization_failure;
    }

    // mame sure the message is >=0 and < MAX_INT32
    if(message_type < 0 || message_type > MAX_INT32) {
        type_ = 0;
    } else {
        type_ = message_type;
    }

    length_ = 0;
    length_ += INT_SIZE;        // 4 bytes of message version
    length_ += INT_SIZE;        // 4 bytes of message type
    length_ += message.size();  // Message size

    message_= message;
}

// converts the message to a byte array
std::vector<unsigned char> Message::ToBytes() const {

    if (length_ <= 0) {
        std::cout << "Cannot serialize the message to bytes. Total message length is not valid" << std::endl;
        throw g_message_initialization_failure;
    }

    // allocate the exact value of char we will store in the vector, so no expansion is needed
    std::vector<unsigned char> data(length_,0);

    size_t offset = 0;

    // ===================================================================
    // convert the version
    std::array<unsigned char, INT_SIZE> version_bytes = Conversions::SmallEndianToInt(version_);
    // add it to the data array
    for(int i = 0; i < INT_SIZE; ++i) {
        data.at(offset + i) = version_bytes[i];
    }

    // ===================================================================

    offset += INT_SIZE;

    // convert the type
    std::array<unsigned char, INT_SIZE> type_bytes = Conversions::SmallEndianToInt(type_);
    // add it to the data array
    for(int i = 0; i < INT_SIZE; ++i) {
        data.at(offset + i) = type_bytes[i];
    }

    // ===================================================================

    offset += INT_SIZE;

    // add the message bytes
    for(int i = 0; i < message_.size(); ++i) {
        data.at(offset + i) = static_cast<const unsigned char>(message_[i]);
    }

    return data;
}

// Convert the vector of bytes to a message
// The bytes here are made of: |4 version| 4 type | clear text
Message Message::FromBytesString(const std::string &data_string) {

    std::vector<unsigned char> data(data_string.begin(), data_string.end());

    // make sure the vector is no empty
    if (data.empty() || data.size() < MIN_MESSAGE_DATA_LEN) {
        std::cout << "Cannot create a message from an empty vector" << std::endl;
        throw g_message_initialization_failure;
    }

    // Initial offset
    size_t offset =  0;

    //==============================================================
    // convert the message version
    // Extract the data (slice the vector)
    std::array<unsigned char, INT_SIZE> version_array;
    for(int i = 0; i < INT_SIZE; ++i) {
        version_array[i] = data.at(i);
    }

    int32_t message_version = Conversions::SmallEndianFromInt(version_array);

    // VERIFY THE MESSAGE VERSION !
    if (message_version != TCP_MESSAGE_VERSION) {
        std::cout << "Error parsing the message from bytes. Version mismatch." << std::endl;
        throw g_message_initialization_failure;
    }

    offset += INT_SIZE;

    //==============================================================
    // convert the message type
    std::array<unsigned char, INT_SIZE> type_array;
    for(int i = 0; i < INT_SIZE; ++i) {
        type_array[i] = data.at(i+offset);
    }

    int32_t message_type = Conversions::SmallEndianFromInt(type_array);

    offset += INT_SIZE;

    //==============================================================
    // read the message
    uint64_t message_length = data.size() - offset;
    // Extract the data (slice the vector)
    std::vector<unsigned char> message_vector(data.begin() + offset, data.end());

    // cast the vector
    const char * message_bytes(reinterpret_cast<const char *>(message_vector.data()));

    // create the message string
    std::string message(message_bytes, message_length);

    // =============================================================

    // build the message
    Message m(message, message_type);

    return m;

}

std::string Message::ToBytesString() const {

    std::vector<unsigned char> bytes = this->ToBytes();

    return std::string(bytes.begin(), bytes.end());
}

