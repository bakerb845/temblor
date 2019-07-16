#ifndef TEMBLOR_UTILITIES_GEODETIC_GLOBALPOSITIONPAIR_HPP
#define TEMBLOR_UTILITIES_GEODETIC_GLOBALPOSITIONPAIR_HPP 1
#include <memory>

namespace Temblor::Utilities::Geodetic
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
     * @brief Copy constructor.
     * @param[in] pair  The pair class to copy to this class.
     */
    GlobalPositionPair(const GlobalPositionPair &pair);
    /*!
     * @brief Move constructor.
     * @param[in,out] pair  On input pair's memory will be moved to this class.
     *                      On exit pair's behavior will be undefined.
     */
    GlobalPositionPair(GlobalPositionPair &&pair) noexcept;
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

    /*! @name Operators
     * @{
     */
    /*!
     * @brief Copy assignment operator.
     * @param[in] pair  The global position pair to copy.
     * @result A deep copy of the global position pair.
     */
    GlobalPositionPair& operator=(const GlobalPositionPair &pair);
    /*!
     * @brief Move assignment operator.
     * @param[in,out] pair  The global position pair whose memory is
     *                      to be moved.
     *                      On exit pair's behavior is undefined.
     * @result Contains the moved memory from pair.
     */
    GlobalPositionPair& operator=(GlobalPositionPair &&pair) noexcept;
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
     * @brief Computes the azimuth from the source location to the
     *        receiver location.
     * @result The azimuth in degrees from the source to the receiver.
     *         This is measured positive clockwise from north and is
     *         in the range [0,360].
     * @throws std::runtime_error if the positions are not set.
     */
    double computeAzimuth() const;
    /*!
     * @brief Computes the back-azimuth from the receiver location to 
     *        the source location.
     * @result The back-azimuth in degrees from the receiver to the
     *         source.  This is measured positive clockwise from north
     *         and is in the range [0,360].
     * @throws std::runtime_error if the positions are not set.
     */
    double computeBackAzimuth() const;
    /*! 
     * @brief Gets the distance between the source and receiver.
     * @result The distance in meters between the source and receiver.
     * @throws std::runtime_error if the positions are not set.
     */
    double computeDistance() const;
    /*!
     ° @brief Gets the great-circle distance between the two points in
     *        degrees.
     * @result The great circle distance in degrees between the source and
     *         receiver.
     * @throws std::runtime_Error if the positions are not set.
     */
    double computeGreatCircleDistance() const;
private:
    class GlobalPositionPairImpl;
    std::unique_ptr<GlobalPositionPairImpl> pImpl;
};

}
#endif
