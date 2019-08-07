#ifndef TEMBLOR_SOLVERS_RAYTRACE1D_RAYSEGMENT_HPP
#define TEMBLOR_SOLVERS_RAYTRACE1D_RAYSEGMENT_HPP
#include <memory>
namespace Temblor::Solvers::RayTrace1D
{
class RaySegment
{
public:
    /*! @name Constructors
     * @{
     */
    /*!
     * @brief Constructor.
     */
    RaySegment();
    /*!
     * @brief Constructor.
     * @param[in] startPoint  The segment start point where startPoint.first
     *                        is the offset in x (meters) and startPoint.second
     *                        is the depth (meters).
     * @param[in] endPoint    The segment end point where endPoint.first
     *                        is the offset in x (meters) and endPoint.second
     *                        is the depth (meters).
     * @param[in] velocity    The seismic velocity (meters/second) along the
     *                        segment.
     * @throws std::invalid_argument if this is not positive.
     */
    RaySegment(const std::pair<double, double> &startPoint,
               const std::pair<double, double> &endPoint,
               const double velocity);
    /*!
     * @brief Copy constructor.
     * @param[in] segment  The ray segment from which to initialize this class.
     */
    RaySegment(const RaySegment &segment);
    /*!
     * @brief Move constructor.
     * @param[in,out] segment  The ray segment from which to initialize this
     *                         class.  On exit, segment's behavior is undefined.
     */
    RaySegment(RaySegment &&segment) noexcept;
    /*! @} */

    /*! @name Operators
     * @{
     */
    /*!
     * @brief Copy assignment operator.
     * @param[in] segment  The segment to copy.
     * @result A deep copy of segment.
     */
    RaySegment& operator=(const RaySegment &segment);
    /*!
     * @brief Move assignment operator. 
     * @param[in,out] segment  The segment whose memory will be moved to this.
     *                         On exit, segment's behavior is undefined.
     */ 
    RaySegment& operator=(RaySegment &&segment) noexcept;
    /*! @} */

    /*! @name Destructors
     * @{
     */
    /*!
     * @brief Destructor.
     */
    ~RaySegment();
    /*!
     * @brief Clears the data off the segment.
     */
    void clear() noexcept;
    /*! @} */

    /*! @name Start point
     * @{
     */
    /*!
     * @brief Sets the segment's start point.
     * @param[in] startPoint  The segment start point where startPoint.first
     *                        is the offset in x (meters) and startPoint.second
     *                        is the depth (meters).
     */
    void setStartPoint(const std::pair<double, double> &startPoint) noexcept;
    /*!
     * @brief Gets the segment's start point.
     * @throws std::runtime_error if the start point was not set.
     */
    std::pair<double, double> getStartPoint() const;
    /*! @} */

    /*! @name End point
     * @{
     */
    /*!
     * @brief Sets the segment's end point.
     * @param[in] endPoint    The segment end point where endPoint.first
     *                        is the offset in x (meters) and endPoint.second
     *                        is the depth (meters).
     */
    void setEndPoint(const std::pair<double, double> &endPoint) noexcept;
    /*!
     * @brief Gets the segment's end point.
     * @throws std::runtime_error if the end point was not set.
     */
    std::pair<double, double> getEndPoint() const;
    /*! @} */

    /*! @name Velocity
     * @{
     */
    /*!
     * @brief Sets the seismic velocity along the segment.
     * @param[in] velocity    The seismic velocity (meters/second) along the
     *                        segment.
     * @throws std::invalid_argument if the velocity is not positive.
     */
    void setVelocity(const double velocity);
    /*!
     * @brief Gets the seismic velocity along the segment.
     * @result The seismic velocity along the segment in meters/second.
     * @throws std::runtime_error if the seismic velocity was not set.
     */
    double getVelocity() const;
    /*! @} */
 
    /*! @name Distance
     * @{
     */
    /*!
     * @brief Gets the distance between the start point and end point.
     * @result The distance between the start and end point in meters.
     * @throws std::runtime_error if the start or end point was not set.
     */
    double getDistance() const;
    /*! @} */

    /*! @name Travel Time
     * @{
     */
    /*!
     * @brief Gets the travel time from the start to end point.
     * @result The travel time in seconds.
     * @throws std::runtime_error if the start point, end point, or seismic 
     *         velocity was not set.
     */
    double getTravelTime() const;
    /*! @} */
private:
    class RaySegmentImpl;
    std::unique_ptr<RaySegmentImpl> pImpl;
};
}
#endif
