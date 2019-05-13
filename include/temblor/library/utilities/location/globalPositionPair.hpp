#ifndef TEMBLOR_LIBRARY_UTILITIES_LOCATION_GLOBALPOSITIONPAIR_HPP
#define TEMBLOR_LIBRARY_UTILITIES_LOCATION_GLOBALPOSITIONPAIR_HPP 1
#include <memory>

namespace Temblor::Library::Utilities::Location
{

/*!
 * @class GlobalPositionPair globalPoisitionPair.hpp "temblor/library/utilities/location/globalPositionPair.hpp"
 * @brief Manages a pair of global locations for computing distance, azimuth,
 *        and back azimuth.
 */
class GlobalPositionPair
{
public:
    /*!
     * @brief Default constructor.
     */
    GlobalPositionPair();
    /*!
     * @brief Constructs the class from the position pair.
     * @throws std::invalid_argument if p1 or p2 are not correctly initialized.
     */
    GlobalPositionPair(const GlobalPosition &p1, const GlobalPosition &p2);
    /*!
     * @brief Flag indicating whether or not the position pair has been set.
     * @result True indicates the position pair has been set.
     */
    bool hasPair() const noexcept;
    /*!
     * @brief Sets the position pair.
     * @param[in] p1   The first position.  This will most likely be the source.
     * @param[in] p2   The second position.  This will most likely be the
     *                 receiver.
     * @throws std::invalid_argument if p1 or p2 are not correctly initialized.
     */
    void setPositionPair(const GlobalPosition &p1, const GlobalPosition &p2);
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
    std::unique_ptr<GlobalPositionPairImpl>;
};

/*
double computeAzimuth(const GlobalPosition &p1,
                      const GlobalPosition &p2);
double computeBackAzimuth(const GlobalPosition &p1,
                          const GlobalPosition &p2);
double computeGreatCircleDistance(const GlobalPosition &p1,
                                  const GlobalPosition &p2);
*/

}
#endif