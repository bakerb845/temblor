#include <cstdio>
#include <cstdlib>
#include <climits>
#include "temblor/library/utilities/time.hpp"
#include <gtest/gtest.h>

namespace
{
//============================================================================//
//                                     Time                                   //
//============================================================================//
TEST(LibraryUtilitiesTime, EpochToCalendar)
{
    Temblor::Library::Utilities::Time time(1408117832.844000);
    //time.setEpochalTime(1408117832.844000);
    EXPECT_EQ(2014, time.getYear());
    EXPECT_EQ(8, time.getMonth());
    EXPECT_EQ(227, time.getJulianDay());
    EXPECT_EQ(15, time.getDayOfMonth());
    EXPECT_EQ(15, time.getHour());
    EXPECT_EQ(50, time.getMinute());
    EXPECT_EQ(32, time.getSecond());
    EXPECT_EQ(844000, time.getMicroSecond());
    // Test the move constructor
    Temblor::Library::Utilities::Time moveTime = std::move(time);
    EXPECT_EQ(2014, moveTime.getYear());
    EXPECT_EQ(8, moveTime.getMonth());
    EXPECT_EQ(15, moveTime.getDayOfMonth());
    EXPECT_EQ(15, moveTime.getHour());
    EXPECT_EQ(50, moveTime.getMinute());
    EXPECT_EQ(32, moveTime.getSecond());
    EXPECT_EQ(844000, moveTime.getMicroSecond());
}

TEST(LibraryUtilitiesTime, clear)
{
    Temblor::Library::Utilities::Time time(1408117832.844000);
    time.clear();
    EXPECT_EQ(1970, time.getYear());
    EXPECT_EQ(1,    time.getJulianDay());
    EXPECT_EQ(1,    time.getMonth());
    EXPECT_EQ(1,    time.getDayOfMonth());
    EXPECT_EQ(0,    time.getHour());
    EXPECT_EQ(0,    time.getMinute());
    EXPECT_EQ(0,    time.getSecond());
    EXPECT_EQ(0,    time.getMicroSecond());
    EXPECT_NEAR(0,  time.getEpochalTime(), 1.e-4);
}

TEST(LibraryUtilitiesTime, swap)
{
    Temblor::Library::Utilities::Time time1(1408117832.844000);
    Temblor::Library::Utilities::Time time1Ref = time1;
    Temblor::Library::Utilities::Time time2(1408117834.123000);
    Temblor::Library::Utilities::Time time2Ref = time2;
    Temblor::Library::Utilities::swap(time1, time2);
    EXPECT_TRUE(time1 == time2Ref);
    EXPECT_TRUE(time2 == time1Ref);
}

TEST(LibraryUtilitiesTime, CalendarToEpoch)
{
    Temblor::Library::Utilities::Time time;
    // Build time from day of month
    time.setYear(2016);
    time.setMonth(4);
    time.setDayOfMonth(11);
    time.setHour(19);
    time.setMinute(13);
    time.setSecond(45);
    time.setMicroSecond(255000);
    EXPECT_NEAR(1460402025.255, time.getEpochalTime(), 1.e-4);
    // Build time from julian day
    Temblor::Library::Utilities::Time timej;
    timej.setYear(2016);
    timej.setJulianDay(102); // Add 1 for leap year
    timej.setHour(19);
    timej.setMinute(13);
    timej.setSecond(45);
    timej.setMicroSecond(255000); 
    // Test the copy constructor
    Temblor::Library::Utilities::Time copyTime(timej);
    EXPECT_NEAR(1460402025.255, copyTime.getEpochalTime(), 1.e-4); 
}

TEST(LibraryUtilitiesTime, CompareTime)
{
    Temblor::Library::Utilities::Time time1(1460402025.255);
    Temblor::Library::Utilities::Time time2(1460402425.255);
    EXPECT_TRUE(time1 < time2);
    EXPECT_TRUE(time2 > time1);
    EXPECT_TRUE(time1 == time1);
    EXPECT_TRUE(time1 != time2);
}
}
