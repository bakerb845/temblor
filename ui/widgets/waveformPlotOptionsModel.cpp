#include <cstdio>
#include <cstdlib>
#include <string>
#include "temblor/userInterface/widgets/waveformPlotOptionsModel.hpp"
#include "temblor/userInterface/models/rgba.hpp"

namespace UIModels = Temblor::UserInterface::Models;
using namespace Temblor::UserInterface::Widgets;

class WaveformPlotOptionsModel::WaveformPlotOptionsModelImpl
{
public:
    int mMaximumNumberOfVisibleWaveforms = 12;
};

/// Constructors
WaveformPlotOptionsModel::WaveformPlotOptionsModel() :
    pImpl(std::make_unique<WaveformPlotOptionsModelImpl> ())
{
}

WaveformPlotOptionsModel::WaveformPlotOptionsModel(
    const WaveformPlotOptionsModel &options)
{
    *this = options;
}

WaveformPlotOptionsModel::WaveformPlotOptionsModel(
    WaveformPlotOptionsModel &&options) noexcept
{
    *this = std::move(options);
}

/// Operators
WaveformPlotOptionsModel&
WaveformPlotOptionsModel::operator=(const WaveformPlotOptionsModel &options)
{
    if (&options == this){return *this;}
    pImpl = std::make_unique<WaveformPlotOptionsModelImpl> (*options.pImpl);
    return *this;
} 

WaveformPlotOptionsModel&
WaveformPlotOptionsModel::operator=(WaveformPlotOptionsModel &&options) noexcept
{
    if (&options == this){return *this;}
    pImpl = std::move(options.pImpl);
    return *this;
}
   

/// Destructors
WaveformPlotOptionsModel::~WaveformPlotOptionsModel() = default;

/// Maximum  number of visible waveforms
void WaveformPlotOptionsModel::setMaximumNumberOfVisibleWaveforms( 
    const int maximumNumberOfVisibleWaveforms)
{
    if (maximumNumberOfVisibleWaveforms < 1)
    {
        throw std::invalid_argument("Maximum number of visible waveforms = "
                           + std::to_string(maximumNumberOfVisibleWaveforms)
                           + " must be positive");
    }
    pImpl->mMaximumNumberOfVisibleWaveforms = maximumNumberOfVisibleWaveforms;
}

int WaveformPlotOptionsModel::getMaximumNumberOfVisibleWaveforms() const noexcept
{
    return pImpl->mMaximumNumberOfVisibleWaveforms;
}
