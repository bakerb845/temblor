#include <ctime>
#include <chrono>
#include <cstring>
#include <string>
#include <cmath>
#include <mutex>
#include <algorithm>
#include <stdexcept>
#include "temblor/library/utilities/time.hpp"

using namespace Temblor::Library::Utilities;

#define CALENDAR_2_EPOCH 0
#define EPOCH_2_CALENDAR 1
std::mutex timeMutex;

static void calendar2epoch(
    const int mode, const double etime,
    const int year, const int jday, const int month, const int dom,
    const int hour, const int minute, const double second,
    const bool luseJday,
    int &yearOut,   int &jdayOut,  int &monthOut,
    int &domOut,    int &hourOut,  int &minuteOut,
    int &secondOut, int &musecOut,
    double &epochOut);

class  Time::TimeImpl
{
public:
    /// Epochal time; Seconds since Jan 1, 1970 (UTC)
    double epoch = 0;
    /// Year
    int year = 1970;
    /// Julian day [1,366]
    int jday = 1;
    /// Month [1,12]
    int month = 1;
    /// Day of month [1,31]
    int dom = 1;
    /// Hour of day [0,23]
    int hour = 0;
    /// Minute [0,59]
    int minute = 0;
    /// Second [0,59] 
    int second = 0;
    /// Microsecond
    int musec = 0;
    /// Flag for lazy evaluation
    bool lhaveEpoch = false;
    /// Use the Julian day
    bool luseJday = true;
};

Time::Time() :
    pImpl(std::make_unique<TimeImpl>())
{
}

Time::~Time() = default;

void Time::clear() noexcept
{
    pImpl->epoch = 0;
    pImpl->year = 1970;
    pImpl->jday = 1;
    pImpl->month = 1;
    pImpl->dom = 1;
    pImpl->hour = 0;
    pImpl->minute = 0;
    pImpl->second = 0;
    pImpl->musec = 0;
    pImpl->lhaveEpoch = false;
    pImpl->luseJday = true;
}

Time::Time(const double epoch) :
    pImpl(std::make_unique<TimeImpl>())
{
    setEpochalTime(epoch); 
}

Time::Time(const Time &time)
{
    *this = time;
}

Time::Time(Time &&time) noexcept
{
    *this = std::move(time);
}

Time& Time::operator=(const Time &time)
{
    if (&time == this){return *this;}
    pImpl = std::make_unique<TimeImpl> (*time.pImpl); // use default copy
    return *this;
}

Time& Time::operator=(Time &&time) noexcept
{
    if (&time == this){return *this;}
    pImpl = std::move(time.pImpl);
    return *this;
}

bool Time::operator==(const Time &time)
{
    if (pImpl->year   != time.getYear()){return false;}
    if (pImpl->month  != time.getMonth()){return false;}
    if (pImpl->jday   != time.getJulianDay()){return false;}
    if (pImpl->dom    != time.getDayOfMonth()){return false;}
    if (pImpl->hour   != time.getHour()){return false;}
    if (pImpl->minute != time.getMinute()){return false;}
    if (pImpl->second != time.getSecond()){return false;}
    return pImpl->musec == time.getMicroSecond();
    // Floating point comparisons are tricky.  Particularly because a lazy
    // user may be content with using Jan 1, 1970.  This gives a massive
    // range on the number of important digits in epoch.  Thus, I don't want
    // to compare it.  I can skip the lazy evaluation equalities as it
    // will be returned on request.
}

bool Time::operator!=(const Time &time)
{
    return !(*this == time);
}

bool Time::operator>(const Time &time)
{
    return pImpl->epoch > time.getEpochalTime();
}

bool Time::operator<(const Time &time)
{
    return pImpl->epoch < time.getEpochalTime();
}

void Time::setEpochalTime(const double epoch)
{
    calendar2epoch(EPOCH_2_CALENDAR,
                   epoch,
                   0, 0, 0, 0, //year, jday, month,  day,
                   0, 0, 0,    //hour, minute, second,
                   false,      //luseJday,
                   pImpl->year,   pImpl->jday,  pImpl->month,
                   pImpl->dom,    pImpl->hour,  pImpl->minute,
                   pImpl->second, pImpl->musec, pImpl->epoch);
    pImpl->lhaveEpoch = true;
}

double Time::getEpochalTime() const
{
    if (!pImpl->lhaveEpoch)
    {
        constexpr double etime = 0;
        int year   = pImpl->year;
        int jday   = pImpl->jday;
        int month  = pImpl->month;
        int dom    = pImpl->dom;
        int hour   = pImpl->hour;
        int minute = pImpl->minute;
        int isec   = pImpl->second;
        int musec  = pImpl->musec;
        bool luseJday = pImpl->luseJday;
        if (luseJday)
        {
            month = 0;
            dom = 0;
        }
        else
        {
            jday = 0;
        }
        double second = static_cast<double> (isec) 
                      + static_cast<double> (musec)*1.e-6;
        calendar2epoch(CALENDAR_2_EPOCH,
                       etime,
                       year, jday, month,  dom,
                       hour, minute, second,
                       luseJday,
                       pImpl->year,   pImpl->jday,  pImpl->month,
                       pImpl->dom,    pImpl->hour,  pImpl->minute,
                       pImpl->second, pImpl->musec, pImpl->epoch);
        pImpl->lhaveEpoch = true;
    }
    return static_cast<double> (pImpl->epoch);
}

void Time::setYear(const int year)
{
    if (year < 1900)
    {
        throw std::invalid_argument("year = " + std::to_string(year)
                                   + " must be at least 1900");
    }
    pImpl->year = year;
    pImpl->lhaveEpoch = false;
}

