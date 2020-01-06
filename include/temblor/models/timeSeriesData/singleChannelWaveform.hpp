#ifndef TEMBLOR_MODELS_TIMESERIESDATA_SINGLECHANNELWAVEFORM_HPP
#define TEMBLOR_MODELS_TIMESERIESDATA_SINGLECHANNELWAVEFORM_HPP 1
#include <memory>
#include "temblor/seismicDataIO/fileFormats.hpp"

// Forward declarations
namespace Temblor::Utilities
{
class Time;
}

namespace Temblor::Models::TimeSeriesData
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

    /*! @name Sampling Rate 
     * @{
     */
    /*!
     * @brief Checks if the sampling rate was set.
     * @result True indicates that the sampling rate was set.
     */
    bool haveSamplingRate() const noexcept;
    /*! 
     * @brief Sets the sampling rate.
     * @param[in] samplingRate  The sampling rate in Hertz.
     * @throws std::invalid_argument if the sampling rate is not positive.
     */
    void setSamplingRate(const double samplingRate);
    /*! 
     * @brief Gets the sampling rate.
     * @result The sampling rate in Hz.
     * @throws std::runtime_error if the sampling rate was not set.
     */
    double getSamplingRate() const;
    /*!
     * @brief Gets the Nyquist frequency.
     * @result The Nyquist frequency in Hz.
     * @throws std::runtime_error if the sampling rate was not set.
     */
    double getNyquistFrequency() const;
    /*! 
     * @brief Gets the sampling period.
     * @result The sampling period in seconds.
     * @throws std::runtime_error if the sampling rate was not set.
     */
    double getSamplingPeriod() const;
    /*! @} */

    /*! @name Time
     * @{
     */
    /*!
     * @brief Gets the trace start time.
     * @result The trace epochal start time in seconds (UTC) from the epoch.
     */
    double getEpochalStartTime() const noexcept;
    /*!
     * @brief Gets the trace end time.
     * @result The trace epochal end time in seconds (UTC) from the epoch.
     */
    double getEpochalEndTime() const noexcept;
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
    /*!
     * @brief Sets the waveform comment, e.g., 'filtered'.
     * @param[in] comment  The waveform comment.
     * @note This can be useful when appending to a waveform gather where
     *       unique waveform identifiers are required.
     */
    void setComment(const std::string &comment) noexcept;
    /*!
     * @brief Gets the waveform comment.
     * @result The waveform comment.
     */
    std::string getComment() const noexcept; 
    /*!
     * @brief Gets the waveform identifier.
     * @result The waveform identifier encapsulating the SNCL and comment.
     */
    size_t getWaveformIdentifier() const noexcept;
    /*! @} */
 
    /*! @name File Input/Output
     * @{
     */
    /*!
     * @brief Creates a single channel waveform from a SAC file.
     * @param[in] sacFileName  The name of the SAC file to read.
     * @throws std::invalid_argument if the SAC file does not exist or
     *         is malformed.
     */
    void readSAC(const std::string &sacFileName);
    /*!
     * @brief Reads a single channel waveform from a miniSEED file.
     * @param[in] mseedFileName  The name of the miniSEED file.
     * @param[in] waveID         The waveform identifier to query. 
     */
    
    /*!
     * @brief Writes a single channel waveform.
     * @param[in] fileName   The name of the file to write.
     * @param[in] format     The file format to write.
     * @throws std::runtime_error if the file can't be written or there is
     *         no data to write.
     */
    void write(const std::string fileName,
               const Temblor::DataReaders::FileFormatTypes format =
                     Temblor::DataReaders::FileFormatTypes::SAC) const;
    /*! @} */

    /*! @name Custom Header Information
     * @{ 
     */
    /*!
     * @brief Sets an integer header value.
     * @param[in] name   The name of the header variable.
     * @param[in] value  The value of the header variable.
     */
    void setIntegerHeader(const std::string &name, const int value);
    /*!
     * @param[in] name   The name of the header variable.
     * @result The value corresponding to the integer header variable.
     * @throws std::invalid_argument if the header variable does not exist.
     */
    int getIntegerHeader(const std::string &name) const;
    /*!
     * @brief Determines if the header variable exists.
     */
    bool integerHeaderVariableExists(const std::string &name) const noexcept;
    /*! @} */
     
private:
    class SingleChannelWaveformImpl;
    std::unique_ptr<SingleChannelWaveformImpl> pImpl; 
};

}
#endif
