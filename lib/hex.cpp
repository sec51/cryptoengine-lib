//
// Created by silenteh on 19.08.16.
//

#include <fstream>
#include <iomanip>
#include "hex.h"

unsigned char hexval(unsigned char c)
{
    if ('0' <= c && c <= '9')
        return c - '0';
    else if ('a' <= c && c <= 'f')
        return c - 'a' + 10;
    else if ('A' <= c && c <= 'F')
        return c - 'A' + 10;
    else abort();
}

std::string Hex::Encode(const std::string &input) {

    size_t len = input.size();
    auto data = input.c_str();
    std::string s(len * 2, ' ');
    for (int i = 0; i < len; ++i) {
        s[2 * i]     = hexmap[(data[i] & 0xF0) >> 4];
        s[2 * i + 1] = hexmap[data[i] & 0x0F];
    }
    return s;
}


std::string Hex::Decode(const std::string &hexString) {

    std::string out;
    out.reserve(hexString.length() / 2);
    for (std::string::const_iterator p = hexString.begin(); p != hexString.end(); p++)
    {
        unsigned char c = hexval(*p);
        p++;
        if (p == hexString.end()) break; // incomplete last digit - should report error
        c = (c << 4) + hexval(*p); // + takes precedence over <<
        out.push_back(c);
    }
    return out;
}

