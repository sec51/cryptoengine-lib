//
// Created by silenteh on 11/10/15.
//

/* Copyright (c) 2014, Google Inc.
 *
 * Permission to use, copy, modify, and/or distribute this software for any
 * purpose with or without fee is hereby granted, provided that the above
 * copyright notice and this permission notice appear in all copies.
 *
 * THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES
 * WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF
 * MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY
 * SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES
 * WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN ACTION
 * OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF OR IN
 * CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE. */

// key derivation function specified in RFC 5869
// it uses the function HMAC(k,m), where k and m are variable length byte strings, which itself is implemented in terms
// of a Merkle-Damg˚ard hash H, such as SHA-256
// The input to HKDF consists of a secret s, an optional salt value salt (discussed below), an
// optional info field (also discussed below), and an output length parameter L. The parameters s,
// salt, and info are variable length byte strings.
// The execution of HKDF consists of two stages, called extract (which corresponds to what we
// called key derivation), and expand (which corresponds to what we called sub-key derivation).
// In the extract stage, HKDF uses salt and s to compute
//          t = HMAC(salt, s).
// Using the intermediate key t, along with info, the expand (or sub-key derivation) stage computes
// L bytes of output data, as follows:
//          q <- [L/HashLen] // HashLen is the output length (in bytes) of H
//          initialize z0 to the empty string
//          for i 1 to q do:
//              z(i) <- HMAC(t, z(i)- 1 || info || Octet(i)) // Octet(i) is a single byte whose value is i
//

#include "cryptoengine_hkdf_utils.h"
#include <openssl/err.h>
#include <openssl/crypto.h>
#include <openssl/rand.h>
#include <openssl/sha.h>
#include <openssl/hmac.h>
#include <cassert>
#include <iostream>
#include "cryptoengine_file_utils.h"
#include <string.h>
#include <assert.h>

class HKDFDeriveNonceFailure: public std::exception
{
    virtual const char* what() const throw()
    {
        return "Could not derive the nonce ! Not safe to encrypt !";
    }
} g_hkdf_nonce_derivation_failure;


// Constructor
HKDFUtils::HKDFUtils(std::string ctx):counter_(0),context_(ctx){};

int HKDF(uint8_t *out_key, size_t out_len,
         const uint8_t *secret, size_t secret_len,
         const uint8_t *salt, size_t salt_len,
         const uint8_t *info, size_t info_len) {
    /* https://tools.ietf.org/html/rfc5869#section-2.2 */

    // we are usign sha256 for our purposes
    const size_t digest_len = 32;
    uint8_t prk[32], previous[32];
    size_t n, done = 0;
    unsigned i, prk_len;
    int ret = 0;
    HMAC_CTX hmac;
    /* If salt is not given, HashLength zeros are used. However, HMAC does that
     * internally already so we can ignore it.*/
    /* Expand key material to desired length. */
    n = (out_len + digest_len - 1) / digest_len;
    if (out_len + digest_len < out_len || n > 255) {
        std::cout << "HDKF Out put too large" << std::endl;
        return 0;
    }
    HMAC_CTX_init(&hmac);
    /* Extract input keying material into pseudorandom key |prk|. */
    if (HMAC(EVP_sha256(), salt, salt_len, secret, secret_len, prk, &prk_len) == NULL) {
        goto out;
    }
    assert(prk_len == digest_len);
    if (!HMAC_Init_ex(&hmac, prk, prk_len, EVP_sha256(), NULL)) {
        goto out;
    }
    for (i = 0; i < n; i++) {
        uint8_t ctr = i + 1;
        size_t todo;
        if (i != 0 && (!HMAC_Init_ex(&hmac, NULL, 0, NULL, NULL) ||
                       !HMAC_Update(&hmac, previous, digest_len))) {
            goto out;
        }
        if (!HMAC_Update(&hmac, info, info_len) ||
            !HMAC_Update(&hmac, &ctr, 1) ||
            !HMAC_Final(&hmac, previous, NULL)) {
            goto out;
        }
        todo = digest_len;
        if (done + todo > out_len) {
            todo = out_len - done;
        }
        memcpy(out_key + done, previous, todo);
        done += todo;
    }
    ret = 1;
    out:
    HMAC_CTX_cleanup(&hmac);
    if (ret != 1) {
        //OPENSSL_PUT_ERROR(HKDF, HKDF, ERR_R_HMAC_LIB);
        std::cout << "HDKF error on the extract function" << std::endl;
    }
    return ret;
}

/* Computes HKDF (as specified by RFC 5869) of initial keying material |secret|
 * with |salt| and |info| using |digest|, and outputs |out_len| bytes to
 * |out_key|. It returns one on success and zero on error.
 *
 * HKDF is an Extract-and-Expand algorithm. It does not do any key stretching,
 * and as such, is not suited to be used alone to generate a key from a
 * password. */

// TODO: take out the SALT_KEY_NAME file creation so it can be tested
std::string HKDFUtils::DeriveNonce(const std::string &masterKey, const std::string &salt) {

    if (masterKey.size() != KEY_SIZE ) {
        std::cout << "MasterKey does not have the proper size: " << masterKey.size() << std::endl;
        throw g_hkdf_nonce_derivation_failure;
    }

    if(salt.size() != KEY_SIZE) {
        std::cout << "Salt do not have the proper size: " << salt.size() << std::endl;
        throw g_hkdf_nonce_derivation_failure;
    }

    if(context_.empty()){
        std::cout << "Contex is empty !" << std::endl;
        throw g_hkdf_nonce_derivation_failure;
    }


    // append the counter to the context with a fetch and add operation
    std::string increment = std::to_string(counter_.fetch_add(1, std::memory_order_seq_cst));
    std::string new_context = context_ + increment;

    int result = 0;
    uint8_t out_key[NONCE_SIZE];

    result = HKDF(
            out_key,
            NONCE_SIZE,
            (uint8_t *)masterKey.data(),
            masterKey.size(),
            (uint8_t *)salt.data(),
            salt.size(),
            (uint8_t *)new_context.data(),
            new_context.size()
    );

    if (1 != result) {
        std::cout << "Could not derive a nonce successfully. Stopping here."<< std::endl;
        throw g_hkdf_nonce_derivation_failure;
    }

    return std::string(reinterpret_cast<char *>(out_key), NONCE_SIZE);
}