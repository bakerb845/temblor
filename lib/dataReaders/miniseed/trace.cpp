#include <cstdio>
#include <cstdlib>
#include <climits>
#include <array>
#include <vector>
#include <string>
#include <libmseed.h>
#include "temblor/library/private/filesystem.hpp"
#include "temblor/library/dataReaders/miniseed/sncl.hpp"
#include "temblor/library/dataReaders/miniseed/trace.hpp"
#include "temblor/library/utilities/time.hpp"

using namespace Temblor::Library;
using namespace Temblor::Library::DataReaders::MiniSEED;

class Trace::TraceImpl
{
public:
    class Utilities::Time mStartTime;
    class SNCL mSNCL;
    std::vector<double> mData64f;
    std::vector<float> mData32f;
    std::vector<int> mData32i;
    double mSamplingRate = 0;
    int64_t mNumberOfSamples = 0;
    Precision mPrecision = Precision::UNKNOWN;
};

Trace::Trace() :
    pImpl(std::make_unique<TraceImpl> ())
{
}

Trace::Trace(const Trace &trace)
{
    *this = trace;
}

Trace::Trace(Trace &&trace) noexcept
{
    *this = std::move(trace);
}

Trace& Trace::operator=(const Trace &trace)
{
    if (&trace == this){return *this;}
    pImpl = std::make_unique<TraceImpl> (*trace.pImpl);
    return *this; 
}

Trace& Trace::operator=(Trace &&trace) noexcept
{
    if (&trace == this){return *this;}
    pImpl = std::move(trace.pImpl);
    return *this;
}

/// Destructors
Trace::~Trace() = default;

void Trace::clear() noexcept
{
    pImpl->mStartTime.clear();
    pImpl->mSNCL.clear();
    pImpl->mData64f.clear();
    pImpl->mData32f.clear();
    pImpl->mData32i.clear();
    pImpl->mSamplingRate = 0;
    pImpl->mNumberOfSamples = 0;
    pImpl->mPrecision = Precision::UNKNOWN;
}

/// FileIO
void Trace::read(const std::string &fileName, const SNCL &sncl)
{
    clear();
#if TEMBLOR_USE_FILESYSTEM == 1
    if (!fs::exists(fileName))
    {
        std::string errmsg = "SAC file = " + fileName + " does not exist";
        throw std::invalid_argument(errmsg);
    }
#endif
    if (sncl.isEmpty())
    {
        throw std::invalid_argument("SNCL cannot be empty");
    }
    pImpl->mSNCL = sncl;
    // Create a SNCL selection
    int retcode = 0;
    std::string network  = sncl.getNetwork();
    std::string station  = sncl.getStation();
    std::string channel  = sncl.getChannel();
    std::string location = sncl.getLocationCode();
    char *networkQuery = NULL;
    if (network.length() > 0){networkQuery = network.data();}
    char *stationQuery = NULL;
    if (station.length() > 0){stationQuery = station.data();}
    char *channelQuery = NULL;
    if (channel.length() > 0){channelQuery = channel.data();}
    char *locationQuery = NULL;
    if (location.length() > 0){locationQuery = location.data();}
    // Pack the SNCL into a miniSEED source identifier (SID)
    std::array<char, LM_SIDLEN+1> sid;
    memset(sid.data(), 0, (LM_SIDLEN+1)*sizeof(char));
    retcode = ms_nslc2sid(sid.data(), LM_SIDLEN, 0,
                          networkQuery, stationQuery,
                          locationQuery, channelQuery);
    if (retcode < 0)
    {
        clear();
        throw std::runtime_error("Failed to create target SNCL");
    }
    // Load the trace list
    MS3TraceList *traceList = NULL;
    uint32_t flags = 0;
    flags = flags | MSF_VALIDATECRC;
    flags = flags | MSF_RECORDLIST;// | ~MSF_UNPACKDATA;
    MS3Tolerance *tolerance = NULL;
    retcode = ms3_readtracelist(&traceList, fileName.c_str(),
                                tolerance, 0, flags, 0);
    if (retcode != MS_NOERROR)
    {
        clear();
        mstl3_free(&traceList, 0);
        throw std::runtime_error("Failed to read trace list");
    }
    bool lfound = false;
    bool lfail = false;
    for (auto traceID=traceList->traces;
         traceID != NULL;
         traceID=traceID->next)
    {
        bool lmatch = false;
        if (strcasecmp(traceID->sid, sid.data())){lmatch = true;}
        if (!lmatch){continue;}
        lfound = true;
        for (auto segment = traceID->first; 
             segment != NULL;
             segment = segment->next)
        {
            // Check the pointer isn't NULL and this is fthe first go
            if (!segment->recordlist){continue;}
            if (!segment->recordlist->first){continue;}
            // Determine the sample size and type
            uint8_t sampleSize;
            char sampleType;
            ms_encoding_sizetype(segment->recordlist->first->msr->encoding,
                                 &sampleSize, &sampleType);
            void *dPtr = NULL;
            if (segment->samplecnt > INT_MAX)
            {
                fprintf(stderr, "%s: Number of samples = %ld can't exceed %d\n",
                        __func__, static_cast<size_t> (segment->samplecnt),
                        INT_MAX);
                lfail = true;
            }
            pImpl->mNumberOfSamples = segment->samplecnt;
            // Allocate space to receive unpacked data
            if (sampleType == 'i')
            {
                pImpl->mPrecision = Precision::INT32;
                pImpl->mData32i.resize(segment->samplecnt);
                dPtr = pImpl->mData32i.data();
            }
            else if (sampleType == 'f')
            {
                pImpl->mPrecision = Precision::FLOAT32;
                pImpl->mData32f.resize(segment->samplecnt);
                dPtr = pImpl->mData32f.data(); 
            }
            else if (sampleType == 'd')
            {
                pImpl->mPrecision = Precision::FLOAT64;
                pImpl->mData64f.resize(segment->samplecnt);
                dPtr = pImpl->mData64f.data();
            }
            else
            {
                fprintf(stderr, "%s: Unsupported sample type = %1s\n", 
                        __func__, &sampleType);
                lfail = true;
                goto EXIT;
            }
            // Does the sampling rate make sense?
            pImpl->mSamplingRate = segment->samprate;
            if (segment->samprate <= 0)
            {
                fprintf(stderr, "%s: Sampling rate = %lf must be positive",
                        __func__, segment->samprate);
                lfail = true;
            }
            // Set the start time (nstime is in nanoseconds)
            double startTime = (segment->starttime)*1.e-9;
            pImpl->mStartTime.setEpochalTime(startTime);
            // Unpack the data
            size_t outputSize = segment->samplecnt*sampleSize;
            auto unpacked = mstl3_unpack_recordlist(traceID, segment,
                                                    dPtr, outputSize, 0);
            if (unpacked != segment->samplecnt)
            {
                fprintf(stderr, "%s: Cannot unpack data for %s\n",
                        __func__, traceID->sid);
                lfail = true;
                goto EXIT;
            }
            goto EXIT; // I've got what I need - leave loop
        } // Loop on segment
    } // Loop on traces
EXIT:;
    if (traceList){mstl3_free(&traceList, 0);}
    if (lfail)
    {
        clear();
        throw std::runtime_error("Algorithmic failure calling miniSEED");
    }
    if (!lfound)
    {
        clear();
        throw std::invalid_argument("Could not find %s"
                                  + std::string(sid.data()));
    }
}

