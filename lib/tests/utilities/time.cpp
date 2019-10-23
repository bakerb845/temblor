#include <cstdio>
#include <cstdlib>
#include <climits>
#include "temblor/utilities/time.hpp"
#include "temblor/utilities/leapSeconds.hpp"
#include <gtest/gtest.h>

namespace
{
using namespace Temblor::Utilities;
//============================================================================//
//                                     Time                                   //
//============================================================================//
TEST(LibraryUtilitiesTime, EpochToCalendar)
{
    Time time(1408117832.844000);
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
    Time moveTime = std::move(time);
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
    Time time(1408117832.844000);
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
    Time time1(1408117832.844000);
    Time time1Ref = time1;
    Time time2(1408117834.123000);
    Time time2Ref = time2;
    swap(time1, time2);
    EXPECT_TRUE(time1 == time2Ref);
    EXPECT_TRUE(time2 == time1Ref);
}

TEST(LibraryUtilitiesTime, CalendarToEpoch)
{
    Time time;
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
    Time timej;
    timej.setYear(2016);
    timej.setJulianDay(102); // Add 1 for leap year
    timej.setHour(19);
    timej.setMinute(13);
    timej.setSecond(45);
    timej.setMicroSecond(255000); 
    // Test the copy constructor
    Time copyTime(timej);
    EXPECT_NEAR(1460402025.255, copyTime.getEpochalTime(), 1.e-4); 
}

TEST(LibraryUtilitiesTime, CompareTime)
{
    Time time1(1460402025.255);
    Time time2(1460402425.255);
    EXPECT_TRUE(time1 < time2);
    EXPECT_TRUE(time2 > time1);
    EXPECT_TRUE(time1 == time1);
    EXPECT_TRUE(time1 != time2);
}

TEST(LibraryUtilitiesTime, LeapSeconds)
{
    LeapSeconds ls;
    Time time;
    time.setYear(2019);
    time.setMonth(10);
    time.setDayOfMonth(23);
    time.setHour(15);
    time.setMinute(0);
    time.setSecond(0);
    time.setMicroSecond(0);
    int nls;
    // Jan 1, 1972 starts the list at 10
    nls = ls.getNumberOfLeapSeconds(time.getEpochalTime());
    EXPECT_EQ(nls, 37);
    EXPECT_EQ(ls.getNumberOfLeapSeconds(time.getEpochalTime(), true), 27);

    time.setYear(1969);
    nls = ls.getNumberOfLeapSeconds(time.getEpochalTime());
    EXPECT_EQ(nls, 10);
    EXPECT_EQ(ls.getNumberOfLeapSeconds(time.getEpochalTime(), true), 0);

    time.setYear(1972);
    time.setMonth(7);
    time.setDayOfMonth(1);
    time.setHour(0);
    time.setMinute(0);
    time.setSecond(0);
    time.setMicroSecond(0);
    nls = ls.getNumberOfLeapSeconds(time.getEpochalTime());
    EXPECT_EQ(nls, 11);
    EXPECT_EQ(ls.getNumberOfLeapSeconds(time.getEpochalTime(), true), 1);
    // Now try some edge cases - 36 starts at July 2015
    time.setYear(2015);
    time.setMonth(6);
    time.setDayOfMonth(30);
    time.setHour(23);
    time.setMinute(59);
    time.setSecond(59);
    time.setMicroSecond(0);
    nls = ls.getNumberOfLeapSeconds(time.getEpochalTime());
    EXPECT_EQ(nls, 35);
    EXPECT_EQ(ls.getNumberOfLeapSeconds(time.getEpochalTime(), true), 25);

    time.setYear(1996);
    time.setMonth(1);
    time.setDayOfMonth(1);
    time.setHour(0);
    time.setMinute(0);
    time.setSecond(0);
    time.setMicroSecond(0);
    nls = ls.getNumberOfLeapSeconds(time.getEpochalTime());
    EXPECT_EQ(nls, 30);
    EXPECT_EQ(ls.getNumberOfLeapSeconds(time.getEpochalTime(), true), 20);

    // October 1, 2012
    nls = ls.getNumberOfLeapSeconds(1349101514);
    EXPECT_EQ(nls, 35); 
    EXPECT_EQ(ls.getNumberOfLeapSeconds(1349101514, true), 25);
}

}
