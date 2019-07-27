#include <cstdio>
#include <cstdlib>
#include <cassert>
#include <cstring>
#include <string>
#include <algorithm>
#include <fstream>
#include "temblor/private/filesystem.hpp"
#include "temblor/utilities/time.hpp"
#include "temblor/dataReaders/sac/waveform.hpp"
#include "temblor/dataReaders/sac/header.hpp"

using namespace Temblor::DataReaders::SAC;

static double *alignedAllocDouble(const int npts);

class Waveform::WaveformImpl
{
public:
    /// Constructor
    WaveformImpl()
    {
        // Must give header version
        mHeader.setHeader(Integer::NVHDR, 6);
        // Must be an evenly spaced time series file
        mHeader.setHeader(Integer::IFTYPE, 1);
        mHeader.setHeader(Logical::LEVEN, true); // Default if not set
        // Force the start time to be the epoch
        /*
        mHeader.setHeader(Integer::NZYEAR, 1970); /// = default;
        mHeader.setHeader(Integer::NZJDAY, 1);
        mHeader.setHeader(Integer::NZHOUR, 0);
        mHeader.setHeader(Integer::NZMIN,  0);
        mHeader.setHeader(Integer::NZSEC,  0);
        mHeader.setHeader(Integer::NZMSEC, 0);
        */
        // Turns out b =-12345 is okay but looks screwy when plotted
        mHeader.setHeader(Double::B, 0);
    }
    WaveformImpl(const WaveformImpl &waveform)
    {
        *this = waveform;
    } 
    /// Copy assignment
    WaveformImpl& operator=(const WaveformImpl &waveform)
    {
        if (&waveform == this){return *this;}
        // Release old memory
        clear();
        mHeader = waveform.mHeader;
        int npts = mHeader.getHeader(Integer::NPTS);
        if (npts > 0 && waveform.mData)
        {
            mData = alignedAllocDouble(npts);
            auto nbytes = static_cast<size_t> (npts)*sizeof(double);
            std::memcpy(mData, waveform.mData, nbytes);
        }
        return *this;
    }
    /// Destructor
    ~WaveformImpl()
    {
        clear();
    }
    void freeData()
    {
        if (mData){free(mData);}
        mHeader.setHeader(Integer::NPTS, 0);
        mData = nullptr;
    }
    void clear()
    {
        freeData();
        mHeader.clear();
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

Waveform::Waveform(Waveform &&waveform) noexcept
{
    *this = std::move(waveform);
}

Waveform& Waveform::operator=(const Waveform &waveform)
{
    if (&waveform == this){return *this;}
    if (pImpl){pImpl.reset();}
    pImpl = std::make_unique<WaveformImpl> (*waveform.pImpl);
    return *this;
}

Waveform& Waveform::operator=(Waveform &&waveform) noexcept
{
    if (&waveform == this){return *this;}
    if (pImpl){pImpl.reset();}
    pImpl = std::move(waveform.pImpl);
    return *this;
}

Waveform::~Waveform() = default;

void Waveform::clear() noexcept
{
    pImpl->mHeader.clear();
    if (pImpl->mData){free(pImpl->mData);}
    pImpl->mData = nullptr;
}

void Waveform::read(const std::string &fileName)
{
    clear();
#if TEMBLOR_USE_FILESYSTEM == 1
    if (!fs::exists(fileName))
    {
        std::string errmsg = "SAC file = " + fileName + " does not exist";
        throw std::invalid_argument(errmsg);
    }
#endif
    // Read the binary file
    std::ifstream sacfl(fileName, std::ios::binary);
    std::vector<char> buffer(std::istreambuf_iterator<char> (sacfl), {});
    sacfl.close();
    size_t nbytes = buffer.size();
    if (nbytes < 632)
    {
        std::string errmsg = "SAC file has less than 632 bytes; nbytes = "
                           + std::to_string(nbytes);
        throw std::invalid_argument(errmsg);
    }
    // Figure out the byte order
    const char *cdat = buffer.data();
    union
    {
        char c4[4];
        int npts;
    };
    std::memcpy(c4, &cdat[316], 4*sizeof(char));
    size_t nbytesEst = static_cast<size_t> (npts)*sizeof(float) + 632;
    bool lswap = false;
    if (nbytesEst != nbytes)
    {
        std::reverse(c4, c4+4);
        nbytesEst = static_cast<size_t> (npts)*sizeof(float) + 632;
        if (nbytesEst != nbytes)
        {
            std::string errmsg = "Cannot determine endianness of file";
            throw std::invalid_argument(errmsg);
        }
        lswap = true;
    }
    // Unpack the header (this will check npts and delta are valid)
    try
    {
        pImpl->mHeader.setFromBinaryHeader(cdat, lswap);
    }
    catch (const std::invalid_argument &ia)
    {
        pImpl->mHeader.clear();
        throw std::invalid_argument(ia);
    }
    // Unpack the data
    pImpl->mData = alignedAllocDouble(npts);
    if (!lswap)
    {
        auto fdata = reinterpret_cast<const float *> (buffer.data() + 632);
        #pragma omp simd
        for (auto i=0; i<npts; i++)
        {
            pImpl->mData[i] = static_cast<double> (fdata[i]);
        }
    }
    else
    {
        // Reverse the byte order
        union
        {
            char crev[4];
            float f4;
        };
        for (auto i=0; i<npts; i++)
        {
            auto indx = 632 + 4*i;
            crev[0] = cdat[indx+3];
            crev[1] = cdat[indx+2];
            crev[2] = cdat[indx+1];
            crev[3] = cdat[indx+0];
            //std::reverse_copy(&cdat[632+4*i], &cdat[632+4*i]+4, crev);
            pImpl->mData[i] = static_cast<double> (f4);
        }
    }
}

void Waveform::write(const std::string &fileName, const bool lswap) const
{
    if (!isValid())
    {
        throw std::runtime_error("SAC waveform is not valid");
    }
    // Make sure the root directory exists
#if TEMBLOR_USE_FILESYSTEM == 1
    fs::path path(fileName);
    if (!path.has_relative_path())
    {
        fs::path pathName = path.parent_path();
        if (!fs::create_directories(pathName))
        {
            std::string errmsg = "Failed to make directory "
                               + std::string(pathName.c_str());
            throw std::invalid_argument(errmsg);
        }
    }
#endif
    // Pack the header
    int npts = getNumberOfSamples();
    size_t nbytes = 632 + sizeof(float)*static_cast<size_t> (npts);
    std::vector<char> cdata(nbytes);
    pImpl->mHeader.getBinaryHeader(cdata.data(), lswap);
    // Pack the data
    if (!lswap)
    {
        auto fdata = reinterpret_cast<float *> (cdata.data() + 632);
        #pragma omp simd
        for (auto i=0; i<npts; ++i)
        {
            fdata[i] = static_cast<float> (pImpl->mData[i]);
        }
    }
    else
    {
        union
        {
            char c4[4];
            float f4; 
        };
        for (int i=0; i<npts; ++i)
        {
            f4 = static_cast<float> (pImpl->mData[i]);
            auto indx = 632 + i*4;
            cdata[indx]   = c4[3];
            cdata[indx+1] = c4[2];
            cdata[indx+2] = c4[1];
            cdata[indx+3] = c4[0];
        }
    }
    // Write it 
    std::ofstream outfile(fileName,
                          std::ofstream::binary | std::ofstream::trunc);
    outfile.write(cdata.data(), nbytes);
    outfile.close();
}

Temblor::Utilities::Time Waveform::getStartTime() const
{
    int year   = pImpl->mHeader.getHeader(SAC::Integer::NZYEAR);
    int jday   = pImpl->mHeader.getHeader(SAC::Integer::NZJDAY);
    int hour   = pImpl->mHeader.getHeader(SAC::Integer::NZHOUR);
    int minute = pImpl->mHeader.getHeader(SAC::Integer::NZMIN);
    int isec   = pImpl->mHeader.getHeader(SAC::Integer::NZSEC);
    int musec  = pImpl->mHeader.getHeader(SAC::Integer::NZMSEC)*1000;
    double b = pImpl->mHeader.getHeader(SAC::Double::B); 
    if (year   ==-12345 || jday ==-12345 || hour  ==-12345 || 
        minute ==-12345 || isec ==-12345 || musec ==-12345 ||
        b ==-12345.0)
    {
        throw std::runtime_error("Header start time is not yet set\n");
    }
    // Create the time
    Utilities::Time startTime;
    startTime.setYear(year);
    startTime.setJulianDay(jday);
    startTime.setHour(hour);
    startTime.setMinute(minute);
    startTime.setSecond(isec);
    startTime.setMicroSecond(musec);
    // May have to deal with b
    if (b == 0){return startTime;}
    double epochalTime = startTime.getEpochalTime() + b;
    startTime.setEpochalTime(epochalTime);
    return startTime;
}

void Waveform::setStartTime(const Utilities::Time &startTime) noexcept
{
     pImpl->mHeader.setHeader(SAC::Integer::NZYEAR, startTime.getYear()); 
     pImpl->mHeader.setHeader(SAC::Integer::NZJDAY, startTime.getJulianDay());
     pImpl->mHeader.setHeader(SAC::Integer::NZHOUR, startTime.getHour());
     pImpl->mHeader.setHeader(SAC::Integer::NZMIN,  startTime.getMinute());
     pImpl->mHeader.setHeader(SAC::Integer::NZSEC,  startTime.getSecond());
     auto millisec = static_cast<int> (startTime.getMicroSecond()*1.e-3 + 0.5);
     pImpl->mHeader.setHeader(SAC::Integer::NZMSEC, millisec);
     pImpl->mHeader.setHeader(SAC::Double::B, 0.0);    
}

void Waveform::setHeader(const Double variableName, const double value)
{
    pImpl->mHeader.setHeader(variableName, value);
}

double Waveform::getHeader(const Double variableName) const noexcept
{
    return pImpl->mHeader.getHeader(variableName);
}

void Waveform::setHeader(const Integer variableName, const int value)
{
    if (variableName == Integer::NPTS)
    {
        throw std::invalid_argument("NPTS cannot be set with this function");
    }
    if (variableName == Integer::IFTYPE)
    {
        throw std::invalid_argument("IFTYPE cannot be set with this function");
    }
    pImpl->mHeader.setHeader(variableName, value);
}

int Waveform::getHeader(const Integer variableName) const noexcept
{
    return pImpl->mHeader.getHeader(variableName);
}

void Waveform::setHeader(const Logical variableName, const bool value) noexcept
{
    pImpl->mHeader.setHeader(variableName, value);
}

int Waveform::getHeader(const Logical variableName) const noexcept
{
    return pImpl->mHeader.getHeader(variableName);
}

void Waveform::setHeader(const Character variableName,
                         const std::string &value) noexcept
{
    pImpl->mHeader.setHeader(variableName, value);
}

std::string Waveform::getHeader(const Character variableName) const noexcept
{
    return pImpl->mHeader.getHeader(variableName);
}
                         

double Waveform::getSamplingPeriod() const noexcept
{
    return pImpl->mHeader.getHeader(Double::DELTA);
}

int Waveform::getNumberOfSamples() const noexcept
{
    return pImpl->mHeader.getHeader(Integer::NPTS);
}

bool Waveform::isValid() const noexcept
{
    if (!pImpl){return false;}
    if (getSamplingPeriod() <= 0){return false;}
    if (getNumberOfSamples() < 0){return false;}
    if (pImpl->mData == nullptr){return false;}
    return true;
}

const double *Waveform::getDataPointer() const noexcept
{
    return pImpl->mData;
}

std::vector<double> Waveform::getData() const noexcept
{
    int npts = getNumberOfSamples();
    if (npts > 0 && pImpl->mData)
    {
        std::vector<double> data(pImpl->mData, pImpl->mData + npts);
        return data;
    }
    else
    {
        std::vector<double> data(0);
        return data;
    }
}

void Waveform::setData(const int npts, const double x[])
{
    pImpl->freeData();
    if (npts <= 0)
    {
        throw std::invalid_argument("npts = " + std::to_string(npts)
                                  + " must be positive\n");
    }
    if (x == nullptr)
    {
        throw std::invalid_argument("x is NULL");
    }
    pImpl->mHeader.setHeader(Integer::NPTS, npts);
    pImpl->mData = alignedAllocDouble(npts);
    std::memcpy(pImpl->mData, x, static_cast<size_t> (npts)*sizeof(double));
}

//============================================================================//

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
