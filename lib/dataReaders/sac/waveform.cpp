#include <cstdio>
#include <cstdlib>
#include <cassert>
#include <string>
#include "temblor/library/dataReaders/sac/waveform.hpp"
#include "temblor/library/dataReaders/sac/header.hpp"

using namespace Temblor::Library::DataReaders::SAC;

static double *alignedAllocDouble(const int npts);

class Waveform::WaveformImpl
{
public:
    /// Constructor
    WaveformImpl() = default;
    /// Copy assignment
    WaveformImpl& operator=(const WaveformImpl &waveform)
    {
        if (&waveform == this){return *this;}
        mHeader = waveform.mHeader;
        int npts = mHeader.getHeader(Integer::NPTS);
        if (npts > 0 && waveform.mData)
        {
            mData = alignedAllocDouble(npts);
            std::memcpy(mData, waveform.mData, static_cast<size_t> (npts));
        }
        return *this;
    }
    /// Destructor
    ~WaveformImpl()
    {
        if (mData){free(mData);}
        mData = nullptr;
    }

//private:
    class Header mHeader;
    double *mData = nullptr;
};

Waveform::Waveform() :
    pImpl(std::make_unique<WaveformImpl> ())
{
}

Waveform::Waveform(const Waveform &waveform)
{
    *this = waveform;
}

Waveform& Waveform::operator=(const Waveform &waveform)
{
    if (&waveform == this){return *this;}
    if (pImpl){pImpl.reset();}
    pImpl = std::make_unique<WaveformImpl> (*waveform.pImpl);
    return *this;
}


Waveform::~Waveform() = default;

static double *alignedAllocDouble(const int npts)
{
    size_t nbytes = static_cast<size_t> (npts)*sizeof(double);
#ifndef __OSX__ //ALIGNED_ALLOC
    void *dataTemp = malloc(nbytes);
    posix_memalign(&dataTemp, 64, nbytes);
    auto data = static_cast<double *> (dataTemp);
#else
    double *data = static_cast<double *> (std::aligned_alloc(64, nbytes));
#endif
    return data;
}