#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <string>
#include <array>
#include <algorithm>
#include <fstream>
#include "temblor/private/filesystem.hpp"
#include "temblor/dataReaders/segy/segy2.hpp"
#include "temblor/dataReaders/segy/binaryFileHeader.hpp"


using namespace Temblor::DataReaders::SEGY; 

/// ASCII to EBCDIC header
static void convertToEBCDICHeader(const char asciiHeader[3200],
                                  char ebcdicHeader[3200])
{
   /*
    * Copyright 2000-2016 The OpenSSL Project Authors. All Rights Reserved.
    *
    * Licensed under the Apache License 2.0 (the "License").  You may not use
    * this file except in compliance with the License.  You can obtain a copy
    * in the file LICENSE in the source distribution or at
    * https://www.openssl.org/source/license.html
    *
    * The US-ASCII to EBCDIC (character set IBM-1047) table: This table is
    * bijective (no ambiguous or duplicate characters)
    *
    * Changes: I have extracted this structure from 
    * https://github.com/openssl/openssl/blob/master/crypto/ebcdic.c
    * and embedded it here.  There are no modifications to os_toebcdic.
    * Ben Baker - 2019.
    */
    const unsigned char os_toebcdic[256] = {
    0x00, 0x01, 0x02, 0x03, 0x37, 0x2d, 0x2e, 0x2f, /* 00-0f: */
    0x16, 0x05, 0x15, 0x0b, 0x0c, 0x0d, 0x0e, 0x0f, /* ................ */
    0x10, 0x11, 0x12, 0x13, 0x3c, 0x3d, 0x32, 0x26, /* 10-1f: */
    0x18, 0x19, 0x3f, 0x27, 0x1c, 0x1d, 0x1e, 0x1f, /* ................ */
    0x40, 0x5a, 0x7f, 0x7b, 0x5b, 0x6c, 0x50, 0x7d, /* 20-2f: */
    0x4d, 0x5d, 0x5c, 0x4e, 0x6b, 0x60, 0x4b, 0x61, /* !"#$%&'()*+,-./ */
    0xf0, 0xf1, 0xf2, 0xf3, 0xf4, 0xf5, 0xf6, 0xf7, /* 30-3f: */
    0xf8, 0xf9, 0x7a, 0x5e, 0x4c, 0x7e, 0x6e, 0x6f, /* 0123456789:;<=>? */
    0x7c, 0xc1, 0xc2, 0xc3, 0xc4, 0xc5, 0xc6, 0xc7, /* 40-4f: */
    0xc8, 0xc9, 0xd1, 0xd2, 0xd3, 0xd4, 0xd5, 0xd6, /* @ABCDEFGHIJKLMNO */
    0xd7, 0xd8, 0xd9, 0xe2, 0xe3, 0xe4, 0xe5, 0xe6, /* 50-5f: */
    0xe7, 0xe8, 0xe9, 0xad, 0xe0, 0xbd, 0x5f, 0x6d, /* PQRSTUVWXYZ[\]^_ */
    0x79, 0x81, 0x82, 0x83, 0x84, 0x85, 0x86, 0x87, /* 60-6f: */
    0x88, 0x89, 0x91, 0x92, 0x93, 0x94, 0x95, 0x96, /* `abcdefghijklmno */
    0x97, 0x98, 0x99, 0xa2, 0xa3, 0xa4, 0xa5, 0xa6, /* 70-7f: */
    0xa7, 0xa8, 0xa9, 0xc0, 0x4f, 0xd0, 0xa1, 0x07, /* pqrstuvwxyz{|}~. */
    0x20, 0x21, 0x22, 0x23, 0x24, 0x04, 0x06, 0x08, /* 80-8f: */
    0x28, 0x29, 0x2a, 0x2b, 0x2c, 0x09, 0x0a, 0x14, /* ................ */
    0x30, 0x31, 0x25, 0x33, 0x34, 0x35, 0x36, 0x17, /* 90-9f: */
    0x38, 0x39, 0x3a, 0x3b, 0x1a, 0x1b, 0x3e, 0xff, /* ................ */
    0x41, 0xaa, 0x4a, 0xb1, 0x9f, 0xb2, 0x6a, 0xb5, /* a0-af: */
    0xbb, 0xb4, 0x9a, 0x8a, 0xb0, 0xca, 0xaf, 0xbc, /* ................ */
    0x90, 0x8f, 0xea, 0xfa, 0xbe, 0xa0, 0xb6, 0xb3, /* b0-bf: */
    0x9d, 0xda, 0x9b, 0x8b, 0xb7, 0xb8, 0xb9, 0xab, /* ................ */
    0x64, 0x65, 0x62, 0x66, 0x63, 0x67, 0x9e, 0x68, /* c0-cf: */
    0x74, 0x71, 0x72, 0x73, 0x78, 0x75, 0x76, 0x77, /* ................ */
    0xac, 0x69, 0xed, 0xee, 0xeb, 0xef, 0xec, 0xbf, /* d0-df: */
    0x80, 0xfd, 0xfe, 0xfb, 0xfc, 0xba, 0xae, 0x59, /* ................ */
    0x44, 0x45, 0x42, 0x46, 0x43, 0x47, 0x9c, 0x48, /* e0-ef: */
    0x54, 0x51, 0x52, 0x53, 0x58, 0x55, 0x56, 0x57, /* ................ */
    0x8c, 0x49, 0xcd, 0xce, 0xcb, 0xcf, 0xcc, 0xe1, /* f0-ff: */
    0x70, 0xdd, 0xde, 0xdb, 0xdc, 0x8d, 0x8e, 0xdf /* ................ */
    };
    /*
    // This is from https://people.cs.umu.se/isak/Snippets/a2e.c
    static unsigned char a2e[256] = { 
          0,  1,  2,  3, 55, 45, 46, 47, 22,  5, 37, 11, 12, 13, 14, 15, 
         16, 17, 18, 19, 60, 61, 50, 38, 24, 25, 63, 39, 28, 29, 30, 31, 
         64, 79,127,123, 91,108, 80,125, 77, 93, 92, 78,107, 96, 75, 97, 
        240,241,242,243,244,245,246,247,248,249,122, 94, 76,126,110,111,
        124,193,194,195,196,197,198,199,200,201,209,210,211,212,213,214,
        215,216,217,226,227,228,229,230,231,232,233, 74,224, 90, 95,109,
        121,129,130,131,132,133,134,135,136,137,145,146,147,148,149,150,
        151,152,153,162,163,164,165,166,167,168,169,192,106,208,161,  7,
         32, 33, 34, 35, 36, 21,  6, 23, 40, 41, 42, 43, 44,  9, 10, 27,
         48, 49, 26, 51, 52, 53, 54,  8, 56, 57, 58, 59,  4, 20, 62,225,
         65, 66, 67, 68, 69, 70, 71, 72, 73, 81, 82, 83, 84, 85, 86, 87,
         88, 89, 98, 99,100,101,102,103,104,105,112,113,114,115,116,117,
        118,119,120,128,138,139,140,141,142,143,144,154,155,156,157,158,
        159,160,170,171,172,173,174,175,176,177,178,179,180,181,182,183,
        184,185,186,187,188,189,190,191,202,203,204,205,206,207,218,219,
        220,221,222,223,234,235,236,237,238,239,250,251,252,253,254,255
    };
    */
    #pragma omp simd
    for (auto i=0; i<3200; ++i)
    {
        auto asciiChar = static_cast<unsigned char> (asciiHeader[i]);
        // ebcdicHeader[i] = a2e[asciiChar];
        ebcdicHeader[i] = os_toebcdic[asciiChar]; //a2e[asciiChar];
    }
}
/// EBCDIC to ASCII header
static void convertToASCIIHeader(const char ebcdicHeader[3200],
                                 char asciiHeader[3200])
{
   /*  
    * Copyright 2000-2016 The OpenSSL Project Authors. All Rights Reserved.
    *
    * Licensed under the Apache License 2.0 (the "License").  You may not use
    * this file except in compliance with the License.  You can obtain a copy
    * in the file LICENSE in the source distribution or at
    * https://www.openssl.org/source/license.html
    *
    * This code does basic character mapping for IBM's TPF and OS/390 operating
    * systems. It is a modified version of the BS2000 table.
    *
    * Bijective EBCDIC (character set IBM-1047) to US-ASCII table: This table is
    * bijective - there are no ambiguous or duplicate characters.
    *
    * Changes: I have extracted this structure from 
    * https://github.com/openssl/openssl/blob/master/crypto/ebcdic.c
    * and embedded it here.  There are no modifications to os_toascii.
    * Ben Baker - 2019.
    */
    const unsigned char os_toascii[256] = {
    0x00, 0x01, 0x02, 0x03, 0x85, 0x09, 0x86, 0x7f, /* 00-0f: */
    0x87, 0x8d, 0x8e, 0x0b, 0x0c, 0x0d, 0x0e, 0x0f, /* ................ */
    0x10, 0x11, 0x12, 0x13, 0x8f, 0x0a, 0x08, 0x97, /* 10-1f: */
    0x18, 0x19, 0x9c, 0x9d, 0x1c, 0x1d, 0x1e, 0x1f, /* ................ */
    0x80, 0x81, 0x82, 0x83, 0x84, 0x92, 0x17, 0x1b, /* 20-2f: */
    0x88, 0x89, 0x8a, 0x8b, 0x8c, 0x05, 0x06, 0x07, /* ................ */
    0x90, 0x91, 0x16, 0x93, 0x94, 0x95, 0x96, 0x04, /* 30-3f: */
    0x98, 0x99, 0x9a, 0x9b, 0x14, 0x15, 0x9e, 0x1a, /* ................ */
    0x20, 0xa0, 0xe2, 0xe4, 0xe0, 0xe1, 0xe3, 0xe5, /* 40-4f: */
    0xe7, 0xf1, 0xa2, 0x2e, 0x3c, 0x28, 0x2b, 0x7c, /* ...........<(+| */
    0x26, 0xe9, 0xea, 0xeb, 0xe8, 0xed, 0xee, 0xef, /* 50-5f: */
    0xec, 0xdf, 0x21, 0x24, 0x2a, 0x29, 0x3b, 0x5e, /* &.........!$*);^ */
    0x2d, 0x2f, 0xc2, 0xc4, 0xc0, 0xc1, 0xc3, 0xc5, /* 60-6f: */
    0xc7, 0xd1, 0xa6, 0x2c, 0x25, 0x5f, 0x3e, 0x3f, /* -/.........,%_>? */
    0xf8, 0xc9, 0xca, 0xcb, 0xc8, 0xcd, 0xce, 0xcf, /* 70-7f: */
    0xcc, 0x60, 0x3a, 0x23, 0x40, 0x27, 0x3d, 0x22, /* .........`:#@'=" */
    0xd8, 0x61, 0x62, 0x63, 0x64, 0x65, 0x66, 0x67, /* 80-8f: */
    0x68, 0x69, 0xab, 0xbb, 0xf0, 0xfd, 0xfe, 0xb1, /* .abcdefghi...... */
    0xb0, 0x6a, 0x6b, 0x6c, 0x6d, 0x6e, 0x6f, 0x70, /* 90-9f: */
    0x71, 0x72, 0xaa, 0xba, 0xe6, 0xb8, 0xc6, 0xa4, /* .jklmnopqr...... */
    0xb5, 0x7e, 0x73, 0x74, 0x75, 0x76, 0x77, 0x78, /* a0-af: */
    0x79, 0x7a, 0xa1, 0xbf, 0xd0, 0x5b, 0xde, 0xae, /* .~stuvwxyz...[.. */
    0xac, 0xa3, 0xa5, 0xb7, 0xa9, 0xa7, 0xb6, 0xbc, /* b0-bf: */
    0xbd, 0xbe, 0xdd, 0xa8, 0xaf, 0x5d, 0xb4, 0xd7, /* .............].. */
    0x7b, 0x41, 0x42, 0x43, 0x44, 0x45, 0x46, 0x47, /* c0-cf: */
    0x48, 0x49, 0xad, 0xf4, 0xf6, 0xf2, 0xf3, 0xf5, /* {ABCDEFGHI...... */
    0x7d, 0x4a, 0x4b, 0x4c, 0x4d, 0x4e, 0x4f, 0x50, /* d0-df: */
    0x51, 0x52, 0xb9, 0xfb, 0xfc, 0xf9, 0xfa, 0xff, /* }JKLMNOPQR...... */
    0x5c, 0xf7, 0x53, 0x54, 0x55, 0x56, 0x57, 0x58, /* e0-ef: */
    0x59, 0x5a, 0xb2, 0xd4, 0xd6, 0xd2, 0xd3, 0xd5, /* \.STUVWXYZ...... */
    0x30, 0x31, 0x32, 0x33, 0x34, 0x35, 0x36, 0x37, /* f0-ff: */
    0x38, 0x39, 0xb3, 0xdb, 0xdc, 0xd9, 0xda, 0x9f /* 0123456789...... */
    };
    /*
    // This is from https://people.cs.umu.se/isak/Snippets/a2e.c
    static unsigned char e2a[256] = { 
          0,  1,  2,  3,156,  9,134,127,151,141,142, 11, 12, 13, 14, 15, 
         16, 17, 18, 19,157,133,  8,135, 24, 25,146,143, 28, 29, 30, 31, 
        128,129,130,131,132, 10, 23, 27,136,137,138,139,140,  5,  6,  7,  
        144,145, 22,147,148,149,150,  4,152,153,154,155, 20, 21,158, 26, 
         32,160,161,162,163,164,165,166,167,168, 91, 46, 60, 40, 43, 33, 
         38,169,170,171,172,173,174,175,176,177, 93, 36, 42, 41, 59, 94, 
         45, 47,178,179,180,181,182,183,184,185,124, 44, 37, 95, 62, 63, 
        186,187,188,189,190,191,192,193,194, 96, 58, 35, 64, 39, 61, 34, 
        195, 97, 98, 99,100,101,102,103,104,105,196,197,198,199,200,201,
        202,106,107,108,109,110,111,112,113,114,203,204,205,206,207,208,
        209,126,115,116,117,118,119,120,121,122,210,211,212,213,214,215,
        216,217,218,219,220,221,222,223,224,225,226,227,228,229,230,231,
        123, 65, 66, 67, 68, 69, 70, 71, 72, 73,232,233,234,235,236,237,
        125, 74, 75, 76, 77, 78, 79, 80, 81, 82,238,239,240,241,242,243,
         92,159, 83, 84, 85, 86, 87, 88, 89, 90,244,245,246,247,248,249,
         48, 49, 50, 51, 52, 53, 54, 55, 56, 57,250,251,252,253,254,255
    };
    */
    #pragma omp simd
    for (auto i=0; i<3200; ++i)
    {
        auto ebcdicChar = static_cast<unsigned char> (ebcdicHeader[i]);
        // asciiHeader[i] = e2a[ebcdicChar];
        asciiHeader[i] = os_toascii[ebcdicChar];
    }
}

