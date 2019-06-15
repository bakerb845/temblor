#ifndef TEMBLOR_LIBRARY_DATAREADERS_SAC_WAVEFORM_HPP
#define TEMBLOR_LIBRARY_DATAREADERS_SAC_WAVEFORM_HPP
#include <memory>
#include <string>
#include <vector>
#include "temblor/library/dataReaders/sac/enums.hpp"

namespace Temblor::Library::DataReaders::SAC
{

/*!
 * @brief The SAC waveform class.  A SAC waveform is comprised of a time series
 *        and is characterized by a SAC header.
 */
class Waveform
{
public:
    /*! @name Constructors
     * *@{
     */
    /*!
     * @brief Constructor.
     */
    Waveform();
    /*!
     * @brief Copy constructor.
     * @param[in] waveform  The waveform class from which to
     *                      construct this class.
     */
    Waveform(const Waveform &waveform);
    /*!
     * @brief Move constructor.
     * @param[in,out] waveform  The waveform class to move to this class.
     *                          On exit, waveform's behavior will be undefined.
     */
    Waveform(Waveform &&waveform) noexcept;
    /*! @} */

    /*! @name Operators
     * @{
     */
    /*!
     * @brief Copy assignment operator.
     * @param[in] waveform  The waveform class to copy.
     * @result A deep copy of the input waveform.
     */
    Waveform& operator=(const Waveform &waveform);
    /*!
     * @brief Move assignment operator.
     * @param[in,out] waveform  The waveform to move.  On exit waveform's
     *                          behavior will be undefined.
     */
    Waveform& operator=(Waveform &&waveform) noexcept;
    /*! @} */

    /*! @name Destructors
     * @{
     */
    /*!
     * @brief Destructor.
     */
    ~Waveform();
    /*!
     * @brief Resets the header and clears the time series.
     */
    void clear() noexcept;
    /*! @} */

    /*!
     * @brief Gets a float header variable.
     * @result The value of the header variable.  If the header
     *         variable has not been set then this will be -12345.
     */
    double getHeader(const Double variableName) const noexcept;
    /*!
     * @brief Sets a float header variable.
     * @param[in] variableName  The name of the header variable to set.
     * @param[in] value         The value of the header variable.
     * @throws std::invalid_argument if attempting to set Double::Delta
     *         with a negative value.
     */
    void setHeader(const Double variableName, const double value);
    /*! @} */

    /*! @name Integer Header Variables
     * @{
     */
    /*!
     * @brief Gets an integer header variable.
     * @result The value of the header variable.  If the header
     *         variable has not been set then this will be -12345.
     */
    int getHeader(const Integer variableName) const noexcept;
    /*! @brief Sets an integer header variable.
      * @param variableName  The name of the integer variable to set.
      *                      Note, that you cannot set Integer::NPTS
      *                      with this function.
      * @param value         The value of the variable.
      * @throws std::invalid_argument if attempting to set Integer::NPTS
      *         to a negative number or if any of the time variables are
      *         out of range.
      */
    void setHeader(const Integer variableName, const int value);
    /*! @} */

    /*! @name Logical Header Variables
     * @{
     */
    /*! @brief Gets a logical header variable.
     * @param variableName  The name of the logical variable to get.
     * @result The value of the header variable.  If the header variable
     *         has not been set then this will be -12345.  Otherwise,
     *         0 indicates false while 1 indicates true.
     */
    int getHeader(const Logical variableName) const noexcept;
    /*! @brief Sets a logical header variable.
      * @param variableName  The name of the logical variable to set.
      * @param value         The value of the variable.
      * @note That after a variable is set it can only to toggled on or off.
      */
    void setHeader(const Logical variableName, const bool value) noexcept;
    /*! @} */

    /*! @name Character Header Variables
     * @{
     */
    /*! 
     * @brief Gets a character header variable.
     * @result The value of the header variable.  If the header variable
     *         has not been set then this will be "-12345".
     */
    std::string getHeader(const Character variableName) const noexcept;
    /*! 
     * @brief Sets a character header variable.
     * @param[in] variableName  The variable name to set.
     * @param[in] value         The value of the character to set.  If the the
     *                          length is too long then it will be truncated.
     */
    void setHeader(const Character variableName,
                   const std::string &value) noexcept;
    /*! @} */

    /*!
     * @brief Checks if this class is a valid representation of a SAC file.
     *        This means that the header is defined and that there is data.
     * @result True indicates that this class represents a valid SAC waveform. 
     */
    bool isValid() const noexcept;
    /*!
     * @brief Gets the sampling period.
     * @result The sampling period in seconds of the waveform.
     * @note If the waveform is not properly initialized then this can be 
     *       negative.  In this case the value will likely be -12345.
     * @sa isValid()
     */
    double getSamplingPeriod() const noexcept;
    /*!
     * @brief Gets the number of points in the waveform.
     * @result The number of samples in the waveform.
     * @note If the waveform is not properly initialized then this can
     *       be negative.  In this case the value will likely be -12345.
     * @sa isValid()
     */
    int getNumberOfSamples() const noexcept;

    /*!
     * @brief Sets the data.
     * @param[in] npts  The number of samples in the waveform.  This must be
     *                  positive. 
     * @param[in] data  The waveform data.  This is an array of dimension
     *                  [npts].
     * @throws std::invalid_argument if npts is not positive or data is NULL.
     */
    void setData(const int npts, const double *data);
    /*!
     * @brief Returns a pointer to the data.
     * @result A pointer to the data.  This can be NULL.  The length of
     *         the pointer is given by \c getNumberOfSamples().
     */
    const double *getDataPointer() const noexcept;
    /*!
     * @brief Returns a copy of the data.
     * @result A copy of the waveform data.
     */
    std::vector<double> getData() const noexcept;

    /*!
     * @brief Loads a SAC data file.
     * @param[in] fileName  The name of file to read.
     * @throws std::invalid_argument if fileName does not exist
     *         or the SAC file is unreadable.
     */
    void read(const std::string &fileName);
    /*!
     * @brief Writes the SAC file.
     * @param[out] fileName  The SAC file to write.
     * @throws std::invalid_argument if the path to fileName is invalid.
     * @throws std::runtime_error if the SAC class is not valid or.
     * @sa \c isValid()
     */
    void write(const std::string &fileName, const bool lswap = false) const;
private:
    class WaveformImpl;
    std::unique_ptr<WaveformImpl> pImpl;
};

}
#endif
