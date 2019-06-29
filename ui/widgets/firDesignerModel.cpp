#include <cstdio>
#include <cstdlib>
#include <string>
#include "temblor/userInterface/widgets/firDesignerModel.hpp"

using namespace Temblor::UserInterface::Widgets;

class FIRDesignerModel::FIRDesignerModelImpl
{
public:
    double mSamplingRate = 1;
    double mLowCriticalFrequency =-1.0;
    double mHighCriticalFrequency =-1.0;
    int mFilterLength = 100;
    BandType mBandType = BandType::LOWPASS;
    WindowType mWindowType = WindowType::HANNING;
};

/// Constructors
FIRDesignerModel::FIRDesignerModel() :
    pImpl(std::make_unique<FIRDesignerModelImpl> ())
{
}

FIRDesignerModel::FIRDesignerModel(const FIRDesignerModel &firModel)
{
    *this = firModel;
}

FIRDesignerModel::FIRDesignerModel(FIRDesignerModel &&firModel) noexcept
{
    *this = std::move(firModel);
}

/// Operators
FIRDesignerModel& FIRDesignerModel::operator=(const FIRDesignerModel &firModel)
{
    if (&firModel == this){return *this;}
    pImpl = std::make_unique<FIRDesignerModelImpl> (*firModel.pImpl);
    return *this;
}

FIRDesignerModel&
FIRDesignerModel::operator=(FIRDesignerModel &&firModel) noexcept
{
    if (&firModel == this){return *this;}
    pImpl = std::move(firModel.pImpl);
    return *this;
}

/// Destructors
FIRDesignerModel::~FIRDesignerModel() = default;

/// Sampling rate
void FIRDesignerModel::setSamplingRate(const double samplingRate)
{
    if (samplingRate <= 0)
    {
        throw std::invalid_argument("Filter length = " +
                                    std::to_string(samplingRate) +
                                    " must be positive");
    }
    pImpl->mLowCriticalFrequency =-1;
    pImpl->mHighCriticalFrequency =-1;
    pImpl->mSamplingRate = samplingRate;
}

double FIRDesignerModel::getSamplingRate() const noexcept
{
    return pImpl->mSamplingRate;
}

double FIRDesignerModel::getNyquistFrequency() const noexcept
{
    return pImpl->mSamplingRate/2;
}

/// Filter length
void FIRDesignerModel::setFilterLength(const int filterLength)
{
    if (filterLength < 1)
    {
        throw std::invalid_argument("Filter length = " +
                                    std::to_string(filterLength) +
                                    " must be positive");
    }
    pImpl->mFilterLength = filterLength;
}

int FIRDesignerModel::getFilterLength() const noexcept
{
    return pImpl->mFilterLength;
}

/// Band type
void FIRDesignerModel::setBandType(const BandType bandType) noexcept
{
    pImpl->mLowCriticalFrequency = -1;
    pImpl->mHighCriticalFrequency = -1;
    pImpl->mBandType = bandType;
}

FIRDesignerModel::BandType FIRDesignerModel::getBandType() const noexcept
{
    return pImpl->mBandType;
}

/// Window type
void FIRDesignerModel::setWindowType(const WindowType windowType) noexcept
{
    pImpl->mWindowType = windowType;
}

FIRDesignerModel::WindowType FIRDesignerModel::getWindowType() const noexcept
{
    return pImpl->mWindowType;
}

/// Critical frequencies
void FIRDesignerModel::setCriticalFrequency(const double fc)
{
    if (pImpl->mBandType == BandType::BANDPASS || pImpl->mBandType == BandType::BANDSTOP)
    {
        throw std::invalid_argument("This is for lowpass/highpass filters");
    }
    if (fc <= 0 || fc >= getNyquistFrequency())
    {
        throw std::invalid_argument("fc = " + std::to_string(fc) +
                                    "must be in range (0," +
                                    std::to_string(getNyquistFrequency()) +
                                    ")");
    }
    pImpl->mLowCriticalFrequency = fc;
}

double FIRDesignerModel::getCriticalFrequency() const
{
    if (pImpl->mBandType == BandType::BANDPASS ||
        pImpl->mBandType == BandType::BANDSTOP)
    {
        throw std::runtime_error("This is for lowpass/highpass filters");
    }
    if (pImpl->mLowCriticalFrequency < 0)
    {
        throw std::runtime_error("setCriticalFrequency never called");
    }
    return pImpl->mLowCriticalFrequency;
}

void FIRDesignerModel::setCriticalFrequencies(const std::pair<double, double> fc)
{
    if (pImpl->mBandType == BandType::LOWPASS ||
        pImpl->mBandType == BandType::HIGHPASS)
    {
        throw std::invalid_argument("This is for bandpass/bandstop filters");
    }
    if (fc.second >= getNyquistFrequency())
    {
        throw std::invalid_argument("fc.second = " +
                                    std::to_string(fc.second) +
                                    " must be less than " +
                                    std::to_string(getNyquistFrequency()));
    }
    if (fc.first <= 0 || fc.first >= fc.second)
    {
        throw std::invalid_argument("fc.first = " +
                                    std::to_string(fc.first) +
                                    " must be in range (0," +
                                    std::to_string(fc.second) + ")");
    }
    pImpl->mLowCriticalFrequency = fc.first;
    pImpl->mHighCriticalFrequency = fc.second;
}

std::pair<double, double>
FIRDesignerModel::getCriticalFrequencies() const
{
    if (pImpl->mBandType == BandType::LOWPASS ||
        pImpl->mBandType == BandType::HIGHPASS)
    {
        throw std::runtime_error("This is for bandpass/bandstop filters");
    }
    if (pImpl->mLowCriticalFrequency < 0)
    {
        throw std::runtime_error("setCriticalFrequencies never called");
    }
    return std::pair<double, double> (pImpl->mLowCriticalFrequency,
                                      pImpl->mHighCriticalFrequency);
}
