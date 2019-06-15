#ifndef TEMBLOR_LIBRARY_MODELS_EVENT_ORIGIN_HPP
#define TEMBLOR_LIBRARY_MODELS_EVENT_ORIGIN_HPP
#include <memory>

namespace Temblor::Library::Utilities
{
class Time;
namespace Geodetic
{
class GlobalPosition;
}
}

namespace Temblor::Library::Models::Event
{

/*!
 * @brief Defines an origin in terms of its time and location.
 */
class Origin 
{
public:
    /*! @name Constructors.
     * @{
     */
    /*! 
     * @brief Constructor.
     */
    Origin();
    /*! 
     * @brief Copy constructor.
     * @param[in] origin  Origin class from which to initialize this class.
     */
    Origin(const Origin &origin);
    /*! 
     * @brief Move constructor.
     * @param[in,out] origin  Origin class which to initialize this class.
     *                        On exit origin's behavior is undefined.
     */
    Origin(Origin &&origin) noexcept;
    /*! @} */

    /*! @name Operators.
     * @{
     */
    /*!
     * @brief Copy operator.
     * @param[in] origin  The origin class to copy.
     * @result A deep copy of the origin.
     */
    Origin& operator=(const Origin &origin);
    /*!
     * @brief Move operator.
     * @param[in,out] origin  The origin class to move.  On exit origin's
     *                        behavior is undefined.
     * @result The result of moving origin's memory to this.
     */
    Origin& operator=(Origin &&origin) noexcept;
    /*! @} */

    /*! @name Destructors
     * @{
     */
    /*! 
     * @brief Default destructor.
     */
    ~Origin();
    /*! 
     * @brief Removes all origin information.
     */
    void clear() noexcept;
    /*! @} */

    /*! @name Name
     * @brief Sets the name of the origin.
     * @{
     */
    /*!
     * @brief Sets the origin name.
     * @param[in] name  The name of the origin.
     */
    void setName(const std::string &name);
    /*!
     * @brief Gets the name of the origin.
     * @result The event origin name.  Note, that this can be blank if not set.
     */
    std::string getName() const noexcept;
    /*! @} */

    /*! @name Time
     * @{
     */
    /*! 
     * @brief Sets the origin time.
     * @param[in] originTime  The origin time.
     */ 
    void setTime(const Temblor::Library::Utilities::Time &originTime);
    /*!
     * @brief Gets the origin time.
     * @result The origin time.
     * @throws std::runtime_error if the origin time was not set.
     */
    Temblor::Library::Utilities::Time getTime() const;
    /*! @} */

    /*! @name Hypocenter
     * @{
     */
    /*!
     * @brief Sets the origin's hypocentral location.
     * @param[in] hypocenter  The origin's hypocentral location.
     * @throws std::invalid_argument if the hypocenter's latitude, longitude,
     *         or depth is not set.
     */
    void setHypocenter(const Temblor::Library::Utilities::Geodetic::GlobalPosition &hypocenter);
    /*!
     * @brief Gets the origin's hypocentral location.
     */
    Utilities::Geodetic::GlobalPosition getHypocenter() const;
    /*! @} */
private:
    class OriginImpl;
    std::unique_ptr<OriginImpl> pImpl;
};

}
#endif
