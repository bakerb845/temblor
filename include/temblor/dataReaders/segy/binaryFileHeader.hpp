#ifndef TEMBLOR_DATAREADERS_SEGY_BINARYFILEHEADER_HPP
#define TEMBLOR_DATAREADERS_SEGY_BINARYFILEHEADER_HPP
#include <memory>

namespace Temblor::DataReaders::SEGY
{

/*! 
 * @brief Defines the data format in the SEGY file.
 * @bug Only 3 out of 16 dataformats are supported.  I can add more on request.
 */
enum class DataFormat
{   
    IBM_FLOAT = 1,   /*!< 4-byte IBM floating point. */
    IEEE_FLOAT = 5,  /*!< 4-byte IEEE floating point (i.e., a float). */
    IEEE_DOUBLE = 6, /*!< 8-byte IEEE floating point (i.e., a double). */
};

/*!
 * @class BinaryFileHeader "binaryFileHeader.hpp" "temblor/library/dataReaders/segy/binaryFileHeader.hpp"
 * @brief Defines the 400-byte binary file header for the SEGY data.
 * @copyright Ben Baker (University of Utah) distributed under the MIT license.
 */
class BinaryFileHeader
{
public:
    /*! @name Constructors
     * @{
     */
    /*! 
     * @brief Default constructor for the binary file header.
     * @param[in] majorVersion  The major version of the SEGY binary header.
     * @param[in] minorVersion  The minor version of the SEGY binary header.
     */
    BinaryFileHeader(const uint16_t majorVersion = 2,
                     const uint16_t minorVersion = 0);
    /*!
     * @brief Copy constructor.
     * @param[in] header   The binary file header from which to initialize
     *                     this class.
     */
    BinaryFileHeader(const BinaryFileHeader &header);
    /*!
     * @brief Move constructor.
     * @param[in,out] header  Header's memory will be moved to this class for
     *                        initialization.
     *                        Header's behavior will be undefined on exit.
     */
    BinaryFileHeader(BinaryFileHeader &&header) noexcept;
    /*! @} */

    /*! @name Operators
     * @{
     */
    /*!
     * @brief Copy assignment operator.
     * @param[in] header  The binary file header to copy.
     * @result A deep copy of header.
     */
    BinaryFileHeader& operator=(const BinaryFileHeader &header);
    /*!
     * @brief Move assignment operator.
     * @param[in,out] header  Header's memory will be moved to the result.
     *                        On exit header's behavior is undefined.
     * @result Contains the moved memory from header.
     */
    BinaryFileHeader& operator=(BinaryFileHeader &&header) noexcept;
    /*! @} */

    /*! @name Destructors
     * @{
     */
    /*! 
     * @brief The destructor.
     */
    ~BinaryFileHeader();
    
    /*! 
     * @brief Sets the binary file header from data read from disk.
     * @param[in] header  The header variable information to set on the class.
     * @throws std::invalid_argument if the header is invalid.
     */
    void setBinaryHeader(const char header[400]);
    /*!
     * @brief Packs a binary file header to write to disk.
     * @param[out] header  The binary header data to write to disk.
     * @throws std::runtime_error if the file header is malformed.
     */
    void getBinaryHeader(char header[400]) const;

    /*! @name Job Identification Number
     * @{
     */
    /*! 
     * @brief Sets the job identifcation number.
     * @param[in] jobid  The job identification number.  If unknown then set
     *                   this to 0.
     */
    void setJobIdentificationNumber(const uint32_t jobid) noexcept;
    /*!
     * @brief Gets the job identification number.
     * @result The job identification number.  0 indicates an undefined value.
     */
    uint32_t getJobIdentificationNumber() const noexcept;
    /*! @} */

    /*! @name Line Number
     * @{
     */
    /*!
     * @brief Sets the line number.  For 3D poststack data this will typically
     *        be the in-line number.
     * @param[in] lineNumber  The line number.
     */
    void setLineNumber(const uint32_t lineNumber) noexcept;
    /*!
     * @brief Gets the line number.
     * @result The line number.  0 indicates an undefined value.
     */
    uint32_t getLineNumber() const noexcept;
    /*! @} */

