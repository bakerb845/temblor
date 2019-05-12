
class SingleChannelWaveform::SingleChannelWaveformImpl
{
public:
    SingleChannelWaveform() = default;


    /// The network to which the station belongs
    std::string networkName(64, 0);
    /// The name of the station
    std::string stationName(64, 0);
    /// The channel name
    std::string channelName(64, 0);
    /// The location code
    std::string locationCode(64, 0);
    /// The station's location
    Utilities::Location location;
    /// The channel start time
    Utilities::Time startTime;
    /// The sampling period
    double samplingPeirod = 0;
    /// Number of samples in waveform
    int npts = 0;
};

int SingleChannelWaveform::getNumberOfSamples() const noexcept
{
    return pImpl->npts;
}

double SingleChannelWaveform::getStartTime() const noexcept
{
    return pImpl->startTime.getEpochalTime();
}

double SingleChannelWaveform::getEndtime() const noexcept
{
    auto startTime = getStartTime();
    auto endTime = startTime + static_cast<double> (std::max(0, npts - 1))*dt;
    return endTime;
}

std::string SingleChannelWaveform::getNetworkName() const noexcept
{
    return pImpl->networkName();
}

std::string SingleChannelWaveform::getStationName() const noexcept
{
    return pImpl->stationName();
}

std::string SingleChannelWaveform::getChannelName() const noexcept
{
    return pImpl->channelName();
}

std::string SingleChannelWaveform::getLocationCode() const noexcept
{
    return pImpl->locationCode();
}
