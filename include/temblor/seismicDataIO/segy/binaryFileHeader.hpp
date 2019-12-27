#ifndef TEMBLOR_SEISMICDATAIO_SEGY_BINARYFILEHEADER_HPP
#define TEMBLOR_SEISMICDATAIO_SEGY_BINARYFILEHEADER_HPP
#include <memory>

namespace Temblor::SeismicDataIO::SEGY
{

/*! 
 * @brief Defines the data format in the SEGY file.
 * @bug Only 3 out of 16 dataformats are supported.  I can add more on request.
 */
enum class DataFormat: std::int16_t
{   
    IBM_FLOAT = 1,   /*!< 4-byte IBM floating point. */
    IEEE_FLOAT = 5,  /*!< 4-byte IEEE floating point (i.e., a float). */
    IEEE_DOUBLE = 6, /*!< 8-byte IEEE floating point (i.e., a double). */
};
/*!
 * @brief Defines the type of ensemble.
 */
enum class TraceSortingCode: std::int16_t
{
    UNKNOWN = 0,                   /*!< Unknown. */
    AS_REOCORDED = 1,              /*!< No implied sorting. */
    COMMON_DEPTH_POINT = 2,        /*!< Common depth point ensemble. */   
    SINGLE_CONTINUOUS_PROFILE = 3, /*!< Single continuous profile. */
    HORIZONTALLY_STACKED = 4,      /*!< Horizontally stacked. */
    COMMON_SOURCE_POINT = 5,       /*!< Common source point. */
    COMMON_RECEIVER_POINT = 6,     /*!< Common receiver point. */
    COMMON_OFFSET_POINT = 7,       /*!< Common offset point. */
    COMMON_MID_POINT = 8,          /*!< Common midpoint. */
    COMMON_CONVERSION_POINT = 9    /*!< Common conversion point .*/
};
/*!
 * @brief Defines the sweep type.
 */
enum class SweepType: std::int16_t
{
    UNKNOWN = 0,     /*!< Unknown - this is not defined in the SEGY header. */
    LINEAR = 1,      /*!< Linear sweep type. */
    PARABOLIC = 2,   /*!< Parabolic sweep type. */
    EXPONENTIAL = 3, /*!< Exponential sweep type. */
    OTHER = 4        /*!< Other sweep type. */
};
/*!
 * @brief Defines the taper on the data.
 */
enum class TaperType: std::int16_t
{
    UNKNOWN = 0,        /*!< Unknown - this is not defined in the SEGY header .*/
    LINEAR = 1,         /*!< Linear taper. */
    COSINE_SQUARED = 2, /*!< \f$ \cos^2 \f$ taper. */
    OTHER = 3           /*!< A different type of taper. */
};
/*!
 * @brief Correlated trace code.
 */
enum class CorrelatedTraceCode: std::int16_t
{
    UNKNOWN = 0, /*!< Unknown - this is not defined in the SEGY header. */
    NO = 1,      /*!< The traces are uncorrelated. */
    YES = 2      /*!< The traces are correlated. */
};
/*!
 * @brief Binary gain recovered code.
 */
enum class BinaryGainRecoveredCode
{
    UNKNOWN = 0, /*!< Unknown - this is not defined in teh SEGY header. */
    YES = 1,     /*!< Binary gain recovered. */
    NO = 2       /*!< Binary gain is not recovered. */
};
/*!
 * @brief Measurement system.
 */
enum class MeasurementSystem: std::int16_t
{
    UNKNOWN = 0, /*!< Unknown - this is not defined in the SEGY header. */
    METERS = 1,  /*!< Meters. */
    FEET = 2     /*!< Feet. */
};
/*!
 * @brief Defines the amplitude recoever method.
 */
enum class AmplitudeRecoverMethod: std::int16_t
{
    UNKNOWN = 0,                /*!< Unknown - this is not defined in the SEGY
                                     header. */
    NONE = 1,                   /*!< No amplitude recovery method. */
    SPHERICAL_DIVERGENCE = 2,   /*!< Spherical divergence. */
    AUTOMATIC_GAIN_CONTROL = 3, /*!< Automatic gain control. */
    OTHER = 4                   /*!< Other amplitude recovery method. */
};
/*!
 * @brief Defines the impulse polarity convention.
 */
enum class ImpulseSignalPolarity: std::int16_t
{
    UNKNOWN = 0,         /*!< Unknown - this is not defined in the SEGY
                              header. */
    UPWARD_NEGATIVE = 1, /*!< Increase in pressure or upward geophone movement
                              corresponds to a negative number on the trace. */
    UPWARD_POSITIVE  = 2 /*!< Increase in pressure or upward geophone movement
                              corresponds to a positive number on the trace. */
};
/*!
 * @brief The time basis code.
 */
enum class TimeBasisCode
{
    UNKNOWN = 0, /*!< Unknown - this is not defined in the SEGY header. */
    LOCAL = 1,   /*!< Local time. */
    GMT = 2,     /*!< Greenwich Mean Time. */
    OTHER = 3,   /*!< Should be defined in user stanza in Extended Textual
                      File Header. */
    UTC = 4,     /*!< Coordinate Universal Time. */
    GPS = 5      /*!< Global Positioning System Time. */
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
    BinaryFileHeader(uint16_t majorVersion = 2,
                     uint16_t minorVersion = 0);
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
    void setJobIdentificationNumber(uint32_t jobid) noexcept;
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
    void setLineNumber(uint32_t lineNumber) noexcept;
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
     * @note This is mandatory for prestack data.
     */
    void setReelNumber(uint32_t reelNumber) noexcept;
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
     * @note This is mandatory for prestack data.
     */
    void setNumberOfTracesPerEnsemble(uint16_t nTracesPerEnsemble) noexcept;
    /*!
     * @copydoc setNumberOfTracesPerEnsemble
     */
    void setNumberOfTracesPerEnsemble(uint32_t nTracesPerEnsemble) noexcept;
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
    void setSampleInterval(uint16_t sampleInterval);
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
    void setNumberOfSamplesPerTrace(uint16_t samplesPerTrace);
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