/// Precision
Precision Trace::getPrecision() const
{
    if (pImpl->mPrecision == Precision::UNKNOWN)
    {
        throw std::runtime_error("Data was never set");
    }
    return pImpl->mPrecision;
}

/// Start time
Utilities::Time Trace::getStartTime() const noexcept
{
    return pImpl->mStartTime;
}

/// Sampling rate
void Trace::setSamplingRate(const double samplingRate)
{
    pImpl->mSamplingRate = 0;
    if (samplingRate <= 0)
    {
        throw std::invalid_argument("Sampling rate "
                                  + std::to_string(samplingRate)
                                  + " must be positive");
    }
    pImpl->mSamplingRate = samplingRate;
}

double Trace::getSamplingRate() const
{
    if (pImpl->mSamplingRate <= 0)
    {
        throw std::runtime_error("Sampling rate not set");
    }
    return pImpl->mSamplingRate;
}

/// Sets the trace data
void Trace::setData(const size_t nSamples, const double x[])
{
    if (nSamples > INT_MAX)
    {
        throw std::runtime_error("Number of samples = "
                               + std::to_string(nSamples)
                               + " must be positive");
    }
    if (nSamples > 0 && x == nullptr)
    {
        throw std::invalid_argument("x cannot be NULL");
    }
    return;
}

/// Number of samples
int Trace::getNumberOfSamples() const noexcept
{
    return static_cast<int> (pImpl->mNumberOfSamples);
}

/// SNCL
void Trace::setSNCL(const SNCL &sncl)
{
    if (sncl.isEmpty())
    {
        throw std::invalid_argument("Can't set a blank SNCL");
    }
    pImpl->mSNCL = sncl;
}

SNCL Trace::getSNCL() const
{
    return pImpl->mSNCL;
}

/// Data

const int *Trace::getDataPointer32i() const
{
    if (pImpl->mPrecision == Precision::UNKNOWN)
    {
        throw std::runtime_error("Data never set");
    }
    if (pImpl->mPrecision != Precision::INT32)
    {
        throw std::runtime_error("Precision is not 32 bit integer");
    }
    return pImpl->mData32i.data();
}

const float *Trace::getDataPointer32f() const
{
    if (pImpl->mPrecision == Precision::UNKNOWN)
    {
        throw std::runtime_error("Data never set");
    }
    if (pImpl->mPrecision != Precision::FLOAT32)
    {
        throw std::runtime_error("Precision is not 32 bit float");
    }
    return pImpl->mData32f.data();
}

const double *Trace::getDataPointer64f() const
{
    if (pImpl->mPrecision == Precision::UNKNOWN)
    {
        throw std::runtime_error("Data never set");
    }
    if (pImpl->mPrecision != Precision::FLOAT64)
    {
        throw std::runtime_error("Precision is not 64 bit float");
    }
    return pImpl->mData64f.data();
}
