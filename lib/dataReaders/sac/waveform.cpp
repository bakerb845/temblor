#include <cstdio>
#include <cstdlib>
#include <cassert>
#include <cstring>
#include <string>
#include <algorithm>
#include <fstream>
#if __has_include(<filesystem>)
#include <filesystem>
#define TEMBLOR_USE_FILESYSTEM 1
#else
#include <experimental/filesystem>
#define TEMBLOR_USE_FILESYSTEM 0
#endif
/*
#ifdef TEMBLOR_USE_FILESYTEM
#include <filesystem>
#else
#include <experimental/filesystem>
#endif
*/
#include "temblor/library/dataReaders/sac/waveform.hpp"
#include "temblor/library/dataReaders/sac/header.hpp"

using namespace Temblor::Library::DataReaders::SAC;
#if TEMBLOR_USE_FILESYSTEM == 1
namespace fs = std::filesystem;
#else
namespace fs = std::experimental::filesystem;
#endif

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

void Waveform::clear() noexcept
{
    pImpl->mHeader.clear();
    if (pImpl->mData){free(pImpl->mData);}
    pImpl->mData = nullptr;
}

void Waveform::read(const std::string &fileName)
{
    clear();
    if (!fs::exists(fileName))
    {
        std::string errmsg = "SAC file = " + fileName + " does not exist";
        throw std::invalid_argument(errmsg);
    }
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
        auto fdata = reinterpret_cast<const float *> (&cdat[632]);
        #pragma omp simd
        for (int i=0; i<npts; i++)
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
        for (int i=0; i<npts; i++)
        {
            int indx = 632 + 4*i;
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
    fs::path path(fileName);
    if (path.has_relative_path())
    {
        std::string pathName = path.parent_path();
        if (!fs::create_directories(pathName))
        {
            std::string errmsg = "Failed to make directory " + pathName;
            throw std::invalid_argument(errmsg);
        }
    }
    // Pack the header
    int npts = getNumberOfSamples();
    size_t nbytes = 632 + static_cast<size_t> (npts);
    std::vector<char> chdr(nbytes);
    pImpl->mHeader.getBinaryHeader(chdr.data(), lswap);
    // Pack the data
    union
    {
        char c4[4];
        float f4;
    };
    if (!lswap)
    {
        #pragma omp simd
        for (int i=0; i<npts; ++i)
        {
            f4 = static_cast<float> (pImpl->mData[i]);
            int indx = 632 + i*4;
            chdr[indx]   = c4[0];
            chdr[indx+1] = c4[1];
            chdr[indx+2] = c4[2];
            chdr[indx+3] = c4[3];
        }
    }
    else
    {
        #pragma omp simd
        for (int i=0; i<npts; ++i)
        {
            f4 = static_cast<float> (pImpl->mData[i]);
            int indx = 632 + i*4;
            chdr[indx]   = c4[3];
            chdr[indx+1] = c4[2];
            chdr[indx+2] = c4[1];
            chdr[indx+3] = c4[0];
        }
    }
    // Write it 
    std::ofstream outfile(fileName,
                          std::ofstream::binary | std::ofstream::trunc);
    outfile.write(chdr.data(), nbytes);
    outfile.close();
}

void Waveform::setHeader(const Double variableName, const double value)
{
    pImpl->mHeader.setHeader(variableName, value);
}

void Waveform::setHeader(const Integer variableName, const int value)
{
    if (variableName == Integer::NPTS)
    {
        throw std::invalid_argument("NPTS cannot be set with this function");
    }
    pImpl->mHeader.setHeader(variableName, value);
}

void Waveform::setHeader(const Logical variableName, const bool value) noexcept
{
    pImpl->mHeader.setHeader(variableName, value);
}

void Waveform::setHeader(const Character variableName,
                         const std::string &value) noexcept
{
    pImpl->mHeader.setHeader(variableName, value);
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