/// Pointer to implementation
class Segy2::Segy2Impl
{
public:
    Segy2Impl()
    {
        std::memset(mTextualHeader.data(), ' ', 3200*sizeof(char));
    }
    ~Segy2Impl() = default;

//private:
    /// 3200 byte EBCDIC text header (stored locally in ASCII)
    std::array<char, 3200> mTextualHeader;
    /// 400 byte binary file header
    BinaryFileHeader mBinaryFileHeader{2, 0};  // Default version and revision
};

/// Default constructor
Segy2::Segy2() :
    pImpl(std::make_unique<Segy2Impl> ())
{
}

/// Default destructor
Segy2::~Segy2() = default;

void Segy2::clear() noexcept
{
     std::memset(pImpl->mTextualHeader.data(), ' ', 3200*sizeof(char));
}

void Segy2::read(const std::string &fileName)
{
    clear();
#if TEMBLOR_USE_FILESYSTEM == 1
    if (!fs::exists(fileName))
    {
        std::string errmsg = "SEGY-2 file = " + fileName + " does not exist";
        throw std::invalid_argument(errmsg);
    }
#endif
    // Read the binary file
    std::ifstream segyfl(fileName, std::ios::binary);
    std::vector<char> buffer(std::istreambuf_iterator<char> (segyfl), {});
    segyfl.close();
    size_t nbytes = buffer.size();
    if (nbytes < 3600)
    {
        std::string errmsg = "SEGY file must have length of at least 3200";
        throw std::invalid_argument(errmsg);
    }
    convertToASCIIHeader(buffer.data(), pImpl->mTextualHeader.data());
//printf("%s\n", pImpl->mTextualHeader.data());
}

/*
void Segy2::setTextualHeader(const std::string &textualHeader)
{

}
*/

std::string Segy2::getTextualHeader() const noexcept
{
    std::string result(pImpl->mTextualHeader.data(), 3200);
    return result;
}
