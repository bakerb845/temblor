#ifndef TEMBLOR_UTILITIES_LEAPSECONDS_HPP
#define TEMBLOR_UTILITIES_LEAPSECONDS_HPP
#include<memory>

namespace Temblor::Utilities
{
/*!
 * @class LeapSeconds leapSeconds.hpp "temblor/utilities/leapSeconds.hpp"
 * @brief Computes the number of leap seconds.
 * @copyright Ben Baker (University of Utah) distributed under the MIT license.
 */
class LeapSeconds
{
public:
    /*! @name Constructors
     */
    /*!
     * @brief Constructor
     */
    LeapSeconds();
    /*!
     * @brief Copy constructor.
     * @param[in] leapSeconds  The leap seconds class from which to initialize
     *                         this class.
     */
    LeapSeconds(const LeapSeconds &leapSeconds);
    /*!
     * @brief Move constructor.
     * @param[in,out] leapSeconds  The leap seconds from which to initialize
     *                             this class.  On exit, leapSeconds behavior
     *                             is undefined.
     */
    LeapSeconds(LeapSeconds &&leapSeconds) noexcept;
    /*! @} */

    /*! @name Operators
     * @{
     */
    /*!
     * @brief Copy assignment operator.
     * @param[in] leapSeconds  The class to copy to this.
     * @result A deep copy of the leap seconds class.
     */
    LeapSeconds& operator=(const LeapSeconds &leapSeconds);
    /*!
     * @brief Move assignment operator.
     * @param[in,out] leapSeconds  On input leapSeconds memory will be moved
     *                             to this.  On exit, leapSeconds's behavior
     *                             is undefined.
     * @result The memory from leapSeconds moved to this.
     */
    LeapSeconds& operator=(LeapSeconds &&leapSeconds) noexcept;
    /*! @} */

    /*! @name Destructors
     * @{
     */
    /*!
     * @brief Destructor.
     */
    ~LeapSeconds();
    /*! @} */

    /*!
     * @brief Gets the number of leap seconds.
     * @param[in] epoch        The epochal time in UTC seconds.
     * @param[in] lSubtract10  Since an epochal time of 0 would return 10 leap 
     *                         seconds this option provides the opportunity
     *                         to remove these first 10 leap seconds.
     * @result The number of leap seconds.
     */
    int getNumberOfLeapSeconds(double epoch, bool lSubtract10 = false) const;
private:
    class LeapSecondsImpl;
    std::unique_ptr<LeapSecondsImpl> pImpl;
};
};

#endif
