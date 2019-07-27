#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <string>
#include "temblor/models/timeSeriesData/waveformIdentifier.hpp"
#include <gtest/gtest.h>

namespace
{
using namespace Temblor::Models::TimeSeriesData;

TEST(LibraryModels, WaveformIdentifier)
{
    std::string network = "uU"; // Will convert 'u' to U
    std::string station = "LHUT";
    std::string channel = "BHZ";
    std::string location = "00";
    std::string comment = "raw data";
    WaveformIdentifier waveID(network, station, channel, location, comment);
    network = "UU"; // Need to capitalize
    ASSERT_STREQ(waveID.getNetworkName().c_str(), network.c_str()); 
    ASSERT_STREQ(waveID.getStationName().c_str(), station.c_str());
    ASSERT_STREQ(waveID.getChannelName().c_str(), channel.c_str());
    ASSERT_STREQ(waveID.getLocationCode().c_str(), location.c_str());
    ASSERT_STREQ(waveID.getComment().c_str(), comment.c_str()); // won't capitalize
    // Test copy constructor
    WaveformIdentifier waveIDCopy(waveID);
    ASSERT_STREQ(waveIDCopy.getNetworkName().c_str(), network.c_str());
    ASSERT_STREQ(waveIDCopy.getStationName().c_str(), station.c_str());
    ASSERT_STREQ(waveIDCopy.getChannelName().c_str(), channel.c_str());
    ASSERT_STREQ(waveIDCopy.getLocationCode().c_str(), location.c_str());
    EXPECT_TRUE(waveIDCopy == waveID);
    // Change something about the waveID
    EXPECT_NO_THROW(waveID.setComment(""));
    EXPECT_FALSE(waveID == waveIDCopy);
}

}
