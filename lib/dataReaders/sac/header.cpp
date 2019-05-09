#include <cstdio>
#include <cstdlib>
#include <cassert>
#include <string>
#include "temblor/library/dataReaders/sac/sac.hpp"

#define NULL_DOUBLE -12345
#define NULL_INT -12345
#define NULL_STRING "-12345"

using namespace Temblor::Library::DataReaders::SAC;

class Header::HeaderImpl
{
public:
    //HeaderImpl() = default;
    //HeaderImpl(const HeaderImpl &header) = default; 
    //HeaderImpl& operator=HeaderImpl() = default;
    //~HeaderImpl() = default;

    int leven   = NULL_INT;
    int lpspol  = NULL_INT;
    int lovrok  = NULL_INT;
    int lcalda  = NULL_INT;
    int lunused = NULL_INT;
    // Character
    std::string kstnm{NULL_STRING,  8};
    std::string kevnm{NULL_STRING, 16};
    std::string khole{NULL_STRING, 8};
    std::string ko{NULL_STRING, 8};
    std::string ka{NULL_STRING, 8};
    std::string kt0{NULL_STRING, 8};
    std::string kt1{NULL_STRING, 8};
    std::string kt2{NULL_STRING, 8};
    std::string kt3{NULL_STRING, 8};
    std::string kt4{NULL_STRING, 8};
    std::string kt5{NULL_STRING, 8};
    std::string kt6{NULL_STRING, 8}; 
    std::string kt7{NULL_STRING, 8}; 
    std::string kt8{NULL_STRING, 8}; 
    std::string kt9{NULL_STRING, 8}; 
    std::string kf{NULL_STRING, 8};
    std::string kuser0{NULL_STRING, 8};
    std::string kuser1{NULL_STRING, 8};
    std::string kuser2{NULL_STRING, 8};
    std::string kcmpnm{NULL_STRING, 8};
    std::string knetwk{NULL_STRING, 8};
    std::string kdatrd{NULL_STRING, 8};
    std::string kinst{NULL_STRING, 8};
};

Header::Header() :
    pImpl(std::make_unique<HeaderImpl> ())
{
}

Header::Header(const Header &header)
{
    *this = header;
}

Header::Header(Header &&header) noexcept
{
    *this = std::move(header);
}

Header& Header::operator=(const Header &header)
{
    if (&header == this){return *this;}
    if (pImpl){pImpl.reset();}
    pImpl = std::make_unique<HeaderImpl> (*header.pImpl);
    return *this;
}

Header& Header::operator=(Header &&header) noexcept
{
    if (&header == this){return *this;}
    if (pImpl){pImpl.reset();}
    pImpl = std::move(header.pImpl);
    return *this;
}

Header::~Header() = default;

void Header::clear() noexcept
{
    Header header; // Create a default header
    *this = std::move(header); // Move it to this header to avoid copying
}

std::string Header::getHeader(const Character variableName) noexcept
{
    if (variableName == Character::KSTNM)
    {
        return pImpl->kstnm;
    }
    else if (variableName == Character::KEVNM)
    {
        return pImpl->kevnm;
    }
    else if (variableName == Character::KHOLE)
    {
        return pImpl->khole;
    }
    else if (variableName == Character::KO)
    {
        return pImpl->ko;
    }
    else if (variableName == Character::KA)
    {
        return pImpl->ka;
    }
    else if (variableName == Character::KT0)
    {
        return pImpl->kt0;
    }
    else if (variableName == Character::KT1)
    {   
        return pImpl->kt1;
    }
    else if (variableName == Character::KT2)
    {
        return pImpl->kt2;
    }
    else if (variableName == Character::KT3)
    {
        return pImpl->kt3;
    }
    else if (variableName == Character::KT4)
    {
        return pImpl->kt4;
    }
    else if (variableName == Character::KT5)
    {
        return pImpl->kt5;
    }
    else if (variableName == Character::KT6)
    {
        return pImpl->kt6;
    }
    else if (variableName == Character::KT7)
    {
        return pImpl->kt7;
    }
    else if (variableName == Character::KT8)
    {
        return pImpl->kt8;
    }
    else if (variableName == Character::KT9)
    {
        return pImpl->kt9;
    }
    else if (variableName == Character::KF)
    {
        return pImpl->kf;
    }
    else if (variableName == Character::KUSER0)
    {
        return pImpl->kuser0;
    }
    else if (variableName == Character::KUSER1)
    {
        return pImpl->kuser1;
    }
    else if (variableName == Character::KUSER2)
    {
        return pImpl->kuser2;
    }
    else if (variableName == Character::KCMPNM)
    {
        return pImpl->kcmpnm;
    }
    else if (variableName == Character::KNETWK)
    {
        return pImpl->knetwk;
    }
    else if (variableName == Character::KDATRD)
    {
        return pImpl->kdatrd;
    }
    else // if (variableName == Character::KINST)
    {
        return pImpl->kinst;
    } 
}

