#ifndef TEMBLOR_SOLVERS_RAYTRACE1D_RAYPATH_HPP
#define TEMBLOR_SOLVERS_RAYTRACE1D_RAYPATH_HPP 1
#include <memory>
using namespace Temblor::Solvers::RayTrace1D
{
class RaySegment;
class RayPath
{
public:
    /*! @name Constructors
     * @{
     */
    /*!
     * @brief Constructor
     */
    RayPath();
    /*! @} */

    /*!
     * @param[in] segment        The ray segment beginining at the source.
     * @param[in] lCloseRayPath  If true then this segment connects from the
     *                           source point to the destination point. 
     */
    void startRayPath(const class RaySegment &segment,
                      const bool lCloseRayPath = false);
    /*!
     * @param[in] segment        The segment to append to the raypath.
     * @param[in] lCloseRayPath  If true then this segment connects to the
     *                           destination point.
     * @throws std::runtime_error if \c startRayPath() was not called
     *         the ray path construction is closed.
     */
    void appendSegment(const class RaySegment &segment,
                       const bool lCloseRayPath = false);
    /*!
     * @param[in] segment   The ray segment ending at the source.
     * @throws std::runtime_error if \c startRayPath() was not called.
     */
    void endRayPath(const class RaySegment &segment);
    /*!
     * @brief Convenience function to close the ray path construction in the
     *        case that \c endRayPath() was not called or \c startRayPath()
     *        or \c appendSegment() were not called with lCloseRayPath = true.
     * @throws std::runtime_error if the \c startRayPath was not called.
     */
    void closeRayPath();
    /*!
     * @brief Determines if a raysegment can be appended to the ray path.
     * @result If true then a ray segment can be appended to the ray path.
     */
    bool canAppendRaySegment() const noexcept;
private:
    class RayPathImpl;
    std::unique_ptr<RayPathImpl> pImpl; 
};
}
#endif
