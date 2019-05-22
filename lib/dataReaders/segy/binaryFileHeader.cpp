#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <string>
#include <algorithm>
#include "temblor/library/dataReaders/segy/binaryFileHeader.hpp"

using namespace Temblor::Library::DataReaders::SEGY;

namespace
{

inline uint16_t unpackUInt16t(const char c2[2], const bool lswap = false)
{
    union
    {
        char cpad2[2];
        uint16_t i2;
    };
    if (!lswap)
    {
        cpad2[0] = c2[0];
        cpad2[1] = c2[1];
    }
    else
    {
        cpad2[0] = c2[1];
        cpad2[1] = c2[0];
    }
    return i2;
}

inline uint32_t unpackUInt32t(const char c4[4], const bool lswap = false)
{
    union
    {
        char cpad4[4];
        uint32_t i4; 
    };
    if (!lswap)
    {
        cpad4[0] = c4[0];
        cpad4[1] = c4[1];
        cpad4[2] = c4[2];
        cpad4[3] = c4[3];
    }
    else
    {
        cpad4[0] = c4[3];
        cpad4[1] = c4[2];
        cpad4[2] = c4[1];
        cpad4[3] = c4[0];
    }
    return i4;
}

}

class BinaryFileHeader::BinaryFileHeaderImpl
{
public:
/*
    BinaryFileHeaderImpl(const BinaryFileHeaderImpl &header)
    {
        *this = header;
    }

    //BinaryFileHeaderImpl&
    //operator=(const BinaryFileHeaderImpl &header) = default;
*/
    uint32_t mJobID = 0;
    uint32_t mLineNumber = 0;
    uint32_t mReelNumber = 0;
    uint16_t mNumberOfTracesPerEnsemble = 0;
    uint16_t mSampleInterval = 0;
    uint16_t mNumberOfSamplesPerTrace = 0;
    uint16_t mNumberOfOriginalSamplesPerTrace = 0;
    uint16_t mDataFormat = 0;
    uint16_t mEnsembleFold = 0;
    uint16_t mStartingSweepFrequency = 0;
    uint16_t mEndingSweepFrequency = 0;
    uint16_t mVibratoryPolarityCode = 0;
    uint32_t mExtendedNumberOfTracesPerEnsemble = 0;
    uint32_t mExtendedNumberOfSamplesPerTrace = 0;
    uint32_t mExtendedOriginalNumberOfSamplesPerTrace = 0;
    uint32_t mExtendedEnsembleFold = 0;
    uint32_t mIntegerConstant = 169090601; // Works as a magic number
    uint32_t mUnassigned = 0;
    uint8_t mMajorRevision = 0;
    uint8_t mMinorRevision = 0;
    uint16_t mFixedTraceFlag = 0;
    uint16_t mNumberOfExtendedTextHeaders = 0;
    uint32_t mMaximumNumberOfExtraTraceHeaders = 0;
    uint16_t mTimeCode = 0;
    uint64_t mNumberOfTracesInFile = 0;
    uint64_t mOffset = 0;
    uint32_t mNumberOfTrailerStanzas = 0;
};

/// Constructor
BinaryFileHeader::BinaryFileHeader(const uint16_t majorRevision,
                                   const uint16_t minorRevision) :
    pImpl(std::make_unique<BinaryFileHeaderImpl> ())
{
    pImpl->mMajorRevision = majorRevision;
    pImpl->mMinorRevision = minorRevision;
}

BinaryFileHeader::BinaryFileHeader(const BinaryFileHeader &header)
{
    *this = header;
}

BinaryFileHeader::BinaryFileHeader(BinaryFileHeader &&header) noexcept
{
    *this = std::move(header);
}

/// Operators
BinaryFileHeader& BinaryFileHeader::operator=(const BinaryFileHeader &header)
{
    if (&header == this){return *this;}
    if (pImpl){pImpl.reset();}
    pImpl = std::make_unique<BinaryFileHeaderImpl> (*header.pImpl);
    return *this;
}

BinaryFileHeader& BinaryFileHeader::operator=(BinaryFileHeader &&header) noexcept
{
    if (&header == this){return *this;}
    pImpl = std::move(header.pImpl);
    return *this;
}

BinaryFileHeader::~BinaryFileHeader() = default;

/// Set/unpack a binary file header
void BinaryFileHeader::setBinaryHeader(const char header[400])
{
    if (header == nullptr)
    {
        throw std::invalid_argument("Header is NULL");
    }
    BinaryFileHeader hdr;
    uint32_t jobID       = unpackUInt32t(&header[0]);
    uint32_t lineNumber  = unpackUInt32t(&header[4]);
    uint32_t reelNumber  = unpackUInt32t(&header[8]);
    uint16_t nEnsemble   = unpackUInt16t(&header[10]);
    uint16_t nSampleInt  = unpackUInt16t(&header[12]);
    uint16_t nSamples    = unpackUInt16t(&header[14]);
    hdr.setJobIdentificationNumber(jobID);
    hdr.setLineNumber(lineNumber);
    hdr.setReelNumber(reelNumber);
    hdr.setNumberOfTracesPerEnsemble(nEnsemble);
    hdr.setSampleInterval(nSampleInt);
    hdr.setNumberOfSamplesPerTrace(nSamples);
    *this = hdr;
}

