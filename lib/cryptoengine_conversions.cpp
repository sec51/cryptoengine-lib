//
// Created by silenteh on 31/10/15.
//

#include "cryptoengine_conversions.h"
#include <cassert>
#include <limits>


// BIG ENDIAN
std::array<unsigned char,INT_SIZE> Conversions::BigEndianToInt(int32_t num){

     assert(num <= std::numeric_limits<int32_t >::max());
     assert(num >= std::numeric_limits<int32_t >::min());

     std::array<unsigned char,INT_SIZE> data;
     data[0] = (unsigned char)(((num >> 24) & 0xFF));
     data[1] = (unsigned char)((num >> 16) & 0xFF);
     data[2] = (unsigned char)((num >> 8) & 0xFF);
     data[3] = (unsigned char)(num & 0xFF);
     return data;
 }

int32_t Conversions::BigEndianFromInt(std::array<unsigned char,INT_SIZE> data) {
    int32_t i = ((data[3]) << 0) | ((data[2]) << 8) | ((data[1]) << 16) | ((data[0]) << 24);
    return i;
}

std::array<unsigned char,UINT64_SIZE> Conversions::BigEndianToUint64(uint64_t num){

    assert(num <= std::numeric_limits<uint64_t >::max());
    assert(num >= std::numeric_limits<uint64_t >::min());

    std::array<unsigned char,UINT64_SIZE> data;

    data[0] = (unsigned char)((num >> 56) & 0xFF);
    data[1] = (unsigned char)((num >> 48) & 0xFF);
    data[2] = (unsigned char)((num >> 40) & 0xFF);
    data[3] = (unsigned char)((num >> 32) & 0xFF);
    data[4] = (unsigned char)((num >> 24) & 0xFF);
    data[5] = (unsigned char)((num >> 16) & 0xFF);
    data[6] = (unsigned char)((num >> 8) & 0xFF);
    data[7] = (unsigned char)(num & 0xFF);
    return data;
}

uint64_t Conversions::BigEndianFromUint(std::array<unsigned char,UINT64_SIZE> data) {
    uint64_t i = ((data[7]) << 0) | ((data[6]) << 8) |
                 ((data[5]) << 16) | ((data[4]) << 24) |
            (uint64_t(data[3]) << 32) | (uint64_t(data[2]) << 40) |
            (uint64_t(data[1]) << 48) | (uint64_t(data[0]) << 56);
    return i;
}

// ## ==========================================================================================
// SMALL ENDIAN
std::array<unsigned char,INT_SIZE> Conversions::SmallEndianToInt(int32_t num){

    assert(num <= std::numeric_limits<int32_t >::max());
    assert(num >= std::numeric_limits<int32_t >::min());

    std::array<unsigned char,INT_SIZE> data;
    data[3] = (unsigned char)(((num >> 24) & 0xFF));
    data[2] = (unsigned char)((num >> 16) & 0xFF);
    data[1] = (unsigned char)((num >> 8) & 0xFF);
    data[0] = (unsigned char)(num & 0xFF);
    return data;
}

int32_t Conversions::SmallEndianFromInt(std::array<unsigned char,INT_SIZE> data) {
    int32_t i = ((data[0]) << 0) | ((data[1]) << 8) | ((data[2]) << 16) | ((data[3]) << 24);
    return i;
}

std::array<unsigned char,UINT64_SIZE> Conversions::SmallEndianToUint64(uint64_t num){

    assert(num <= std::numeric_limits<uint64_t >::max());
    assert(num >= std::numeric_limits<uint64_t >::min());

    std::array<unsigned char,UINT64_SIZE> data;
    data[7] = (unsigned char)((num >> 56) & 0xFF);
    data[6] = (unsigned char)((num >> 48) & 0xFF);
    data[5] = (unsigned char)((num >> 40) & 0xFF);
    data[4] = (unsigned char)((num >> 32) & 0xFF);
    data[3] = (unsigned char)((num >> 24) & 0xFF);
    data[2] = (unsigned char)((num >> 16) & 0xFF);
    data[1] = (unsigned char)((num >> 8) & 0xFF);
    data[0] = (unsigned char)(num & 0xFF);
    return data;
}

uint64_t Conversions::SmallEndianFromUint(std::array<unsigned char,UINT64_SIZE> data) {
    uint64_t i = (((data[0]) << 0) | ((data[1]) << 8) |
                 ((data[2]) << 16) | ((data[3]) << 24) |
                 (uint64_t(data[4]) << 32) | (uint64_t(data[5]) << 40) |
                 (uint64_t(data[6]) << 48) | (uint64_t(data[7]) << 56));
    return i;
}