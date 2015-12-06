//
// Created by silenteh on 31/10/15.
//

#ifndef CRYPTOENGINE_MESSAGE_H
#define CRYPTOENGINE_MESSAGE_H

#include <string>
#include <vector>

class Message {

private:
    uint64_t length_;                   // length of the message
    std::string message_;               // the encrypted message
    int32_t version_;                   // version of the message, done to support backward compatibility
    int32_t type_;                      // type of message, this can be used to process the messages differently on the receiver part

    // convert the message to bytes
    // This function assumes that the message has already been encrypted
    std::vector<unsigned char> ToBytes() const;

public:
    // constructor
    Message(const std::string &message, const int32_t message_type);

    // convert the message to bytes
    // This function assumes that the message has already been encrypted
    std::string ToBytesString() const;

    // This message assumes the bytes passed have been decrypted already
    static Message FromBytesString(const std::string &data_string);

    // This function adds the version+type+message all together as a string
    // static std::string generateMessageBytes(const std::string &message, const int32_t type);

    // Accessor(s)
    // The message transported
    inline std::string message() const{
        return message_;
    }

    // the total length of the message for parsing
    inline uint64_t length() const{
        return length_;
    }

    // the Version of the message to support different formats
    inline int32_t version() const{
        return version_;
    }

    // The type of the message so the receiver can parse different types if needed
    inline int32_t type() const{
        return type_;
    }


};


#endif //CRYPTOENGINE_MESSAGE_H