    /*! @name Reel Number
     * @{
     */
    /*!
     * @brief Sets the reel number.
     * @param[in] reelNumber  The reel number.  If unknown then set this to 0.
     */
    void setReelNumber(const uint32_t reelNumber) noexcept;
    /*!
     * @brief Gets the reel number.
     * @result The reel number.  0 indicates an undefined value.
     */
    uint32_t getReelNumber() const noexcept;
    /*! @} */

    /*! @name Number Of Traces Per Enemble
     * @{
     */
    /*!
     * @brief Sets the number of traces per ensemble.
     * @param[in] ntracesPerEnsemble  The number of traces per ensemble.
     *                                This is mandatory for prestack data.
     */
    void setNumberOfTracesPerEnsemble(const uint16_t nTracesPerEnsemble) noexcept;
    void setNumberOfTracesPerEnsemble(const uint32_t nTracesPerEnsemble) noexcept;
    /*!
     * @brief Gets the number of traces per ensemble.
     * @result The number of traces per ensemble.  0 indicates unknown.
     *         If the context is prestack data then 0 may indicate a problem.
     */
    uint32_t getNumberOfTracesPerEnsemble() const noexcept;
    /*! @} */

    /*! @name Sample Interval
     * @{
     */
    /*!
     * @brief Sets the sampling interval.
     * @param[in] sampleInterval  For time-series data this is in microseconds.
     *                            For frequency data this is in Hertz.
     *                            For spatial data this is in meters or feet.
     * @throws std::invalid_argument if this is not positive.
     * @note This should be for the primary set of seismic data traces
     *       in the file.
     */
    void setSampleInterval(const uint16_t sampleInterval);
    /*!
     * @brief Gets the sampling interval. 
     * @result The sampling interval.
     * @throws std::runtime_argument if this was not set.
     */
    uint16_t getSampleInterval() const;
    /*! @} */

    /*! @name Number Of Samples Per Trace
     * @{
     */
    /*!
     * @brief Sets the number of samples per trace.
     * @param[in] samplesPerTrace  The number of samples per trace. 
     * @throws std::invalid_argument if this is not positive.
     * @note This should be for the primary set of seismic data traces
     *       in the file.
     */
    void setNumberOfSamplesPerTrace(const uint16_t samplesPerTrace);
    /*!
     * @brief Gets the number of samples per trace.
     * @result The number of samples per trace.
     * @throws std::runtime_error if this was not set.
     */
    uint32_t getNumberOfSamplesPerTrace() const;
    /*! @} */


    /*! @name Data Format
     * @{
     */
    /*!
     * @brief Sets the data format.
     * @param[in] dataFormat  The data format.
     */
    void setDataFormat(const DataFormat dataFormat) noexcept;
    /*!
     * @brief Gets the data format.
     * @result The data format.
     * @throws std::runtime_error if the data format was not set.
     */
    DataFormat getDataFormat() const;
    /*! @} */

    /*! @name Sweep Frequency
     * @{
     */
    /*!
     * @brief Sets the sweep frequency at start.
     * @param[in] sweepFrequency  The sweep frequency in Hz at the start.
     *                            0 indicates the value is unknown.
     */
    void setStartingSweepFrequency(const uint16_t sweepFrequency) noexcept;
    /*!
     * @brief Gets the sweep frequency at the start.
     * @result The sweep frequency at the end.  0 indicates the value
     *         is unknown. 
     */
    uint16_t getStartingSweepFrequency() const noexcept;
    /*! 
     * @brief Sets the sweep frequency at end.
     * @param[in] sweepFrequency  The sweep frequency in Hz at the end.
     *                            0 indicates the value is unknown.
     */
    void setEndingSweepFrequency(const uint16_t sweepFrequency) noexcept;
    /*!
     * @brief Gets the sweep frequency at the end.
     * @result The sweep frequency at the end.  0 indicates the value
     *         is unknown. 
     */
    uint16_t getEndingSweepFrequency() const noexcept;
    /*! @} */

private:
    class BinaryFileHeaderImpl;
    std::unique_ptr<BinaryFileHeaderImpl> pImpl;
};

}
#endif
