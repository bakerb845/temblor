#ifndef TEMBLOR_DATAREADERS_MINISEED_SNCL_HPP
#define TEMBLOR_DATAREADERS_MINISEED_SNCL_HPP 1
#include <memory>

namespace Temblor::DataReaders::MiniSEED
{
/*!
 * @brief Defines a SEED station, network, channel, location name.
 */
class SNCL
{
public:
    /*! @name Constructors
     * @{
     */
    /*!
     * @brief Default constructor.
     */
    SNCL();
    /*!
     * @brief Copy constructor.
     * @param[in] sncl  The SNCL class from which to initialize.
     */
    SNCL(const SNCL &sncl);
    /*!
     * @brief Move constructor.
     * @param[in,out] sncl  The SNCL class to initialize from.  On exit
     *                      sncl's behavior is undefined.
     */
    SNCL(SNCL &&sncl) noexcept;
    /*! @} */

    /*! @name Operators
     * @{
     */
    /*!
     * @brief Copy assignment operator.
     * @param[in] sncl  SNCL to copy.
     * @result A deep copy of sncl.
     */
    SNCL& operator=(const SNCL &sncl);
    /*!
     * @brief Move assignment operator.
     * @param[in] sncl  SNCL whose memory will be moved to this.  On exit
     *                  sncl's behavior is undefined.
     */
    SNCL& operator=(SNCL &&sncl) noexcept;
    /*!
     * @brief Equality operator.
     * @result True indicates that the given SNCL 
     *         equals the SNCL represented by this class.
     */
    bool operator==(const SNCL &sncl) noexcept;
    /*!
     * @brief Inequality operator.
     * @param[in] time  Class to test for inequality.
     * @result True indicates that the given  SNCL
     *         does not equal the SNCL represented by this class.
     */
    bool operator!=(const SNCL &sncl) noexcept;
    /*! @} */

    /*! @name Destructors
     * @{
     */
    /*!
     * @brief Default destructor.
     */
    ~SNCL();
    /*!
     * @brief Clears the SNCL.
     */
    void clear() noexcept; 
    /*! @} */

    /*! @name Network
     * @{
     */
    /*!
     * @brief Sets the network name.  If the network name exceeds
     *        \c getMaximumNetworkength() then this will be truncated.
     * @param[in] name  The name of the network.
     */
    void setNetwork(const std::string &name) noexcept;
    /*!
     * @brief Gets the network name.
     * @result The name of the network.
     */
    std::string getNetwork() const noexcept;
    /*!
     * @brief Gets the maximum string length of a network name.
     * @result The maximum network string length.
     * @note A typical length is 2 but the file format can accomodate 10.
     */
    int getMaximumNetworkLength() const noexcept;
    /*! @} */

    /*! @name Station
     * @{
     */
    /*!
     * @brief Sets the station name.  If the station name exceeds
     *        \c getMaximumStationLength() then this will be truncated.
     * @param[in] name  The name of the station.
     */
    void setStation(const std::string &name) noexcept;
    /*!
     * @brief Gets the station name.
     * @result The name of the station.
     */
    std::string getStation() const noexcept;
    /*! 
     * @brief Gets the maximum string length of a station name.
     * @result The maximum station name string length.
     * @note A typical length is 3-5 but the file format can accomodate 10.
     */
    int getMaximumStationLength() const noexcept;
    /*! @} */

    /*! @name Channel
     * @{
     */
    /*!
     * @brief Sets the channel name.  If the channel name exceeds
     *        \c getMaximumChannelLength then this will be truncated.
     * @param[in] name  The name of the channel.
     */
    void setChannel(const std::string &name) noexcept;
    /*!
     * @brief Gets the channel name.
     * @result The name of the channel.
     */
    std::string getChannel() const noexcept;
    /*! 
     * @brief Gets the maximum string length of a channel name.
     * @result The maximum channel name string length.
     * @note A typical length is 3 but the file format can accomodate 10.
     */
    int getMaximumChannelLength() const noexcept;
    /*! @} */

    /*! @name Location Code
     * @{
     */
    /*!
     * @brief Sets the location code.  If the location code exceeds
     *        \c getMaximumLocationCodeLength() then this will be truncated.
     * @param[in] name  The name of the location code.
     */
    void setLocationCode(const std::string &name) noexcept;
    /*!
     * @brief Gets the location code.
     * @result The name of the location code.
     */
    std::string getLocationCode() const noexcept;
    /*! 
     * @brief Gets the maximum string length of a location code.
     * @result The maximum station name string length.
     * @note A typical length is 0 or 2 but the file format can accomodate 10.
     */
    int getMaximumLocationCodeLength() const noexcept;
    /*! @} */

    /*!
     * @brief Convenience routine to determine if the network, station,
     *        channel location are all empty.
     * @result True indicates that there is no SNCL information.
     *         False indicates that at least one of the SNCL elements is
     *         defined.
     */
    bool isEmpty() const noexcept;
private:
    class SNCLImpl;
    std::unique_ptr<SNCLImpl> pImpl;
};
}
#endif