int Time::getYear() const noexcept
{
    return pImpl->year;
}

void Time::setJulianDay(const int jday)
{
    if (jday < 1 || jday > 366)
    {
        throw std::invalid_argument("jday = " + std::to_string(jday)
                                   + " must be in range [1,366]");
    }
    pImpl->jday = jday;
    pImpl->luseJday = true;
    pImpl->lhaveEpoch = false;
}

int Time::getJulianDay() const noexcept
{
    return pImpl->jday;
}

void Time::setMonth(const int month)
{
    if (month < 1 || month > 12)
    {
        throw std::invalid_argument("month = " + std::to_string(month)
                                   + " must be in range [1,12]");
    }
    pImpl->month = month;
    pImpl->luseJday = false;
    pImpl->lhaveEpoch = false;
}

int Time::getMonth() const noexcept
{
    return pImpl->month;
}

void Time::setDayOfMonth(const int dom)
{
    if (dom < 1 || dom > 31)
    {
        throw std::invalid_argument("dom = " + std::to_string(dom)
                                   + " must be in range [1,31]");
    }
    pImpl->dom = dom;
    pImpl->luseJday = false;
    pImpl->lhaveEpoch = false;
}

int Time::getDayOfMonth() const noexcept
{
    return pImpl->dom;
}

void Time::setHour(const int hour)
{
    if (hour < 0 || hour > 23)
    {
        throw std::invalid_argument("hour = " + std::to_string(hour)
                                   + " must be in range [0,23]");
    }
    pImpl->hour = hour;
    pImpl->lhaveEpoch = false;
}

int Time::getHour() const noexcept
{
    return pImpl->hour;
}

void Time::setMinute(const int minute)
{
    if (minute < 0 || minute > 59)
    {
        throw std::invalid_argument("minute = " + std::to_string(minute)
                                   + " must be in range [0,59]");
    }
    pImpl->minute = minute;
    pImpl->lhaveEpoch = false;
}

int Time::getMinute() const noexcept
{
    return pImpl->minute;
}

void Time::setSecond(const int second)
{
    if (second < 0 || second > 59)
    {
        throw std::invalid_argument("second = " + std::to_string(second)
                                   + " must be in range [0,59]");
    }
    pImpl->second = second;
    pImpl->lhaveEpoch = false;
}

int Time::getSecond() const noexcept
{
    return pImpl->second;
}

void Time::setMicroSecond(const int musec)
{
    if (musec < 0 || musec > 999999)
    {
        throw std::invalid_argument("musec = " + std::to_string(musec)
                                   + " must be in range [0,999999]");
    }
    pImpl->musec = musec;
    pImpl->lhaveEpoch = false;
}

int Time::getMicroSecond() const noexcept
{
    return pImpl->musec;
}

void Temblor::Library::Utilities::swap(Time &lhs, Time &rhs)
{
    std::swap(lhs.pImpl, rhs.pImpl);
}

void calendar2epoch(
    const int mode, const double etime,
    const int year, const int jday, const int month, const int dom,
    const int hour, const int minute, const double second,
    const bool luseJday,
    int &yearOut,   int &jdayOut,  int &monthOut,
    int &domOut,    int &hourOut,  int &minuteOut,
    int &secondOut, int &musecOut,
    double &epochOut)
{
    // Time functions aren't thread safe so lock this function
    std::lock_guard<std::mutex> guard(timeMutex);
    if (mode == CALENDAR_2_EPOCH)
    {
        // Proceed as normal 
        int isec = static_cast<int> (second);
        double frac = second - static_cast<double> (isec);
        // Get the GMT time
        std::tm t0{};
        std::tm t{};
        memset(&t, 0, sizeof(std::tm));
        t.tm_year = year - 1900;
        t.tm_hour = hour;
        if (luseJday)
        {
            t.tm_mday = jday - 1;  // C ignores yday
        }
        else
        {
            t.tm_mon  = month - 1; // Month of year [0,11]
            t.tm_mday = dom - 1;   // Day of month [0,30]
        }
        t.tm_min  = minute; 
        t.tm_sec  = isec;
        memset(&t0, 0, sizeof(std::tm));
        t0.tm_year = 70; // 1970
        double epoch = difftime(timegm(&t), timegm(&t0));
        epoch = epoch + frac;
        // Copy result
        yearOut   = year;
        jdayOut   = t.tm_yday + 2;
        monthOut  = month;
        domOut    = dom;
        hourOut   = hour;
        minuteOut = minute;
        secondOut = isec;
        musecOut  = std::lround(frac*1.e6); //static_cast<int> (frac*1.e6 + 0.5);
        epochOut  = epoch;
    }
    else
    {
        auto t(static_cast<time_t>(etime)); // = static_cast<time_t> (etime);
        struct std::tm *tmptr = gmtime(&t);
        if (tmptr == nullptr)
        {
            throw std::invalid_argument("gmtime failed");
        }
        yearOut   = static_cast<int> (tmptr->tm_year) + 1900;
        jdayOut   = static_cast<int> (tmptr->tm_yday) + 1;
        monthOut  = static_cast<int> (tmptr->tm_mon) + 1;
        domOut    = static_cast<int> (tmptr->tm_mday);
        hourOut   = static_cast<int> (tmptr->tm_hour);
        minuteOut = static_cast<int> (tmptr->tm_min);
        secondOut = static_cast<int> (tmptr->tm_sec);
        auto epochIFix = static_cast<double> (static_cast<uint64_t> (etime));
        musecOut  = std::lround ((etime - epochIFix)*1.e6);
        epochOut  = etime;
    }
}
