#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <algorithm>
#include <functional>
#include <string>
#include "temblor/models/timeSeriesData/waveformIdentifier.hpp"

using namespace Temblor::Models::TimeSeriesData;

class WaveformIdentifier::WaveformIdentifierImpl
{
public:
    std::string mNetwork;
    std::string mStation;
    std::string mChannel;
    std::string mLocationCode;
    std::string mComment;
    size_t mHash = 0;
    bool mHaveHash = false;
};

/// Constructors
WaveformIdentifier::WaveformIdentifier() :
    pImpl(std::make_unique<WaveformIdentifierImpl> ())
{
}

WaveformIdentifier::WaveformIdentifier(
    const std::string &network,
    const std::string &station,
    const std::string &channel) noexcept :
    pImpl(std::make_unique<WaveformIdentifierImpl> ())
{
    setNetworkName(network);
    setStationName(station);
    setChannelName(channel);
    setLocationCode("--");
}

WaveformIdentifier::WaveformIdentifier(
    const std::string &network,
    const std::string &station,
    const std::string &channel,
    const std::string &locationCode) noexcept :
    pImpl(std::make_unique<WaveformIdentifierImpl> ())
{
    setNetworkName(network);
    setStationName(station);
    setChannelName(channel);
    setLocationCode(locationCode);
}

WaveformIdentifier::WaveformIdentifier(
    const std::string &network,
    const std::string &station,
    const std::string &channel,
    const std::string &locationCode,
    const std::string &comment) noexcept :
    pImpl(std::make_unique<WaveformIdentifierImpl> ())
{
    setNetworkName(network);
    setStationName(station);
    setChannelName(channel);
    setLocationCode(locationCode);
    setComment(comment);
}

WaveformIdentifier::WaveformIdentifier(const WaveformIdentifier &id)
{
    *this = id;
}

WaveformIdentifier::WaveformIdentifier(WaveformIdentifier &&id) noexcept
{
    *this = std::move(id);
}

/// Operators
WaveformIdentifier&
WaveformIdentifier::operator=(const WaveformIdentifier &id)
{
    if (&id == this){return *this;}
    pImpl = std::make_unique<WaveformIdentifierImpl> (*id.pImpl);
    return *this;
}

WaveformIdentifier&
WaveformIdentifier::operator=(WaveformIdentifier &&id) noexcept
{
    if (&id == this){return *this;}
    pImpl = std::move(id.pImpl);
    return *this;
}

bool WaveformIdentifier::operator==(const WaveformIdentifier &waveID)
{
    return getIdentifier() == waveID.getIdentifier();
/*
    if (pImpl->mNetwork != waveID.getNetwork()){return false;}
    if (pImpl->mStation != waveID.getStation()){return false;}
    if (pImpl->mChannel != waveID.getChannel()){return false;}
    if (pImpl->mLocationCode != waveID.getLocationCode()){return false;}
    return pImpl->mComment == waveID.getComment();
*/
}

bool WaveformIdentifier::operator!=(const WaveformIdentifier &waveID)
{
    return !(*this == waveID);
}

/// Destructors
WaveformIdentifier::~WaveformIdentifier() = default;

void WaveformIdentifier::clear() noexcept
{
    pImpl->mNetwork.clear();
    pImpl->mStation.clear();
    pImpl->mChannel.clear();
    pImpl->mLocationCode.clear();
    pImpl->mComment.clear();
    pImpl->mHash = 0;
    pImpl->mHaveHash = false;
}

/// Network
void WaveformIdentifier::setNetworkName(const std::string &network) noexcept
{
    pImpl->mHaveHash = false;
    pImpl->mNetwork.resize(network.size());
    std::transform(network.begin(), network.end(),
                   pImpl->mNetwork.begin(), std::toupper);
}

std::string WaveformIdentifier::getNetworkName() const noexcept
{
    return pImpl->mNetwork;
}

/// Station
void WaveformIdentifier::setStationName(const std::string &station) noexcept
{
    pImpl->mHaveHash = false;
    pImpl->mStation.resize(station.size());
    std::transform(station.begin(), station.end(),
                   pImpl->mStation.begin(), std::toupper);
}

std::string WaveformIdentifier::getStationName() const noexcept
{
    return pImpl->mStation;
}

/// Channel
void WaveformIdentifier::setChannelName(const std::string &channel) noexcept
{
    pImpl->mHaveHash = false;
    pImpl->mChannel.resize(channel.size());
    std::transform(channel.begin(), channel.end(),
                   pImpl->mChannel.begin(), std::toupper);
}

std::string WaveformIdentifier::getChannelName() const noexcept
{
    return pImpl->mChannel;
}

/// Location code
void WaveformIdentifier::setLocationCode(const std::string &loc) noexcept
{
    pImpl->mHaveHash = false;
    pImpl->mLocationCode.resize(loc.size());
    std::transform(loc.begin(), loc.end(),
                   pImpl->mLocationCode.begin(), std::toupper);
}

std::string WaveformIdentifier::getLocationCode() const noexcept
{
    return pImpl->mLocationCode;
}

/// Comment
void WaveformIdentifier::setComment(const std::string &comment) noexcept
{
    pImpl->mHaveHash = false;
    pImpl->mComment = comment;
}

std::string WaveformIdentifier::getComment() const noexcept
{
    return pImpl->mComment;
}

/// Lazy evaluation - gets identifier which is made with a hash
size_t WaveformIdentifier::getIdentifier() const noexcept
{
    if (pImpl->mHaveHash){return pImpl->mHash;}
    auto hashString = getHashString();
    pImpl->mHash = std::hash<std::string>{}(hashString);
    pImpl->mHaveHash = true;
    return pImpl->mHash;
}

/// Gets the string to be hashed
std::string WaveformIdentifier::getHashString() const noexcept
{
    std::string hashString = pImpl->mNetwork + "|"
                           + pImpl->mStation + "|"
                           + pImpl->mChannel + "|"
                           + pImpl->mLocationCode + "|"
                           + pImpl->mComment;
    return hashString; 
}
