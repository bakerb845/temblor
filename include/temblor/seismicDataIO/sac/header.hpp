#ifndef TEMBLOR_SEISMICDATAIO_SAC_HEADER_HPP
#define TEMBLOR_SEISMICDATAIO_SAC_HEADER_HPP
#include <memory>
#include <string>
#include "temblor/seismicDataIO/sac/enums.hpp"

namespace Temblor::SeismicDataIO::SAC
{
/*!
 * @brief Defines the SAC header.
 * @copyright Ben Baker (University of Utah) distributed under the MIT license.
 */
class Header
{
public:
    /*! @name Constructors
     * @{
     */
    /*!
     * @brief Constructor.
     */
    Header();
    /*!
     * @brief Creates a header from character data.
     * @param[in] header  The binary header from which to initialize the header.
     * @param[in] lswap   This an optional variable that indicates whether or
     *                    not the header bytes should be swapped.  By default
     *                    this is false.
     * @throw std::invalid_argument if header is NULL or if delta or npts
     *        is invalid.  If delta or npts are invalid this can indicate a
     *        problem with the byte order.
     */
    explicit Header(const char header[632], const bool lswap = false);
    /*!
     * @brief Copy constructor.
     * @param[in] header  Header class from which to initialize this class.
     */
    Header(const Header &header);
    /*!
     * @brief Move constructor.
     * @param[in,out] header  Header class to move to this class.
     *                        On exit header's behavior will be undefined.
     */
    Header(Header &&header) noexcept;
    /*! @} */

    /*! @name Operators
     * @{
     */
    /*!
     * @brief Copy assignment operator.
     * @param[in] header  Header class to copy.
     * @result A deep copy of the input header.
     */
    Header &operator=(const Header &header);
    /*!
     * @brief Move assignment operator.
     * @param[in] header  Header to move.
     *                    On exit header's behavior will be undefined.
     * @result The moved contents of header.
     */
    Header &operator=(Header &&header) noexcept;
    /*! @}*/

    /*! @name Destructors
     * @{
     */
    /*!
     * @brief Destructor.
     */
    ~Header();
    /*!
     * @brief Clears the header and sets all variables to undefined.
     */
    void clear() noexcept;
    /*! @} */

    /*!
     * @brief Reads the header from a file.
     * @param[in] fileName  The name of the SAC file.
     * @throws std::invalid_argument if the file does not exists or the header
     *         is malformed.
     * @sa \c setFromBinaryHeader()
     */
    void read(const std::string &fileName);
    /*!
     * @brief Sets the header from a character string comprised of binary data.
     * @param[in] header   Character string with header variables.  This is an
     *                     array of dimension [632].  Moreover, this would have
     *                     been directly lifted from the first 632 bytes of the
     *                     SAC file.
     * @param[in] lswap    If true then the byte order will be swapped.
     *                     The default is false.
     * @throws std::invalid_argument if the sampling rate or number of samples
     *         are invalid.  This can indicate a problem with the byte order.
     */
    void setFromBinaryHeader(const char chdr[632], bool lswap = false);
    /*!
     * @brief Creates a header for writing to a binary file.
     * @param[out] header  The packed header.  This constitutes the first
     *                     632 bytes of the SAC binary file.
     * @param[in] lswap    If true then the byte order will be swapped.
     *                     The default is false.
     */
    void getBinaryHeader(char header[632], 
                         bool lswap = false) const noexcept;
    /*! @name Floating Point Header Variables
     * @{
     */
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
    void setHeader(const Double variableName, double value);
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
      * @param value         The value of the variable.
      * @throws std::invalid_argument if attempting to set Integer::NPTS
      *         to a negative number or if any of the time variables are
      *         out of range.
      */
    void setHeader(const Integer variableName, int value);
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
    void setHeader(const Logical variableName, bool value) noexcept;
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
private:
    class HeaderImpl;
    std::unique_ptr<HeaderImpl> pImpl;
};

}
#endif
