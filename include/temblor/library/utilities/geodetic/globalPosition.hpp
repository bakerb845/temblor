#ifndef TEMBLOR_LIBRARY_UTILITIES_GEODETIC_GLOBALPOSITION_HPP
#define TEMBLOR_LIBRARY_UTILITIES_GEODETIC_GLOBALPOSITION_HPP 1
#include <memory>

namespace Temblor::Library::Utilities::Geodetic
{

/*!
 * @brief Defines the location operation mode.
 */
enum class Type
{
    UNKNOWN, /*!< Unknown location type. */
    GLOBAL,  /*!< Global location type which uses latitude and longitude. */
    UTM      /*!< Universal Transverse Mercator location type.
                  This can be used in tandem with global. */
};

/*!
 * @class GlobalPosition globalPosition.hpp "temblor/library/utilities/location/globalPosition.hpp"
 * @brief Defines a location in terms of a latitude, longitude, and depth.
 * @note The coordinate system convention is positive north, positive east,
 *       positive down.
 */
class GlobalPosition
{
public:
    /*! @name Constructors
     * @{
     */
    /*!
     * @brief Default constructor.
     */
    GlobalPosition();
    /*!
     * @brief Copy constructor.
     * @param[in] position  The global position to class from which to
     *                      initialize this class.
     */
    GlobalPosition(const GlobalPosition &position);
    /*!
     * @brief Move constructor.
     * @param[in,out] position  The class to move to this class.
     *                          On exit position's behavior will be undefined.
     */
    GlobalPosition(GlobalPosition &&position);
    /*! 
     * @brief Sets the global location from the latitude and  longitude.
     *        Note, that the depth is not set.
     * @param[in] latitude   The latitude in degrees.  This must be in the range
     *                       [-90,90].
     * @param[in] longitude  The longitude in degrees.  This must be in the
     *                       range (-540,540).  Though it will be converted to
     *                       the range [0,360].
     * @sa \c setLatitude(), \c setLongitude()
     * @throws std::invalid_argument if the latitude or longitude is out
     *         of range.
     */ 
    GlobalPosition(const double latitude, const double longitude);
    /*!
     * @brief Sets the global location from the latitude, longitude and depth.
     * @param[in] latitude   The latitude in degrees.  This must be in the range
     *                       [-90,90].
     * @param[in] longitude  The longitude in degrees.  This must be in the
     *                       range (-540,540).  Though it will be converted to
     *                       the range [0,360].
     * @param[in] depth      The depth in meters below sea level.
     * @sa \c setLatitude(), \c setLongitude(), \c setDepth()
     * @throws std::invalid_argument if the latitude or longitude is out
     *         of range.
     */ 
    GlobalPosition(const double latitude, const double longitude,
                   const double depth);
    /*! @} */

    /*! @name Operators
     * @{
     */
    /*!
     * @brief Copy assignment operator.
     * @param[in] position  The global position class to copy.
     * @result A deep copy of position.
     */
    GlobalPosition& operator=(const GlobalPosition &position);
    /*!
     * @brief Move assignemnt operator.
     * @param[in,out] position  Position class to move.
     *                          On exit position's memory will be undefined.
     * @result Position's memory moved to this.
     */
    GlobalPosition& operator=(GlobalPosition &&position);
    /*! @} */

    /*! @name Destructors
     * @{
     */
    /*!
     * @brief Destructor.
     */
    ~GlobalPosition();
    /*!
     * @brief Resets the class.
     */
    void clear() noexcept;
    /*! @} */

    /*!
     * @brief Indicates whether or not the depth has been set.
     * @result True indicates that the depth has been set.
     */
    bool haveDepth() const noexcept;
    /*!
     * @brief Sets the depth.
     * @param[in] depth   The position's depth below sea-level in meters.
     *                    Positive is down.
     */
    void setDepth(const double) noexcept;
    /*!
     * @brief Gets the depth.
     * @result The depth below sea level in meters.
     * @throws std::runtime_error if the depth is not set.
     */
    double getDepth() const;

    /*! 
     * @brief Indicates whether or not the longitude has been set.
     * @result True indicates that the longitude has been set.
     */
    bool haveLongitude() const noexcept;
    /*!
     * @brief Sets the longitude.
     * @param[in] longitude  The longitude in degrees.  East is positive.
     *                       This must be in the range (-540,540)
     *                       This will be converted to the range [0,360).
     * @result std::invalid_argument if longitude is not in bounds.
     */
    void setLongitude(const double longitude);
    /*!
     * @brief Gets the longitude.
     * @result The longitude in degrees.  This will be in the range [0,360]
     *         where east is positive.
     * @throws std::runtime_error if the position has not been set.
     */
    double getLongitude() const;

    /*! 
     * @brief Indicates whether or not the latitude has been set.
     * @result True indicates that the latitude has been set.
     */
    bool haveLatitude() const noexcept;
    /*!
     * @brief Sets the latitude.
     * @param[in] latitude  The latitude in degrees.  North is positive.
     *                      This must be in the range [-90,90].
     * @throws std::invalid_argument if the latitude is not in bounds.
     */
    void setLatitude(const double latitude);
    /*!
     * @brief Gets the latitude.
     * @result The latitude in degrees.  This will be in the range [-90,90]
     *         where north is positive.
     * @throws std::runtime_error if the position has not been set.
     */
    double getLatitude() const;

    /*!
     * @brief Gets the UTM easting position.
     * @result The UTM northing in meters.
     * @throws std::runtime_error if the position has not been set.
     */
    double computeEasting() const;
    /*!
     * @brief Gets the UTM northing position.
     * @result The UTM easting in meters.
     * @throws std::runtime_error if the position has not been set.
     */
    double computeNorthing() const;
    /*!
     * @brief Gets the UTM zone.
     * @result The UTM zone.
     * throws std::runtime_error if the position has not been set.
     */ 
    int computeUTMZone() const;
private:
    class GlobalPositionImpl;
    std::unique_ptr<GlobalPositionImpl> pImpl;
};

}
#endif
