#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <fstream>
#include <string>
#include <vector>
#include "temblor/library/utilities/time.hpp"
#include "temblor/library/dataReaders/miniseed/sncl.hpp"
#include "temblor/library/dataReaders/miniseed/trace.hpp"
#include "temblor/library/dataReaders/miniseed/enums.hpp"
#include <gtest/gtest.h>

namespace
{

std::vector<int>
loadIntegerData(const std::string &textFileName, const int npts);

using namespace Temblor::Library::DataReaders;

TEST(LibraryDataReadersMiniSEED, SNCL)
{
    MiniSEED::SNCL sncl;
    ASSERT_TRUE(sncl.isEmpty());
    ASSERT_EQ(sncl.getMaximumNetworkLength(), 10);
    ASSERT_EQ(sncl.getMaximumStationLength(), 10);
    ASSERT_EQ(sncl.getMaximumChannelLength(), 10);
    ASSERT_EQ(sncl.getMaximumLocationCodeLength(), 10);
    // Set some sncls
    std::string network = "UU";
    std::string station = "DUG";
    std::string channel = "HHZ";
    std::string location = "--";
    sncl.setNetwork(network);
    ASSERT_STREQ(sncl.getNetwork().c_str(), network.c_str()); 
    sncl.setStation(station);
    ASSERT_STREQ(sncl.getStation().c_str(), station.c_str());
    sncl.setChannel(channel);
    ASSERT_STREQ(sncl.getChannel().c_str(), channel.c_str());
    sncl.setLocationCode(location);
    ASSERT_STREQ(sncl.getLocationCode().c_str(), location.c_str());
    EXPECT_FALSE(sncl.isEmpty());
    // Test copy constructor
    MiniSEED::SNCL snclCopy(sncl);
    ASSERT_STREQ(snclCopy.getNetwork().c_str(), network.c_str()); 
    ASSERT_STREQ(snclCopy.getStation().c_str(), station.c_str());
    ASSERT_STREQ(snclCopy.getChannel().c_str(), channel.c_str());
    ASSERT_STREQ(snclCopy.getLocationCode().c_str(), location.c_str());
    // Try to overfill buffers
    std::string networkTooBig  = "12345678910UU";
    std::string stationTooBig  = "12345678910DUG";
    std::string channelTooBig  = "12345678910HHZ";
    std::string locationTooBig = "12345678910--";
    sncl.setNetwork(networkTooBig);
    sncl.setStation(stationTooBig);
    sncl.setChannel(channelTooBig);
    sncl.setLocationCode(locationTooBig);
    ASSERT_STREQ(sncl.getNetwork().c_str(), "1234567891");
    ASSERT_STREQ(sncl.getStation().c_str(), "1234567891");
    ASSERT_STREQ(sncl.getChannel().c_str(), "1234567891");
    ASSERT_STREQ(sncl.getLocationCode().c_str(), "1234567891");
}

TEST(LibraryDataReadersMiniSEED, Trace)
{
    MiniSEED::SNCL sncl;
    MiniSEED::Trace trace;
    std::string fileName = "data/WY.YWB.EHZ.01.mseed";
    std::string wyFileName = "data/WY.YWB.EHZ.01.txt";
    sncl.setNetwork("WY");
    sncl.setStation("YWB");
    sncl.setChannel("HHZ");
    sncl.setLocationCode("01");
    // Load the reference data file
    std::vector<int> referenceSignal = loadIntegerData(wyFileName, 14609);
    EXPECT_EQ(referenceSignal.size(), 14609); 
/*
    std::string fileName = "data/test.mseed";
    sncl.setNetwork("IU");
    sncl.setStation("COLA");
    sncl.setChannel("LH1");
    sncl.setLocationCode("00");
*/
    // Load the file
    EXPECT_NO_THROW(trace.read(fileName, sncl));
    // Check the trace statistics are correct
    EXPECT_NEAR(trace.getSamplingRate(), 100, 1.e-10);
    EXPECT_EQ(trace.getPrecision(), MiniSEED::Precision::INT32);
    EXPECT_EQ(trace.getNumberOfSamples(), 14609);
    Temblor::Library::Utilities::Time startTime = trace.getStartTime();
    EXPECT_EQ(startTime.getYear(), 2016);
    EXPECT_EQ(startTime.getMonth(), 1);
    EXPECT_EQ(startTime.getDayOfMonth(), 14);
    EXPECT_EQ(startTime.getHour(), 3); 
    EXPECT_EQ(startTime.getMinute(), 36);
    EXPECT_EQ(startTime.getSecond(), 33);
    EXPECT_EQ(startTime.getMicroSecond(), 340000);
    // Check the SNCL is correct
    MiniSEED::SNCL snclCheck = trace.getSNCL();
    ASSERT_STREQ(snclCheck.getNetwork().c_str(), "WY");
    ASSERT_STREQ(snclCheck.getStation().c_str(), "YWB");
    ASSERT_STREQ(snclCheck.getChannel().c_str(), "HHZ");
    ASSERT_STREQ(snclCheck.getLocationCode().c_str(), "01"); 
    // Check the time series is correct
    const int *data = trace.getDataPointer32i();
    int idmax = 0;
    for (int i=0; i<trace.getNumberOfSamples(); ++i)
    {
        idmax = std::max(idmax, std::abs(data[i] - referenceSignal[i]));
    }
    EXPECT_EQ(idmax, 0);
}

std::vector<int>
loadIntegerData(const std::string &textFileName, const int npts)
{
    std::ifstream signalFile(textFileName);
    std::string line;
    std::vector<int> referenceSignal;
    referenceSignal.reserve(npts);
    while (std::getline(signalFile, line))
    {   
        int sample;
        std::sscanf(line.c_str(), "%d\n", &sample);
        referenceSignal.push_back(sample);
    }   
    signalFile.close();
    return referenceSignal;
}

}