/// Job ID number
void BinaryFileHeader::setJobIdentificationNumber(const uint32_t jobid) noexcept
{
    pImpl->mJobID = jobid;
}
uint32_t BinaryFileHeader::getJobIdentificationNumber() const noexcept
{
    return pImpl->mJobID;
}

/// Line number
void BinaryFileHeader::setLineNumber(const uint32_t lineNumber) noexcept
{
    pImpl->mLineNumber = lineNumber;
}
uint32_t BinaryFileHeader::getLineNumber() const noexcept
{
    return pImpl->mLineNumber;
}

/// Reel number
void BinaryFileHeader::setReelNumber(const uint32_t reelNumber) noexcept
{
    pImpl->mReelNumber = reelNumber;
}
uint32_t BinaryFileHeader::getReelNumber() const noexcept
{
    return pImpl->mReelNumber;
}

/// Number of traces per ensemble
void BinaryFileHeader::setNumberOfTracesPerEnsemble(
    const uint16_t nTracesPerEnsemble) noexcept
{
    pImpl->mExtendedNumberOfTracesPerEnsemble = 0;
    pImpl->mNumberOfTracesPerEnsemble = nTracesPerEnsemble;
}
void BinaryFileHeader::setNumberOfTracesPerEnsemble(
    const uint32_t nTracesPerEnsemble) noexcept
{
    pImpl->mNumberOfTracesPerEnsemble = 0;
    pImpl->mExtendedNumberOfTracesPerEnsemble = nTracesPerEnsemble;
}

uint32_t BinaryFileHeader::getNumberOfTracesPerEnsemble() const noexcept
{
   auto ntemp = static_cast<uint32_t> (pImpl->mNumberOfTracesPerEnsemble);
   return std::max(ntemp, pImpl->mExtendedNumberOfTracesPerEnsemble);
}

/// Sampling interval
void BinaryFileHeader::setSampleInterval(const uint16_t sampleInterval)
{
    pImpl->mSampleInterval = sampleInterval;
}
uint16_t BinaryFileHeader::getSampleInterval() const
{
    return pImpl->mSampleInterval;
}

/// Number of samples per trace
void BinaryFileHeader::setNumberOfSamplesPerTrace(
    const uint16_t samplesPerTrace)
{
    pImpl->mExtendedNumberOfSamplesPerTrace = 0;
    pImpl->mNumberOfSamplesPerTrace = samplesPerTrace;
}
uint32_t BinaryFileHeader::getNumberOfSamplesPerTrace() const
{
    auto temp = static_cast<uint32_t> (pImpl->mNumberOfSamplesPerTrace);
    return std::max(temp, pImpl->mExtendedNumberOfSamplesPerTrace);
}

/// Data format
void BinaryFileHeader::setDataFormat(const DataFormat dataFormat) noexcept
{
    if (dataFormat == DataFormat::IEEE_FLOAT)
    {
        pImpl->mDataFormat = 5;
    }
    else if (dataFormat == DataFormat::IEEE_DOUBLE)
    {
        pImpl->mDataFormat = 6;
    }
    else if (dataFormat == DataFormat::IBM_FLOAT)
    {
        pImpl->mDataFormat = 1;
    }
}
DataFormat BinaryFileHeader::getDataFormat() const
{
    DataFormat dataFormat = DataFormat::IEEE_FLOAT;
    if (pImpl->mDataFormat == 5)
    {
        dataFormat = DataFormat::IEEE_FLOAT;
    }
    else if (pImpl->mDataFormat == 6)
    {
        dataFormat = DataFormat::IEEE_DOUBLE;
    }
    else if (pImpl->mDataFormat == 1)
    {   
        dataFormat = DataFormat::IBM_FLOAT;
    }
    else
    {
        throw std::runtime_error("Unsupported data format");
    }
    return dataFormat; 
}

/// Sweep frequencies
void BinaryFileHeader::setStartingSweepFrequency(
    const uint16_t sweepFrequency) noexcept
{
    pImpl->mStartingSweepFrequency = sweepFrequency;
}
uint16_t BinaryFileHeader::getStartingSweepFrequency() const noexcept
{
    return pImpl->mStartingSweepFrequency;
}
void BinaryFileHeader::setEndingSweepFrequency(
    const uint16_t sweepFrequency) noexcept
{
    pImpl->mEndingSweepFrequency = sweepFrequency;
}
uint16_t BinaryFileHeader::getEndingSweepFrequency() const noexcept
{
    return pImpl->mEndingSweepFrequency;
}

