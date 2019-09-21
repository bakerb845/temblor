#include <cstdio>
#include <cstdlib>
#include <vector>
#include <string>
#include <algorithm>
#include <libmseed.h>
#include "temblor/private/filesystem.hpp"
#include "temblor/utilities/time.hpp"
#include "temblor/dataReaders/miniseed/traceGroup.hpp"
#include "temblor/dataReaders/miniseed/sncl.hpp"
#include "temblor/dataReaders/miniseed/trace.hpp"

using namespace Temblor::DataReaders::MiniSEED;

namespace
{
std::string sncl2str(const SNCL &sncl)
{
    std::string str;
    str = sncl.getNetwork() + "."
        + sncl.getStation() + "."
        + sncl.getChannel() + "."
        + sncl.getLocationCode();
    return str;
}
}

class TraceGroup::TraceGroupImpl
{
public:
    std::vector<SNCL> mSNCLs;
    std::vector<Trace> mTraces;
};

/// Constructor
TraceGroup::TraceGroup() :
    pImpl(std::make_unique<TraceGroupImpl> ())
{
}

/// Copy constructor
TraceGroup::TraceGroup(const TraceGroup &traceGroup)
{
    *this = traceGroup;
}

/// Move constructor
TraceGroup::TraceGroup(TraceGroup &&traceGroup) noexcept
{
    *this = std::move(traceGroup);
}

/// Copy assignment
TraceGroup& TraceGroup::operator=(const TraceGroup &traceGroup)
{
    if (&traceGroup == this){return *this;}
    pImpl = std::make_unique<TraceGroupImpl> (*traceGroup.pImpl);
    return *this;
}

/// Move assignment
TraceGroup& TraceGroup::operator=(TraceGroup &&traceGroup) noexcept
{
    if (&traceGroup == this){return *this;}
    pImpl = std::move(traceGroup.pImpl);
    return *this;
}

/// Destructor
TraceGroup::~TraceGroup() = default;

/// Clear data off the class
void TraceGroup::clear() noexcept
{
    pImpl->mSNCLs.clear();
    pImpl->mTraces.clear();
}

/// Read the traces
void TraceGroup::read(const std::string &fileName)
{
    clear();
#if TEMBLOR_USE_FILESYSTEM == 1
    if (!fs::exists(fileName))
    {
        std::string errmsg = "miniSEED file = " + fileName
                          + " does not exist\n";
        throw std::invalid_argument(errmsg);
    }
#endif
    // Load the trace list
    MS3TraceList *mstl = nullptr;
    constexpr uint32_t flags = 0;
    constexpr int8_t verbose = 0;
    constexpr int splitversion = 0;
    int retcode = MS_NOERROR;
    retcode = ms3_readtracelist(&mstl, fileName.c_str(), NULL,
                                splitversion, flags, verbose);
    if (retcode != MS_NOERROR)
    {
        auto error = std::string(ms_errorstr(retcode));
        throw std::invalid_argument("Encountered error: " + error
                                  + " when reading: " + fileName);
    }
    // Unpack the SNCLs
    auto id = mstl->traces;
    while (id)
    {
        // Extract the SNCL from the miniseed archive
        std::string network(11, 0);
        std::string station(11, 0);
        std::string channel(11, 0);
        std::string location(11, 0);
        retcode = ms_sid2nslc(id->sid,
                              network.data(), station.data(),
                              location.data(), channel.data());
        if (retcode != MS_NOERROR)
        {
            std::string error = std::string(ms_errorstr(retcode));
            throw std::invalid_argument("Encountered error: " + error
                                      + " when unpacking: " + fileName);
        }
        // Create the SNCL
        SNCL sncl;
        sncl.setNetwork(network);
        sncl.setStation(station);
        sncl.setChannel(channel);
        if (strnlen(location.c_str(), location.size()) > 0)
        {
            sncl.setLocationCode(location);
        }
        // Add the SNCL?
        auto idx = std::find(pImpl->mSNCLs.begin(), pImpl->mSNCLs.end(), sncl);
        if (idx == pImpl->mSNCLs.end()){pImpl->mSNCLs.push_back(sncl);}
        // Update the pointer to trace IDs
        id = id->next; 
    }
    // Clean up
    if (mstl){mstl3_free(&mstl, 0);}
    // Load the data
    int i = 0;
    for (auto sncl : pImpl->mSNCLs)
    {
        try
        {
            pImpl->mTraces[i].read(fileName, sncl);
        }
        catch (const std::exception &e)
        {
            clear();
            throw std::invalid_argument(e.what()); 
        }
        i = i + 1;
    }
}

/// Check if the SNCL exists
bool TraceGroup::haveSNCL(const SNCL &sncl) const noexcept
{
    auto idx = std::find(pImpl->mSNCLs.begin(), pImpl->mSNCLs.end(), sncl);
    if (idx == pImpl->mSNCLs.end()){return false;}
    return true;
}

/// Get the SNCLs
std::vector<SNCL> TraceGroup::getSNCLs() const noexcept
{
    return pImpl->mSNCLs;
}

/// Get the trace
Trace TraceGroup::getTrace(const SNCL &sncl) const
{
    /// Check the SNCL exists
    auto snclPtr = std::find(pImpl->mSNCLs.begin(), pImpl->mSNCLs.end(), sncl);
    if (snclPtr == pImpl->mSNCLs.end())
    {
        std::string errmsg = "SNCL = " + sncl2str(sncl)
                           + " is not loaded\n";
        throw std::invalid_argument(errmsg);
    }
    // Get the corresponding position and return that element of trace vector
    auto index = std::distance(pImpl->mSNCLs.begin(), snclPtr); 
    return pImpl->mTraces[index];
}

int TraceGroup::getNumberOfTraces() const noexcept
{
    return static_cast<int> (pImpl->mTraces.size());
}
