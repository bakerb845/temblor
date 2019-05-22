#include <cstdio>
#include <cstdlib>
#include <string>
#include <algorithm>
#include <vector>
#include "temblor/library/models/timeSeriesData/singleChannelWaveform.hpp"
#include "temblor/library/utilities/time.hpp"
#include "temblor/library/utilities/geodetic/globalPosition.hpp"

using namespace Temblor::Library::Utilities;
using namespace Temblor::Library::Models::TimeSeriesData;

class SingleChannelWaveform::SingleChannelWaveformImpl
{
public:
    /// Constructor
    SingleChannelWaveformImpl() = default;
    /// Destructor
    ~SingleChannelWaveformImpl() = default;

    /// The network to which the station belongs
    std::string networkName{"", 64};
    /// The name of the station
    std::string stationName{"", 64};
    /// The channel name
    std::string channelName{"", 64};
    /// The location code
    std::string locationCode{"", 64};
    /// The station's location
    Geodetic::GlobalPosition location;
    /// The channel start time
    Time startTime;
    /// Container with the waveform data
    std::vector<double> mData;
    /// The sampling period
    double mSamplingPeriod = 0;
    /// Number of samples in waveform
    int mNumberOfSamples = 0;
};


SingleChannelWaveform::SingleChannelWaveform() :
    pImpl(std::make_unique<SingleChannelWaveformImpl> ())
{
}

SingleChannelWaveform::~SingleChannelWaveform() = default;

int SingleChannelWaveform::getNumberOfSamples() const noexcept
{
    return pImpl->mNumberOfSamples;
}

/*
double SingleChannelWaveform::getStartTime() const noexcept
{
    return pImpl->startTime.getEpochalTime();
}

double SingleChannelWaveform::getEndtime() const noexcept
{
    auto npts = getNumberOfSamples();
    auto startTime = getStartTime();
    auto endTime = startTime + static_cast<double> (std::max(0, npts - 1))*dt;
    return endTime;
}
*/


void SingleChannelWaveform::setNetworkName(const std::string &str) noexcept
{
    auto len = std::min(pImpl->networkName.capacity(), str.size());
    pImpl->networkName.assign(str, len);
}

std::string SingleChannelWaveform::getNetworkName() const noexcept
{
    return pImpl->networkName;
}

void SingleChannelWaveform::setStationName(const std::string &str) noexcept
{
    auto len = std::min(pImpl->stationName.capacity(), str.size());
    pImpl->stationName.assign(str, len);
}

std::string SingleChannelWaveform::getStationName() const noexcept
{
    return pImpl->stationName;
}

void SingleChannelWaveform::setChannelName(const std::string &str) noexcept
{
    auto len = std::min(pImpl->channelName.capacity(), str.size());
    pImpl->channelName.assign(str, len);
}

std::string SingleChannelWaveform::getChannelName() const noexcept
{
    return pImpl->channelName;
}

void SingleChannelWaveform::setLocationCode(const std::string &str) noexcept
{
    auto len = std::min(pImpl->locationCode.capacity(), str.size());
    pImpl->locationCode.assign(str, len);
}

std::string SingleChannelWaveform::getLocationCode() const noexcept
{
    return pImpl->locationCode;
}
