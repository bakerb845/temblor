#include <cstdio>
#include <cstdlib>
#include <string>
#include <algorithm>
#include <vector>
#include "temblor/models/timeSeriesData/waveformIdentifier.hpp"
#include "temblor/models/timeSeriesData/singleChannelWaveform.hpp"
#include "temblor/utilities/time.hpp"
#include "temblor/utilities/geodetic/globalPosition.hpp"
#include "temblor/dataReaders/sac/waveform.hpp"
#include "temblor/dataReaders/miniseed/trace.hpp"

namespace DataReaders = Temblor::DataReaders;
using namespace Temblor::Utilities;
using namespace Temblor::Models::TimeSeriesData;

class SingleChannelWaveform::SingleChannelWaveformImpl
{
public:
    WaveformIdentifier mWaveID;
/*
    /// The network to which the station belongs
    std::string mNetworkName{"", 64};
    /// The name of the station
    std::string mStationName{"", 64};
    /// The channel name
    std::string mChannelName{"", 64};
    /// The location code
    std::string mLocationCode{"", 64};
*/
    /// The station's location
    Geodetic::GlobalPosition mLocation;
    /// The channel start time
    Time mStartTime;
    /// Container with the waveform data
    std::vector<double> mData;
    /// The sampling period
    double mSamplingRate = 0;
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

void SingleChannelWaveform::clear() noexcept
{
    pImpl->mWaveID.clear();
    pImpl->mStartTime.clear();
    pImpl->mLocation.clear();
    pImpl->mData.clear();
    pImpl->mStartTime.clear();
    pImpl->mSamplingRate = 0;
    pImpl->mNumberOfSamples = 0;
}

double SingleChannelWaveform::getEpochalStartTime() const noexcept
{
    return pImpl->mStartTime.getEpochalTime();
}

double SingleChannelWaveform::getEpochalEndTime() const noexcept
{
    auto npts = getNumberOfSamples();
    auto startTime = getEpochalStartTime();
    double dt = 0;
    try
    {
        dt = getSamplingPeriod();
    }
    catch (const std::exception &e)
    {
       dt = 0;
    }
    auto endTime = startTime + static_cast<double> (std::max(0, npts - 1))*dt;
    return endTime;
}

/// Network name
void SingleChannelWaveform::setNetworkName(const std::string &str) noexcept
{
    pImpl->mWaveID.setNetworkName(str);
    //auto len = std::min(pImpl->mNetworkName.capacity(), str.size());
    //pImpl->mNetworkName.assign(str, len);
}

std::string SingleChannelWaveform::getNetworkName() const noexcept
{
    return pImpl->mWaveID.getNetworkName(); //pImpl->mNetworkName;
}
/// Station name
void SingleChannelWaveform::setStationName(const std::string &str) noexcept
{
    pImpl->mWaveID.setStationName(str);
    //auto len = std::min(pImpl->mStationName.capacity(), str.size());
    //pImpl->mStationName.assign(str, len);
}

std::string SingleChannelWaveform::getStationName() const noexcept
{
    return pImpl->mWaveID.getStationName(); //pImpl->mStationName;
}
/// Channel name
void SingleChannelWaveform::setChannelName(const std::string &str) noexcept
{
    pImpl->mWaveID.setChannelName(str);
    //auto len = std::min(pImpl->mChannelName.capacity(), str.size());
    //pImpl->mChannelName.assign(str, len);
}

std::string SingleChannelWaveform::getChannelName() const noexcept
{
    return pImpl->mWaveID.getChannelName(); //pImpl->mChannelName;
}
/// Location code
void SingleChannelWaveform::setLocationCode(const std::string &str) noexcept
{
    pImpl->mWaveID.setLocationCode(str);
    //auto len = std::min(pImpl->mLocationCode.capacity(), str.size());
    //pImpl->mLocationCode.assign(str, len);
}

std::string SingleChannelWaveform::getLocationCode() const noexcept
{
    return pImpl->mWaveID.getLocationCode(); //pImpl->mLocationCode;
}

/// Sampling rate
bool SingleChannelWaveform::haveSamplingRate() const noexcept
{
    if (pImpl->mSamplingRate <= 0){return false;}
    return true;
}
 
void SingleChannelWaveform::setSamplingRate(const double df)
{
    pImpl->mSamplingRate = 0;
    if (df <= 0)
    {
        throw std::invalid_argument("Sampling rate = " + std::to_string(df)
                                  + " must be positive");
    }
    pImpl->mSamplingRate = df;
}

double SingleChannelWaveform::getSamplingPeriod() const
{
    if (pImpl->mSamplingRate <= 0)
    {
        throw std::runtime_error("Sampling rate never set");
    }
    return 1.0/pImpl->mSamplingRate;
}

double SingleChannelWaveform::getSamplingRate() const
{
    if (pImpl->mSamplingRate <= 0)
    {
        throw std::runtime_error("Sampling rate never set");
    }
    return pImpl->mSamplingRate;
}

double SingleChannelWaveform::getNyquistFrequency() const
{
    double df = getSamplingRate();
    return df/2.0;
}

/// Loads a SAC file
void SingleChannelWaveform::readSAC(const std::string &fileName)
{
    clear();
    DataReaders::SAC::Waveform sac; 
    sac.read(fileName); // Will throw
    // Now figure out the basics
    pImpl->mStartTime = sac.getStartTime(); // Will throw
    double dt = sac.getSamplingPeriod();
    if (dt <= 0)
    {
        throw std::runtime_error("Sampling period in header is invalid\n"); 
    }
    setSamplingRate(1./dt);
    pImpl->mData = sac.getData();
}

/*
void SingleChannelWaveform::readMiniSEED(const std::string &fileName)
{

}
*/
