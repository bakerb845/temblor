#ifndef TEMBLOR_UTILITIES_TIME_HPP
#define TEMBLOR_UTILITIES_TIME_HPP
#include <memory>

namespace Temblor::Utilities
{

/*!
 * @class Time time.hpp "temblor/utilities/time.hpp"
 * @brief A class for managing calendar and epochal time.
 * @author Ben Baker
 */
class Time
{
public:
    /*! @name Constructors
     * @{
     */
    /*!
     * @brief Default constructor which initializes epochal time to the
     *        epoch, i.e., January 01, 1970.
     */
    Time();
    /*!
     * @brief Copy constructor.
     * @param[in] time  The class from which to initialize this class.
     */
    Time(const Time &time); 
    /*!
     * @brief Move constructor.
     * @param[in,out] time  The class from which to initialize.  On exit
     *                      time's behavior will be undefined.
     */
    Time(Time &&time) noexcept;
    /*!
     * @brief Constructs an Earthworm time class from an epochal time.
     * @param[in] epoch  The number of seconds since the epoch (Jan 1, 1970)
     *                   in UTC for which the date will be initialized.
     */ 
    explicit Time(const double epoch);
    /*! @} */

    /*!
     * @name Operators
     * @{
     */
    /*!
     * @brief Assignment operator.
     * @param[in] time  The class to copy.
     * @result A deep copy of the input class.
     */
    Time& operator=(const Time &time);
    /*!
     * @brief Move assignment operator.
     * @param[in,out] time  The class to move.  On exit time's behavior
     *                      will be undefined.
     * @result The input class.
     */
    Time& operator=(Time &&time) noexcept;
    /*!
     * @brief Equality operator.
     * @result True indicates that the given time 
     *         equals the time represented by this class.
     */
    bool operator==(const Time &time);
    /*!
     * @brief Inequality operator.
     * @param[in] time  Class to test for inequality.
     * @result True indicates that the given time
     *         does not equal the time represented by this class.
     */
    bool operator!=(const Time &time);
    /*!
     * @brief Greater than operator.
     * @param[in] time  Class to test.
     * @result True indicates that *this > time.
     */
    bool operator>(const Time &time);
    /*!
     * Less than operator.
     * @param[in] time  Class to test.
     * @result True indicates that *this < time.
     */
    bool operator<(const Time &time);
    /*! @} */

    /*! @name Destructors
     * @{
     */
    /*!
     * @brief Default destructor.
     */
    ~Time();
    /*!
     * Resets the time on the class to January 01, 1970.
     */
    void clear() noexcept;
    /*! @} */

    /*!
     * @name Epochal Time Setters/Getters
     * @{
     */
    /*!
     * @brief Sets the epochal time.
     * @param[in] epoch  The number of seconds since the epoch (Jan 1, 1970)
     *                   in UTC for which the date will be initialized.
     */
    void setEpochalTime(const double epoch);
    /*!
     * @brief Gets the epochal time (seconds) correpsonding to the time
     *        set in the class.
     * @result The UTC epochal time in seconds since the epoch.
     * @throws std::invalid_argument if the time is improperly set.
     */
    double getEpochalTime() const;
    /*! @} */

    /*!
     * @name Year
     * @{
     */
    /*!
     * @brief Sets the year.
     * @param[in] year  The 4-digit year to set.  This should be greater
     *                  than 1900.
     * @throws std::invalid_argument if year is less than 1900.
     */
    void setYear(const int year);
    /*!
     * @brief Gets the year corresponding to the time set in the class.
     * @result The 4-digit year.
     */
    int getYear() const noexcept;
    /*! @} */

    /*!
     * @name Julian Day
     * @{
     */
    /*!
     * @brief Sets the day of the year.
     * @note This will invalidate the calendar day set with 
     *       \c setMonth() and \c setDayOfMonth().
     * @param[in] jday  The Julian day of the year to set.  This is in the
     *                  range of [1,366].  Note, additional checking for
     *                  leap years will not be performed.
     * @throws std::invalid_argument if jday is out of range.
     */
    void setJulianDay(const int jday);
    /*!
     * @brief Gets the Julian day corresponding to the time set in
     *        the class.
     * @result The Julian day.  This is in the range [1,366] where 366
     *         accounts for leap years.
     */
    int getJulianDay() const noexcept;
    /*! @} */

