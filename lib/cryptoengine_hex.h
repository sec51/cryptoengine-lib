//
// Created by silenteh on 19.08.16.
//

#ifndef CRYPTOENGINE_HEX_H
#define CRYPTOENGINE_HEX_H

#include <string>
#include <sstream>

constexpr char hexmap[] = {'0', '1', '2', '3', '4', '5', '6', '7',
                           '8', '9', 'a', 'b', 'c', 'd', 'e', 'f'};

class Hex {

public:
    static std::string Encode(const std::string &input);
    static std::string Decode(const std::string &hexString);

};


#endif //CRYPTOENGINE_HEX_H
