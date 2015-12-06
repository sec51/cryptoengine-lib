//
// Created by silenteh on 25/10/15.
//

#ifndef CRYPTOENGINE_BASE64_H
#define CRYPTOENGINE_BASE64_H

#include <string>

class Base64 {

public:

    std::string Encode(const std::string &data);
    std::string Decode(const std::string &data, size_t size);

private:
    unsigned long Base64decode(char *bufplain, const char *bufcoded);

    unsigned long Base64encode(char *encoded, const char *string, unsigned long len);

    unsigned long Base64encode_len(unsigned long len);

    unsigned long Base64decode_len(const char *bufcoded);

};


#endif //CRYPTOENGINE_BASE64_H