    /*!
     * @name Calendar Day
     * @{
     */
    /*!
     * @brief Sets the months.
     * @note When using this setter the day of the month must be set with 
     *       \c setDayOfMonth().  This will invalidate the current Julian day.
     * @param[in] month  The month to set.  This is in the range [1,12].
     * @throws std::invalid_argument if month is out of range.
     */
    void setMonth(const int month);
    /*!
     * @brief Gets the month corresponding to the time set in the class.
     * @result The month.  This is in the range [1,12].
     */
    int getMonth() const noexcept;
    /*!
     * @brief Sets the day of the month.
     * @note  When using this setter the month must also be set with
     *        \c setMonthDayOfMonth().  This will invalidate the current
     *        Julian day.
     * @param[in] dom  The day of the month to set.  This must be in the
     *                 range [1,31].  Further checking for the correct
     *                 number of days in the month will not be performed.
     * @throws std::invalid_argument if dom is out of bounds.
     */
    void setDayOfMonth(const int dom);
    /*!
     * @brief Gets the day of the month corresponding to the time set
     *        in the class.
     * @result The day of the month.  This is in the range [1,31].
     */
    int getDayOfMonth() const noexcept;
    /*! @} */

    /*!
     * @name Hour
     * @{
     */
    /*!
     * @brief Sets the hour.
     * @param[in] hour  The hour to set.  This is in the range [0,23].
     * @throws std::invalid_argument if hour is out of range.
     */
    void setHour(const int hour);
    /*!
     * @brief Gets the hour corresponding to the time set in the class.
     * @result The time of the day.  This is in the range [0,23].
     */
    int getHour() const noexcept;
    /*! @} */

    /*!
     * @name Minute
     * @{
     */
    /*!
     * @brief Sets the minute.
     * @param[in] minute  The minute to set.  This is in the range [0,59].
     * @throws std::invalid_argument if minute is out of range.
     */
    void setMinute(const int minute);
    /*!
     * @brief Gets the minute corresponding to the time set in the class.
     * @result The minute of the hour.  This is in the range [0,59].
     */
    int getMinute() const noexcept;
    /*! @} */

    /*!
     * @name Second
     * @{
     */
    /*!
     * @brief The integer part of the second to set.
     * @param[in] second  The integral second to set.  This is in the
     *                    range [0,59].
     * @throws std::invalid_argument if second is out of range.
     */
    void setSecond(const int second);
    /*!
     * @brief Gets the second corresponding to the time set in the class.
     * @result The second.  This is in the range [0,59].
     */
    int getSecond() const noexcept;
    /*! @} */

    /*!
     * @name Microsecond
     * @{
     */
    /*!
     * @brief Sets the microsecond.  
     * @param[in] musec  The microsecond to set.  This must be in the range
     *                   [0,999999].
     * @throws std::invalid_argument if microsecond is negative or too large.
     */
    void setMicroSecond(const int musec);
    /*!
     * @brief Gets the microsecond corresonding to the time set in the class.
     * @result The microsecond.
     */
    int getMicroSecond() const noexcept;
    /*!
     * @} 
    */

    /*!
     * @brief Swaps two time classes.
     * @param[in,out] lhs  Class to exchange with rhs.
     * @param[in,out] rhs  Class to exchange with lhs.
     */
    friend void swap(Time &lhs, Time &rhs);
private:
     class TimeImpl;
     std::unique_ptr<TimeImpl> pImpl;
}; // End class

/*!
 *  @brief Swaps two time classes, lhs and rhs.
 *  @param[in,out] lhs  On exit this will contain the information in rhs.
 *  @parma[in,ouut rhs  On exit this will contain the information in lhs.
 */
void swap(Time &lhs, Time &rhs);

}

#endif
