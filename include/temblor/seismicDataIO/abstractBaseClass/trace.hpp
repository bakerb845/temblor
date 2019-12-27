#ifndef TEMBLOR_SEISMICDATAIO_ABSTRACTBASECLASS_TRACE_HPP
#define TEMBLOR_SEISMICDATAIO_ABSTRACTBASECLASS_TRACE_HPP
#include <memory>
#include <vector>
#include "temblor/utilities/time.hpp"

// Forward declarations
namespace Temblor::Utilities
{
class Time;
}

namespace Temblor::SeismicDataIO::AbstractBaseClass
{
/*!
 * @brief The SAC waveform class.  A SAC waveform is comprised of a time series
 *        and is characterized by a SAC header.
 */
class ITrace
{
public:
    virtual ~ITrace() = default;
    virtual int getNumberOfSamples() const = 0;
    virtual double getSamplingRate() const = 0;
    virtual double getSamplingPeriod() const = 0;
    virtual void getData(int npts, double *data[]) const = 0;
    virtual void getData(int npts, float *data[]) const = 0;
    virtual Temblor::Utilities::Time getStartTime() const = 0;
};
}
#endif
