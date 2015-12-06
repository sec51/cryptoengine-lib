//
// Created by silenteh on 11/10/15.
//

#ifndef PROJECT_HKDF_H
#define PROJECT_HKDF_H

#include <array>
#include <string>
#include <atomic>


class HKDFUtils {

private:
    std::atomic_uint_fast64_t counter_;
    std::string context_;

public:
    HKDFUtils(std::string ctx);
    std::string DeriveNonce(const std::string &masterKey, const std::string &salt);
};


#endif //PROJECT_HKDF_H