    /*! @name Ensemble Fold
     * @{
     */
    /*!
     * @brief Sets the number of data traces per trace ensemble.
     *        For example, the CMP fold.
     * @param[in] nTracesPerFold  The number of traces per ensemble fold.
     */
    void setNumberOfTracesPerEnsembleFold(uint16_t nTracesPerFold);
    /*!
     * @brief Gets the number of data traces per ensemble.
     * @result The number of traces per the ensemble fold, e.g., the number
     *         of traces in the CMP fold.
     */
    uint16_t getNumberOfTracesPerEnsembleFold() const;
    /*! @} */

    /*! @name Trace Sorting Code
     * @{
     */
    /*!
     * @brief Sets the trace sorting code (i.e., type of ensemble).
     * @param[in] traceSortingCode  The trace sorting code.
     */
    void setTraceSortingCode(const TraceSortingCode traceSortingCode) noexcept;
    /*!
     * @brief Gets the trace sorting code.
     * @result The trace sorting code.
     */
    TraceSortingCode getTraceSortingCode() const noexcept;
    /*! @} */

    /*! @name Vertical Sum Code
     * @{
     */
    /*!
     * @brief Sets the vertical sum code.  
     * @param[in] verticalSumCode  The vertical sum code where (1) is no sum,
     *                             (2) is two sum, ..., N = M-1 sum where
     *                             M is in the range [2,32767].
     */
    void setVerticalSumCode(uint16_t verticalSumCode);
    /*!
     * @brief Gets the vertical sum code.
     * @result The vertical sum code.
     */
    uint16_t getVerticalSumCode() const noexcept;
    /*! @} */

    /*! @name Sweep Frequency
     * @{
     */
    /*!
     * @brief Sets the sweep frequency at start.
     * @param[in] sweepFrequency  The sweep frequency in Hz at the start.
     *                            0 indicates the value is unknown.
     */
    void setStartingSweepFrequency(int16_t sweepFrequency) noexcept;
    /*!
     * @brief Gets the sweep frequency at the start.
     * @result The sweep frequency at the end.  0 indicates the value
     *         is unknown. 
     */
    int16_t getStartingSweepFrequency() const noexcept;
    /*! 
     * @brief Sets the sweep frequency at end.
     * @param[in] sweepFrequency  The sweep frequency in Hz at the end.
     *                            0 indicates the value is unknown.
     */
    void setEndingSweepFrequency(int16_t sweepFrequency) noexcept;
    /*!
     * @brief Gets the sweep frequency at the end.
     * @result The sweep frequency at the end.  0 indicates the value
     *         is unknown. 
     */
    int16_t getEndingSweepFrequency() const noexcept;
    /*! @} */

    /*! @name Sweep Length
     * @{
     */
    /*!
     * @brief Sets the sweep length.
     * @param[in] sweepLength  The sweep length in ms.
     */
    void setSweepLength(uint16_t sweepLength);
    /*!
     * @brief Gets the sweep length.
     * @result The sweep length in ms.
     */
    uint16_t getSweepLength() const noexcept;
    /*! @} */

    /*! @name Time Basis Code
     * @{
     */

    /*! @} */
private:
    class BinaryFileHeaderImpl;
    std::unique_ptr<BinaryFileHeaderImpl> pImpl;
};

}
#endif
