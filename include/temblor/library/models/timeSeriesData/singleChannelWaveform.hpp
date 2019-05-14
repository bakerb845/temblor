#ifndef TEMBLOR_LIBRARY_MODELS_TIMESERIESDATA_SINGLECHANNELWAVEFORM_HPP
#define TEMBLOR_LIBRARY_MODELS_TIMESERIESDATA_SINGLECHANNELWAVEFORM_HPP
#include <memory>

// Forward declarations
namespace Temblor::Utilities
{
class Time;
}

namespace Temblor::Library::Models::TimeSeriesData
{
/*!
 * @class SingleChannelWaveform singleChannelWaveform.hpp "temblor/library/models/timeSeriesData/singleChannelWaveform.hpp"
 * @brief A single channel waveform is the most fundamental type of time series
 *        data and forms the building block of higher-level data structures
 *        suches as multi-channel waveforms and gathers.
 * @copyright Ben Baker
 */
class SingleChannelWaveform
{
public:
    /*! @name Constructors
     * @{
     */
    /*!
     * @brief Default constructor.
     */
    SingleChannelWaveform();
    /*!
     * @brief Copy constructor.
     * @param[in] waveform  Single channel waveform from which to initialize
     *                      this class.
     */
    SingleChannelWaveform(const SingleChannelWaveform &waveform);
    /*!
     * @brief Move constructor.
     * @param[in,out] waveform  Single channel waveform whose memory is to be
     *                          moved to this class.
     *                          On exit, waveform's behavior is undefined.
     */
    SingleChannelWaveform(SingleChannelWaveform &&waveform);
    /*! @} */

    /*! @name Operators
     * @{
     */
    /*!
     * @brief Copy assignment operator.
     * @param[in] waveform  Single channel waveform class to copy.
     * @result A deep copy of the waveform.
     */
    SingleChannelWaveform &operator=(const SingleChannelWaveform &waveform); 
    /*!
     * @brief Move assignment operator.
     * @param[in,out] waveform  Single channel waveform whose memory is moved 
     *                          to the left-hand side.
     *                          On exit waveform's behavior is undefined.
     */
    SingleChannelWaveform &operator=(const SingleChannelWaveform &&waveform);
    /*! @} */

    /*! @name Destructors
     * @{
     */
    /*!
     * @brief Destructor.
     */
    ~SingleChannelWaveform();
    /*!
     * @brief Clears all memory on the waveform and resets variables.
     */ 
    void clear() noexcept;
    /*! @} */

    /*!
     * @brief Sets the waveform data.
     * @param[in] nSamples  The number of samples in the waveform.
     *                      This cannot be negative.
     * @param[in] data      The time series data.  This is an array whose
     *                      dimension is [nSamples].
     * @throws std::invalid_argument if nSamples is positive and data is NULL
     *         or if the number of samples is negative.
     * @note The time series is a continuous sequence.  To impune data in a 
     *       gap the user should set the value to a nan.
     */
    void setData(const int nSamples, const double data[]);
    /*!
     * @brief Gets a pointer to the time series data.
     * @result A pointer to the time series data.  This can be NULL if the data
     *         was not set.  The length of the array can be determined with
     *         \c getNumberOfSamples().
     */
    const double *getTimeSeriesDataPointer() const noexcept;
    /*!
     * @brief Gets a copy of the time series data.
     * @result A copy of the time series data in the class.
     */
    std::vector<double> getTimeSeriesData() const noexcept;
    /*!
     * @brief Gets the number of samples in the time series.
     * @result The number of samples in teh time series. 
     */
    int getNumberOfSamples() const noexcept;

    /*! @name Sampling Period
     * @{
     */
    /*!
     * @brief Sets the sampling period.
     * @param[in] samplingPeriod  The sampling period in seconds.
     * @throws std::invalid_argument if the sampling period  or sampling rate
     *         was not positive.
     */
    void setSamplingPeriod(const double samplingPeriod);
    /*!
     * @brief Gets the sampling period.
     * @result The sampling period in seconds.
     * @throws std::runtime_error if the sampling period is not set.
     */
    double getSamplingPeriod() const;
    /*! 
     * @brief Sets the sampling rate.
     * @param[in] samplingRate  The sampling rate in Hertz.
     * @throws std::invalid_argument if the sampling rate is not positive.
     */
    void setSamplingRate(const double samplingRate);
    /*! 
     * @brief Gets the sampling rate.
     * @result The sampling rate in Hz.
     * @throws std::runtime_error if the sampling period or sampling rate
     *         was not set.
     */
    double getSamplingRate() const;
    /*! @} */

    /*! @Name Network, Station, Channel, Location Code Naming
     * @{
     */ 
    /*! 
     * @brief Sets the network name.
     * @param[in] networkName  The name of the network to set.
     */
    void setNetworkName(const std::string &networkName) noexcept;
    /*!
     * @brief Gets the network name.
     * @result The network name.
     */
    std::string getNetworkName() const noexcept;
    /*! 
     * @brief Sets the station name.
     * @param[in] stationName  The name of the station to set.
     */
    void setStationName(const std::string &stationName) noexcept;
    /*! 
     * @brief Gets the station name.
     * @result The station name.
     */
    std::string getStationName() const noexcept;
    /*!
     * @brief Sets the channel name.
     * @param[in] channelName  The name of the channel to set.
     */
    void setChannelName(const std::string &channelName) noexcept;
    /*!
     * @brief Gets the channel name.
     * @result The channel name.
     */
    std::string getChannelName() const noexcept;
    /*! 
     * @brief Gets the location code.
     * @param[in] locationCode  The location code to set.
     */
    void setLocationCode(const std::string &locationCode) noexcept;
    /*! 
     * @brief Gets the location code.
     * @result The location code.
     */
    std::string getLocationCode() const noexcept;
    /*! @} */
 
private:
    class SingleChannelWaveformImpl;
    std::unique_ptr<SingleChannelWaveformImpl> pImpl; 
};

}
#endif
