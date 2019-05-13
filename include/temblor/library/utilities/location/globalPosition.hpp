#ifndef TEMBLOR_LIBRARY_UTILITIES_LOCATION_GLOBALPOSITION_HPP
#define TEMBLOR_LIBRARY_UTILITIES_LOCATION_GLOBALPOSITION_HPP 1
#include <memory>

namespace Temblor::Library::Utilities::Location
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
    /*!
     * @brief Default constructor.
     */
    GlobalPosition();
    /*!
     * @brief
     */
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
    GlobalPosition(const double latitude, const double longitude, const double depth = 0);

    /*!
     * @brief Sets the latitude, longitude, and, optionally, depth.
     * @param[in] latitude   The latitude in degrees.  This must be in the range
     *                       [-90,90].
     * @param[in] longitude  The longitude in degrees.  This must be in the
     *                       range (-540,540).  Though it will be converted to
     *                       the range [0,360].
     * @param[in] depth      The depth in meters below sea-level.  By default 
     *                       this is 0 meters which corresponds to sea level.
     * @sa \c setLatitude(), \c setLongitude(), \c setDepth()
     * @throws std::invalid_argument if the latitude or longitude is out
     *         of range.
     */
    void setPosition(const double latitude, const double longitude, const double depth = 0); 
    /*!
     * @brief
     */ 
    /*!
     * @brief Gets the depth.
     * @result The depth below sea level in meters.
     */
    double getDepth() const noexcept;
    /*!
     * @brief Gets the longitude.
     * @result The longitude in degrees.  This will be in the range [0,360]
     *         where east is positive.
     * @throws std::runtime_error if the position has not been set.
     */
    double getLongitude() const;
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
    double getEasting() const;
    /*!
     * @brief Gets the UTM northing position.
     * @result The UTM easting in meters.
     * @throws std::runtime_error if the position has not been set.
     */
    /*!
     * @brief Gets the UTM zone.
     * @result The UTM zone.
     * throws std::runtime_error if the position has not been set.
     */ 
    int getUTMZone() const;
private:
    class GlobalPositionImpl;
    std::unique_ptr<GlobalPositionImpl> pImpl;
};

}
#endif
