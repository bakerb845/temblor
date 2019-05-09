#ifndef TEMBLOR_LIBRARY_DATAREADERS_SAC_SAC_HPP
#define TEMBLOR_LIBRARY_DATAREADERS_SAC_SAC_HPP
#include <memory>
#include <string>

namespace Temblor::Library::DataReaders::SAC
{

/*!
 * @brief Identifies a SAC floating point header variable.
 * @note SAC files internally store 32-bit float numbers.
 *       However, this interface will deal exclusively in doubles.
 *       Consequently, there will be truncation errors on write.
 */
enum class Double
{
    DELTA,  /*!< The sampling period in seconds.  This is required
                 and must be positive. */
    DEPMEN,
    DEPMAX,
    SCALE,
    ODELTA,
    B,      /*!< Start time of trace relative to GMT time. */
    E,      /*!< End time of trace relative to GMT start time. */
    AZ,     /*!< The event to station azimuth in degrees measured
                 positive clockwise from north.  This is in the range
                 [0,360). */
    BAZ,    /*!< The station to event azimuth in degrees measured
                 positive clockwise from north.   This is in the range
                 [0,360). */
    GCARC,  /*!< The station to event great circle distance
                 in degrees. */
};

/*!
 * @brief Identifies a SAC integer header variable.
 */
enum class Integer
{
    NPTS,    /*! The number of points. */
    NZYEAR,  /*!< The four-digit GMT year. */
    NZJDAY,  /*!< The GMT Julian day.  This must be in the range [1,366]. */
    NZHOUR,  /*!< The GMT hour.  This must be in the range [0,23]. */
    NZMIN,   /*!< The GMT minute.  This must be in the range [0,59]. */
    NZSEC,   /*!< The GMT second.  This must be in the range [0,59]. */
    NZMSEC,  /*!< The GMT millisecond.  This must be in the range [0,999]. */ 
};

/*!
 * @brief Identifies a SAC logical header variable.
 * @note SAC files internally store logical variables as 
 *       32-bit integer.  However, this interface will deal
 *       exclusively with C boolean types.
 */
enum class Boolean
{
    LEVEN,     /*!< True if the data are evenly spaced. */
    LPSPOL,    /*!< True if the station components have a positive polarity
                    under the left-hand rule. */
    LOVROK,    /*!< True if it is okay to overwite this file on disk. */ 
    LCALDA,    /*!< If true then the distance, azimuth, back-azimuth
                    and great-circle distance are to be calculated from
                    the station and event coordinates. */
    UNUSED     /*!< Undefined.  The user can specify this. */
};

/*!
 * @brief Identifies a SAC character header variable.
 *        Note, all character header variables have length 8 except for
 *        KEVNM which has length 16.
 */
enum class Character
{
    KSTNM,  /*!< The station name. */
    KEVNM,  /*!< The event name.  */
    KHOLE,  /*!< The location code. */
    KO,     /*!< The origin time label. */
    KA,     /*!< The first arrival time label. */
    KT0,    /*!< The label corresponding to the T0 arrival. */
    KT1,    /*!< The label corresponding to the T1 arrival. */
    KT2,    /*!< The label corresponding to the T2 arrival. */
    KT3,    /*!< The label corresponding to the T3 arrival. */
    KT4,    /*!< The label corresponding to the T4 arrival. */
    KT5,    /*!< The label corresponding to the T5 arrival. */
    KT6,    /*!< The label corresponding to the T6 arrival. */
    KT7,    /*!< The label corresponding to the T7 arrival. */
    KT8,    /*!< The label corresponding to the T8 arrival. */
    KT9,    /*!< The label corresponding to the T9 arrival. */
    KF,     /*!< The label corresponding to the event end time. */
    KUSER0, /*!< Undefined.  The user can specify this. */
    KUSER1, /*!< Undefined.  The user can specify this. */
    KUSER2, /*!< Undefined.  The user can specify this. */
    KCMPNM, /*!< The channel (component) name. */ 
    KNETWK, /*!< The network to which the station belongs. */
    KDATRD, /*!< Date data was read onto a computer.  This is deprecated. */
    KINST   /*!< Generic name of recording instrument. */
};

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
    Header& operator=(const Header &header);
    /*!
     * @brief Move assignment operator.
     * @param[in] header  Header to move.
     *                    On exit header's behavior will be undefined.
     * @result The moved contents of header.
     */
    Header& operator=(Header &&header) noexcept;
    /*! @}*/

    /*!
     * @brief Destructor.
     */
    ~Header();
    /*!
     * @brief Clears the header and sets all variables to undefined.
     */
    void clear() noexcept;

    /*!
     * @brief Gets a float header variable.
     * @result The value of the header variable.  If the header
     *         variable has not been set then this will be -12345.
     */
    double getHeader(const Double variableName);

    /*!
     * @brief Gets an integer header variable.
     * @result The value of the header variable.  If the header
     *         variable has not been set then this will be -12345.
     */
    int getHeader(const Integer variableName);

    void setHeader(const Boolean variableName, const bool variable) noexcept;
    bool getHeader(const Boolean variableName);
    /*!
     * @brief Gets a character header variable.
     * @result The value of the header variable.  If the header variable
     *         has not been set then this will be "-12345". 
     */
    std::string getHeader(const Character variableName) noexcept;
    /*!
     * @brief Sets a character header variable.
     * @param[in] variableName  The variable name to set.
     * @param[in] variable      The value of the character to set.  If the the
     *                          length is too long then it will be truncated.
     */
    void setHeader(const Character variableName,
                   const std::string &variable) noexcept;
private:
    class HeaderImpl;
    std::unique_ptr<HeaderImpl> pImpl;
};

/*!
 * @brief The SAC class.  This class contains the header and dadta.
 */
class SAC
{
public:
    /*!
     * @brief Constructor.
     */
    SAC();
    /*!
     * @brief Destructor.
     */
    ~SAC();

    /*!
     * @brief Checks if this class is a valid representation of a SAC file.
     *        This means that the header is defined and that there is data.
     * @result True indicates that this class represents a valid SAC waveform. 
     */
    bool isValid() noexcept;
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
     * @throws std::runtime_error if the SAC class is valid.
     */
    void write(const std::string &fileName); 
private:
    class SACImpl;
    std::unique_ptr<SACImpl> pImpl;
};

}
#endif
