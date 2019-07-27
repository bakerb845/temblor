#ifndef TEMBLOR_MODELS_TIMESERIESDATA_WAVEFORMIDENTIFIER_HPP
#define TEMBLOR_MODELS_TIMESERIESDATA_WAVEFORMIDENTIFIER_HPP 1
#include <memory>

namespace Temblor::Models::TimeSeriesData
{

/*!
 * @brief Identifies a waveform by its network, station, channel, and,
 *        optionally location code and a comment.
 */ 
class WaveformIdentifier
{
public:
    /*! @name Constructors
     * @{
     */
    /*!
     * @brief Constructor
     */
    WaveformIdentifier();
    /*!
     * @brief Constructor.
     * @param[in] network   The network name.
     * @param[im] station   The station name.
     * @param[in] channel   The channel code.
     * @note The location code will be set to "--" and comment will be blank.
     */
    WaveformIdentifier(const std::string &network,
                       const std::string &station,
                       const std::string &channel) noexcept;
    /*!
     * @brief Constructor.
     * @param[in] network       The network name.
     * @param[im] station       The station name.
     * @param[in] channel       The channel code.
     * @param[in] locationCode  The location code.
     * @note The comment will be blank.
     */
    WaveformIdentifier(const std::string &network,
                       const std::string &station,
                       const std::string &channel,
                       const std::string &locationCode) noexcept;
    /*!
     * @brief Constructor.
     * @param[in] network       The network name.
     * @param[im] station       The station name.
     * @param[in] channel       The channel code.
     * @param[in] locationCode  The location code.
     * @param[in] comment       A waveform comment.
     */
    WaveformIdentifier(const std::string &network,
                       const std::string &station,
                       const std::string &channel,
                       const std::string &locationCode,
                       const std::string &comment) noexcept;
    /*!
     * @brief Copy constructor.
     * @param[in] id   The waveform identifier.
     */
    WaveformIdentifier(const WaveformIdentifier &id);
    /*!
     * @brief Move constructor.
     * @param[in,out] id  The waveform ID.  On exit, id's behavior is undefined.
     */
    WaveformIdentifier(WaveformIdentifier &&id) noexcept;
    /*! @} */

    /*! @name Operators
     * @{
     */
    /*!
     * @brief Copy assignment operator.
     * @param[in] id   The waveform identifier to copy.
     * @result A deep copy of the waveform identifier.
     */
    WaveformIdentifier& operator=(const WaveformIdentifier &id);
    /*!
     * @brief Move assignment operator.
     * @param[in,out] id   The waveform identifier to move to the output.
     *                     On exit id's behavior is undefined.
     * @result The moved contents of id.
     */
    WaveformIdentifier& operator=(WaveformIdentifier &&id) noexcept;
    /*!
     * @brief Determines if the given waveform identifier equals this.
     * @param[in] rhs  The waveform identifier to compare to this.
     * @result True indicates that rhs equals this.
     */
    bool operator==(const WaveformIdentifier &rhs);
    /*!
     * @brief Determines if the given waveform identifier does not equal this.
     * @param[in] rhs  The waveform identifier to compare to this.
     * @result True indicates that rhs does not equal this.
     */
    bool operator!=(const WaveformIdentifier &rhs);
    /*! @} */

    /*! @name Destructors
     * @{
     */
    /*!
     * @brief Destructor.
     */
    ~WaveformIdentifier();
    /*!
     * @brief Clears all fields comprising the waveforms ID.
     */
    void clear() noexcept;
    /*! @} */

    /*! @name Network
     * @{
     */
    /*!
     * @brief Sets the network name.
     * @param[in] network  The name of the network.
     * @note This will be capitalized.
     */
    void setNetworkName(const std::string &network) noexcept;
    /*!
     * @brief Gets the network code.
     * @result The network code.
     */
    std::string getNetworkName() const noexcept;
    /*! @} */

    /*! @name Station
     * @{
     */
    /*!
     * @brief Sets the station name.
     * @param[in] station  The name of the station.
     * @note This will be capitalized.
     */
    void setStationName(const std::string &station) noexcept;
    /*!
     * @brief Gets the station name. 
     * @result The station name.
     */
    std::string getStationName() const noexcept;
    /*! @} */

    /*! @name Channel
     * @{
     */
    /*!
     * @brief Sets the channel name.
     * @param[in] channel  The name of the channel.
     * @note This will be capitalized.
     */
    void setChannelName(const std::string &station) noexcept;
    /*!
     * @brief Gets the channel name.
     * @result The channel name.
     */
    std::string getChannelName() const noexcept;
    /*! @} */

    /*! @name Location code
     * @{
     */
    /*!
     * @brief Sets the location code.
     * @param[in] locationCode  The location code.
     */
    void setLocationCode(const std::string &locationCode) noexcept;
    /*!
     * @brief Gets the location code.
     * @result The location code.
     */
    std::string getLocationCode() const noexcept;
    /*! @} */

    /*! @name Comment
     * @{
     */
    /*!
     * @brief Sets a comment about the waveform.
     * @param[in] comment   A waveform comment.
     */
    void setComment(const std::string &comment) noexcept;
    /*!
     * @brief Gets the waveform comment.
     * @result The waveform comment.
     */
    std::string getComment() const noexcept;
    /*! @} */

    /*!
     * @brief Gets the waveform identifier corresponding to the network, 
     *        station, channel, location code, and comment.
     * @note While this is created with a hash function it is not necessarily
     *       secure.
     */
    size_t getIdentifier() const noexcept;
    /*!
     * @brief Gets the string that will hashed.
     * @result The string that will be hashed.
     */
    std::string getHashString() const noexcept;

private:
    class WaveformIdentifierImpl;
    std::unique_ptr<WaveformIdentifierImpl> pImpl;
};
}
#endif
