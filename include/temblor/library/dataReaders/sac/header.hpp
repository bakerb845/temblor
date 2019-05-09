#ifndef TEMBLOR_LIBRARY_DATAREADERS_SAC_HEADER_HPP
#define TEMBLOR_LIBRARY_DATAREADERS_SAC_HEADER_HPP
#include <memory>
#include <string>
#include "temblor/library/dataReaders/sac/enums.hpp"

namespace Temblor::Library::DataReaders::SAC
{

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
     * @throw std::invalid_argument if header is NULL.
     */
    explicit Header(const char header[638], const bool lswap = false);
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
private:
    class HeaderImpl;
    std::unique_ptr<HeaderImpl> pImpl;
};

}
#endif
