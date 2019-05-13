#ifndef TEMBLOR_LIBRARY_UTILITIES_LOCATION_GLOBALPOSITIONPAIR_HPP
#define TEMBLOR_LIBRARY_UTILITIES_LOCATION_GLOBALPOSITIONPAIR_HPP 1
#include <memory>

namespace Temblor::Library::Utilities::Location
{

class GlobalPosition;
/*!
 * @class GlobalPositionPair globalPoisitionPair.hpp "temblor/library/utilities/location/globalPositionPair.hpp"
 * @brief Manages a pair of global locations for computing distance, azimuth,
 *        and back azimuth.
 */
class GlobalPositionPair
{
public:
    /*! @name Constructors
     * @{
     */
    /*!
     * @brief Default constructor.
     */
    GlobalPositionPair();
    /*!
     * @brief Constructs the class from the position pair.
     * @param[in] source    The source position.
     * @param[in] receiver  The receiver position.
     * @throws std::invalid_argument if source and receiver do not have
     *         a latitude and longitude.
     */
    GlobalPositionPair(const GlobalPosition &source,
                       const GlobalPosition &receiver);
    /*! @} */

    /*! @name Destructors
     * @{
     */
    /*!
     * @brief Default destructor.
     */
    ~GlobalPositionPair();
    /*!
     * @brief Clears the positions.
     */
    void clear() noexcept; 
    /*! @} */

    /*! 
     * @brief Flag indicating whether or not the source position was set.
     * @result True indicates that the source position has been set.
     */
    bool hasSourcePosition() const noexcept;
    /*!
     * @brief Sets the source position.
     * @param[in] source   The source position.
     * @throws std::invalid_argument if the source position does not have a 
     *         latitude or longitude.
     */
    void setSourcePosition(const GlobalPosition &source);
    /*! 
     * @brief Gets the source position.
     * @result The source position.
     * @throws std::runtime_error if the source position has not been set.
     */ 
    GlobalPosition getSourcePosition() const;

    /*! 
     * @brief Flag indicating whether or not the receiver position was set.
     * @result True indicates that the receiver position has been set.
     */
    bool hasReceiverPosition() const noexcept;
    /*!
     * @brief Sets the receiver position.
     * @param[in] receiver  The receiver position.
     * @throws std::invalid_argument if the receiver position does not have a
     *         latitude or longitude.
     */
    void setReceiverPosition(const GlobalPosition &receiver);
    /*!
     * @brief Gets the receiver position.
     * @result The receiver position.
     * @throws std::runtime_error if the receiver position has not been set.
     */
    GlobalPosition getReceiverPosition() const;

    /*!
     * @brief Gets the azimuth from point 1 to point 2.
     * @result The azimuth in degrees from point 1 to point 2.  Azimuth is
     *         measured positive clockwise from north.
     * @throws std::runtime_error if the positiosn are not set.
     */
    double getAzimuth() const;
    /*!
     * @brief gEts the back-azimuth from point 2 to point 1.
     * @Result The back-azimuth in degrees from point 2 to point 1.  
     *         Back-azimuth is measured positive clockwise from north.
     * @throws std::runtime_error if the positions are not set.
     */
    double getBackAzimuth() const;
    /*! 
     * @brief Gets the distance between the two points in meters.
     * @result The distance between point 1 and point 2 in meters
     * @throws std::runtime_error if the positions are not set.
     */
    double getDistance();
    /*!
     * @brief Gets the distance between the two points in kilometers.
     * @result The distance between point 1 and point 2 in kilometers
     * @throws std::runtime_error if the positions are not set.
     */
    double getDistanceInKilometers();
    /*!
     ° @brief Gets the great-circle distance between the two points in
     *        degrees.
     * @result The distance between point 1 and point 2 in degrees.
     * @throws std::runtime_Error if the positions are not set.
     */
    double getGreatCircleDistance();
private:
    class GlobalPositionPairImpl;
    std::unique_ptr<GlobalPositionPairImpl> pImpl;
};

}
#endif
