//
// Created by silenteh on 31/10/15.
//

#ifndef CRYPTOENGINE_CONVERSIONS_H
#define CRYPTOENGINE_CONVERSIONS_H

#include <string>
#include <array>
#include <limits>

#define INT_SIZE 4
#define UINT64_SIZE 8
#define MAX_UINT64 std::numeric_limits<uint64_t >::max()
#define MAX_INT32 std::numeric_limits<int32_t >::max()

class Conversions {

public:
    static std::array<unsigned char,INT_SIZE> BigEndianToInt(int32_t num);
    static int32_t BigEndianFromInt(std::array<unsigned char,INT_SIZE> data);

    static std::array<unsigned char,UINT64_SIZE> BigEndianToUint64(uint64_t num);
    static uint64_t BigEndianFromUint(std::array<unsigned char,UINT64_SIZE> data);


    static std::array<unsigned char,INT_SIZE> SmallEndianToInt(int32_t num);
    static int32_t SmallEndianFromInt(std::array<unsigned char,INT_SIZE> data);

    static std::array<unsigned char,UINT64_SIZE> SmallEndianToUint64(uint64_t num);
    static uint64_t SmallEndianFromUint(std::array<unsigned char,UINT64_SIZE> data);

};


#endif //CRYPTOENGINE_CONVERSIONS_H
