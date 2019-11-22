#ifndef TEMBLOR_DATAREADERS_MINISEED_TRACE_HPP
#define TEMBLOR_DATAREADERS_MINISEED_TRACE_HPP 1
#include <memory>
#include "temblor/dataReaders/miniseed/enums.hpp"

namespace Temblor::Utilities
{
class Time;
}

namespace Temblor::DataReaders::MiniSEED
{
class SNCL;
/*!
 * @brief Defines a miniSEED trace.
 */
class Trace
{
public:
    /*! @name Constructors
     * @{
     */
    /*!
     * @brief Constructor.
     */
    Trace();
    /*!
     * @brief Copy constructor.
     * @param[in] trace  The trace class from which to initialize this class.
     */
    Trace(const Trace &trace);
    /*!
     * @brief Move constructor.
     * @param[in,out] trace  The trace to initialize from.  On exit, trace's
     *                       behavior is undefined.
     */
    Trace(Trace &&trace) noexcept; 
    /*! @} */

    /*! @name Operators
     * @{
     */
    /*!
     * @brief Copy operator.
     * @param[in] trace  The trace to copy.
     * @result A deep copy of trace.
     */
    Trace& operator=(const Trace &trace);
    /*!
     * @brief Move assignment operator.
     * @param[in,out] trace  The trace whose memory is to be moved to this.
     *                       On exit, trace's behavior is undefined.
     */
    Trace& operator=(Trace &&trace) noexcept;
    /*! @} */

    /*! @name Destructors
     * @{
     */
    /*!
     * @brief Destructor.
     */
    ~Trace();
    /*!
     * @brief Releases memory on the class and resets all variables.
     */
    void clear() noexcept;
    /*! @} */

    /*! @name File IO
     * @{
     */
    /*!
     * @brief Reads a trace with a given SNCL from a miniSEED file.
     * @param[in] fileName  The name of the miniSEED file to read.
     * @param[in] sncl      The SNCL to read.
     * @throws std::invalid_argument if the file does not exist, cannot be read,
     *         or does not contain the given SNCL.
     */
    void read(const std::string &fileName, const SNCL &sncl);

    /*! @} */

    /*! @name Start Time and End Time
     * @{
     */
    /*!
     * @brief Sets the start time of the trace.
     * param[in] startTime  The trace start time.
     */
    void setStartTime(Temblor::Utilities::Time &startTime) noexcept;
    /*!
     * @brief Gets the start time of the trace.
     * @result The start time of the trace.
     * @note If this was not set then it will correspond to January 1, 1970.
     */
    Temblor::Utilities::Time getStartTime() const noexcept;
    /*!
     * @brief Gets the end time of the trace.
     * @result The end time of the trace.
     * @throws std::runtime_error if the sampling rate was not set.
     * @note This is computed relative to the start time.  Hence, the time 
     *       series data and sampling rate must have been set.
     * @sa \c setSamplingRate().
     */
    Temblor::Utilities::Time getEndTime() const;
    /*! @} */

    /*! @name Sampling Rate
     * @{
     */
    /*!
     * @brief Sets the sampling rate.
     * @param[in] samplingRate  The sampling rate in Hz.
     * @throws std::invalid_argument if sampling rate is not positive.
     */
    void setSamplingRate(const double samplingRate);
    /*!
     * @brief Gets the sampling rate.
     * @result The sampling rate in Hz.
     * @throws std::runtime_error if the sampling rate was not set.
     */
    double getSamplingRate() const;    
    /*! !} */

    /*! @name Number of Samples
     * @{
     */
    /*!
     * @brief Sets the number of samples.
     * @result The number of samples in the trace.
     */
    int getNumberOfSamples() const noexcept;
    /*! @} */

    /*! @name Precision
     * @{
     */
    /*!
     * @brief Defines the precision of the underlying time series data.
     * @throws std::runtime_error if a time series was never set or read
     *         from disk.
     */
    Precision getPrecision() const;
    /*! @} */

    /*! @name SNCL
     * @{
     */
    /*!
     * @brief Sets the station, network, channel, and location code.
     * @param[in] sncl  The SNCL to set.
     * @throws std::invalid_argument if all the elements of the SNCL are 
     *         undefined.
     */
    void setSNCL(const SNCL &sncl);
    /*!
     * @brief Gets the station, network, channel, and location code.
     * @result The SNCL.
     * @throws std::runtime_error if this was not set.
     */
    SNCL getSNCL() const;
    /*! @} */

    /*! @name Time Series Data
     * @{
     */
    /*!
     * @brief Gets teh time series data.
     * @param[out] x    A vector containing the time series data.
     * @throws std::runtime_error if the time series data was never set
     *         or read from disk.
     */
    std::vector<double> getData64f() const;
    std::vector<float>  getData32f() const;
    std::vector<int>    getData32i() const;
    /*!
     * @brief Gets the time series data.
     * @param[in] length    The length of the output array.  This must
     *                      be at least \c getNumberOfSamples().
     * @param[out] x        The time series data.  This is an array of
     *                      dimension [length] however only the first
     *                      \c getNumberOfSamples() are valid. 
     * @throws std::invalid_argument if length is invalid or x is NULL.
     * @throws std::runtime_error if the time series data was never set
     *         or read from disk.
     * @sa \c getNumberOfSamples()
     */ 
    void getData(const int length, double *x[]) const;
    void getData(const int length, float *x[]) const;
    void getData(const int length, int *x[]) const;
    /*!
     * @brief Sets the time series data.
     * @param[in] nSamples  The number of samples in the signal.
     *                      This cannot exceed INT_MAX for the time being.
     * @param[in] x         The time series data.  If nSamples is positive
     *                      then this cannot be NULL.
     * @throws std::invalid_argument if nSamples is too big or if nSamples is
     *         positive and x is NULL.
     */
    void setData(const size_t nSamples, const double x[]);
    void setData(const size_t nSamples, const float x[]);
    void setData(const size_t nSamples, const int x[]);
    /*!
     * @brief Gets a pointer to the double precision time series.
     * @result A pointer to the time series data.
     * @throws std::runtime_error if the underlying precision is not a double,
     *         or the time series data was never set or the trace was
     *         never read from disk.
     * @sa \c getNumberOfSamples()
     * @sa \c getPrecision()
     */
    const double *getDataPointer64f() const;
    /*!
     * @brief Gets a poitner to the float precision time series data.
     * @result A pointer to the time series data.
     * @throws std::runtime_error if the underlying precision is not a float,
     *         or the time series data was never set or the trace was
     *         never read from disk.
     * @sa \c getNumberOfSamples()
     * @sa \c getPrecision()
     */
    const float *getDataPointer32f() const;
    /*! 
     * @brief Gets a poitner to the integer precision time series data.
     * @result A pointer to the time series data.
     * @throws std::runtime_error if the underlying precision is not an integer,
     *         (int32_t), or the time series data was never set or the trace
     *         was never read from disk.
     * @sa \c getNumberOfSamples()
     * @sa \c getPrecision()
     */
    const int *getDataPointer32i() const;
    /*! @} */
private:
    class TraceImpl;
    std::unique_ptr<TraceImpl> pImpl;
};
}
#endif
