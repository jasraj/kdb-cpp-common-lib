// Base64 Encoder / Decoder for kdb+
// Copyright (c) 2020 Jaskirat Rajasansir and Contributors

// Base64 Encoder and Decoder from https://stackoverflow.com/a/41094722
//  - Encoder: Copyright (c) 2005-2011, Jouni Malinen <j@w1.fi>
//  - Decoder: Copyright (c) 2016, https://stackoverflow.com/users/5358284/polfosol-%e0%b0%a0-%e0%b0%a0

#include "kx-c-lib/c/c/k.h"

#include "include/Common.hpp"

#include <string>
#include <iostream>

#include <unistd.h>


namespace kdbbase64 {

static const int DECODE_INDEX[256] = { 0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0, 62, 63, 62, 62, 63, 52, 53, 54, 55,
56, 57, 58, 59, 60, 61,  0,  0,  0,  0,  0,  0,  0,  0,  1,  2,  3,  4,  5,  6,
7,  8,  9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25,  0,
0,  0,  0, 63,  0, 26, 27, 28, 29, 30, 31, 32, 33, 34, 35, 36, 37, 38, 39, 40,
41, 42, 43, 44, 45, 46, 47, 48, 49, 50, 51 };

static const unsigned char ENCODE_TABLE[65] =
    "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";


std::string decode(const void* data, const size_t len) {
    unsigned char* p = (unsigned char*)data;
    int pad = len > 0 && (len % 4 || p[len - 1] == '=');
    const size_t L = ((len + 3) / 4 - pad) * 4;
    std::string str(L / 4 * 3 + pad, '\0');

    for (size_t i = 0, j = 0; i < L; i += 4) {
        int n = DECODE_INDEX[p[i]] << 18 | DECODE_INDEX[p[i + 1]] << 12 | DECODE_INDEX[p[i + 2]] << 6 | DECODE_INDEX[p[i + 3]];
        str[j++] = n >> 16;
        str[j++] = n >> 8 & 0xFF;
        str[j++] = n & 0xFF;
    }

    if (pad) {
        int n = DECODE_INDEX[p[L]] << 18 | DECODE_INDEX[p[L + 1]] << 12;
        str[str.size() - 1] = n >> 16;

        if (len > L + 2 && p[L + 2] != '=')
        {
            n |= DECODE_INDEX[p[L + 2]] << 6;
            str.push_back(n >> 8 & 0xFF);
        }
    }

    return str;
}

std::string encode(const unsigned char *src, size_t len)
{
    unsigned char *out, *pos;
    const unsigned char *end, *in;

    size_t olen;

    olen = 4*((len + 2) / 3); /* 3-byte blocks to 4-byte */

    if (olen < len)
        return std::string(); /* integer overflow */

    std::string outStr;
    outStr.resize(olen);
    out = (unsigned char*)&outStr[0];

    end = src + len;
    in = src;
    pos = out;
    while (end - in >= 3) {
        *pos++ = ENCODE_TABLE[in[0] >> 2];
        *pos++ = ENCODE_TABLE[((in[0] & 0x03) << 4) | (in[1] >> 4)];
        *pos++ = ENCODE_TABLE[((in[1] & 0x0f) << 2) | (in[2] >> 6)];
        *pos++ = ENCODE_TABLE[in[2] & 0x3f];
        in += 3;
    }

    if (end - in) {
        *pos++ = ENCODE_TABLE[in[0] >> 2];
        if (end - in == 1) {
            *pos++ = ENCODE_TABLE[(in[0] & 0x03) << 4];
            *pos++ = '=';
        }
        else {
            *pos++ = ENCODE_TABLE[((in[0] & 0x03) << 4) |
                (in[1] >> 4)];
            *pos++ = ENCODE_TABLE[(in[1] & 0x0f) << 2];
        }
        *pos++ = '=';
    }

    return outStr;
}

}   // namespace kdbbase64c

extern "C" K _cpp_b64_decode(K toDecode) {
    if(toDecode->t != KC)
        return kdbcommon::error(EINVAL, "Type for Base64 decode must be string");

    std::string decodedString = kdbbase64::decode((unsigned char*) kC(toDecode), toDecode->n);

    return kpn((char*) decodedString.c_str(), decodedString.length());
}

extern "C" K _cpp_b64_encode(K toEncode) {
    if(toEncode->t != KC)
        return kdbcommon::error(EINVAL, "Type for Base64 encode must be string");

    std::string encodedString = kdbbase64::encode((unsigned char*) kC(toEncode), toEncode->n);

    return kpn((char*) encodedString.c_str(), encodedString.length());
}
