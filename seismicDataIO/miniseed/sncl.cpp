#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <string>
#include <algorithm>
#include "temblor/dataReaders/miniseed/sncl.hpp"

using namespace Temblor::DataReaders::MiniSEED;

#define NETWORK_LENGTH 10 
#define STATION_LENGTH 10
#define CHANNEL_LENGTH 10
#define LOCATION_LENGTH 10

class SNCL::SNCLImpl
{
public:
    SNCLImpl() = default;
    SNCLImpl(const SNCLImpl &sncl)
    {
        *this = sncl;
    }
    SNCLImpl& operator=(const SNCLImpl &sncl)
    {
        if (&sncl == this){return *this;}
        std::memcpy(mNetwork,  sncl.mNetwork, NETWORK_LENGTH*sizeof(char));
        std::memcpy(mStation,  sncl.mStation, STATION_LENGTH*sizeof(char));
        std::memcpy(mChannel,  sncl.mChannel, CHANNEL_LENGTH*sizeof(char));
        std::memcpy(mLocation, sncl.mLocation, LOCATION_LENGTH*sizeof(char));
        return *this;
    }
    void clear()
    {
        std::memset(mNetwork,  0, NETWORK_LENGTH*sizeof(char));
        std::memset(mStation,  0, STATION_LENGTH*sizeof(char));
        std::memset(mChannel,  0, CHANNEL_LENGTH*sizeof(char));
        std::memset(mLocation, 0, LOCATION_LENGTH*sizeof(char)); 
    } 
    char mNetwork[NETWORK_LENGTH+1]   = {"\0\0\0\0\0\0\0\0\0\0"};
    char mStation[STATION_LENGTH+1]   = {"\0\0\0\0\0\0\0\0\0\0"};
    char mChannel[CHANNEL_LENGTH+1]   = {"\0\0\0\0\0\0\0\0\0\0"};
    char mLocation[LOCATION_LENGTH+1] = {"\0\0\0\0\0\0\0\0\0\0"};
};

/// Constructors
SNCL::SNCL() :
    pImpl(std::make_unique<SNCLImpl> ())
{
}

SNCL::SNCL(const SNCL &sncl)
{
    *this = sncl;
}

/// Operators
SNCL::SNCL(SNCL &&sncl) noexcept
{
    *this = std::move(sncl);
}

SNCL& SNCL::operator=(const SNCL &sncl)
{
    if (&sncl == this){return *this;}
    pImpl = std::make_unique<SNCLImpl> (*sncl.pImpl);
    return *this;
}

SNCL& SNCL::operator=(SNCL &&sncl) noexcept
{
    if (&sncl == this){return *this;}
    pImpl = std::move(sncl.pImpl);
    return *this;
}

bool SNCL::operator==(const SNCL &sncl) noexcept
{
    if (getNetwork() != sncl.getNetwork()){return false;}
    if (getStation() != sncl.getStation()){return false;}
    if (getChannel() != sncl.getChannel()){return false;}
    if (getLocationCode() != sncl.getLocationCode()){return false;}
    return true;
}

bool SNCL::operator!=(const SNCL &sncl) noexcept
{
    return !(*this == sncl);
}

/// Destructors
SNCL::~SNCL() = default;

void SNCL::clear() noexcept
{
    pImpl->clear();
}

/// Network
int SNCL::getMaximumNetworkLength() const noexcept
{
    return NETWORK_LENGTH;
}

std::string SNCL::getNetwork() const noexcept
{
    std::string result(pImpl->mNetwork); //, NETWORK_LENGTH);
    return result;
}

void SNCL::setNetwork(const std::string &str) noexcept
{
    constexpr size_t maxlen = NETWORK_LENGTH;
    size_t len = std::min(str.size(), maxlen);
    std::memset(pImpl->mNetwork, 0, NETWORK_LENGTH*sizeof(char));
    std::memcpy(pImpl->mNetwork, str.data(), len*sizeof(char));
}

/// Station
int SNCL::getMaximumStationLength() const noexcept
{
    return STATION_LENGTH;
}

std::string SNCL::getStation() const noexcept
{
    std::string result(pImpl->mStation); //, STATION_LENGTH);
    return result;
}

void SNCL::setStation(const std::string &str) noexcept
{
    constexpr size_t maxlen = STATION_LENGTH;
    size_t len = std::min(str.size(), maxlen);
    std::memset(pImpl->mStation, 0, STATION_LENGTH*sizeof(char));
    std::memcpy(pImpl->mStation, str.data(), len*sizeof(char));
}

/// Channel
int SNCL::getMaximumChannelLength() const noexcept
{
    return CHANNEL_LENGTH;
}

std::string SNCL::getChannel() const noexcept
{
    std::string result(pImpl->mChannel); //, CHANNEL_LENGTH);
    return result;
}

void SNCL::setChannel(const std::string &str) noexcept
{
    constexpr size_t maxlen = CHANNEL_LENGTH;
    size_t len = std::min(str.size(), maxlen);
    std::memset(pImpl->mChannel, 0, CHANNEL_LENGTH*sizeof(char));
    std::memcpy(pImpl->mChannel, str.data(), len*sizeof(char));
}

/// Location code
int SNCL::getMaximumLocationCodeLength() const noexcept
{
    return LOCATION_LENGTH;
}

std::string SNCL::getLocationCode() const noexcept
{
    std::string result(pImpl->mLocation); //LOCATION_LENGTH);
    return result;
}

void SNCL::setLocationCode(const std::string &str) noexcept
{
    constexpr size_t maxlen = LOCATION_LENGTH;
    size_t len = std::min(str.size(), maxlen);
    std::memset(pImpl->mLocation, 0, LOCATION_LENGTH*sizeof(char));
    std::memcpy(pImpl->mLocation, str.data(), len*sizeof(char));
}

bool SNCL::isEmpty() const noexcept
{
    size_t lenos = strnlen(pImpl->mNetwork,  NETWORK_LENGTH)
                 + strnlen(pImpl->mStation,  STATION_LENGTH)
                 + strnlen(pImpl->mChannel,  CHANNEL_LENGTH)
                 + strnlen(pImpl->mLocation, LOCATION_LENGTH);
    if (lenos == 0){return true;}
    return false;
}
