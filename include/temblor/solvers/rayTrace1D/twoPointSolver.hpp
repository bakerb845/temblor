#ifndef TEMBLOR_SOLVERS_RAYTRACE1D_TWOPOINTSOLVER_HPP
#define TEMBLOR_SOLVERS_RAYTRACE1D_TWOPOINTSOLVER_HPP
#include <memory>
namespace Temblor::Solvers::RayTrace1D
{
class IsotropicLayerCakeModel;
/*!
 * @class TwoPoint twoPoint.hpp
 * @brief Performs the two-point ray tracing.  This is useful when you have a
 *        known source position and receiver position and want to connect a
 *        ray between the two.
 * @note For algorithmic details see Microseismic event location using the
 *       first and reflected arrivals (Belayouni, 2015).
 * @copyright Ben Baker (University of Utah) distributed under the MIT license.
 */
class TwoPointSolver
{
public:
    /*! @name Constructors
     * @{
     */
    /*!
     * @brief Constructor.
     */
    TwoPointSolver();
    /*!
     * @brief Copy constructor.
     * @param[in] solver  The solver class from which to initialize this class.
     */
    TwoPointSolver(const TwoPointSolver &solver);
    /*!
     * @brief Move constructor.
     * @param[in,out] solver  The solver class from which to initialize this 
     *                        class.  On exit, solver's behavior is undefined.
     */
    TwoPointSolver(TwoPointSolver &&solver) noexcept;
    /*! @} */

    /*! @name Operators
     * @{
     */
    /*!
     * @brief Copy assignment operator.
     * @param[in] solver  The solver to copy.
     * @result A deep copy of the solver.
     */
    TwoPointSolver& operator=(const TwoPointSolver &solver);
    /*!
     * @brief Move assignment operator.
     * @param[in,out] solver  The solver whose memory is moved to this.
     *                        On exit solver's behavior is undefined.
     * @result The memory from solved moved onto this.
     */
    TwoPointSolver& operator=(TwoPointSolver &&solver) noexcept;
    /*! @} */

    /*! @name Destructors
     * @{
     */
    /*!
     * @brief Destructor.
     */
    ~TwoPointSolver();
    /*!
     * @brief Clears all memory and resets the class.
     */
    void clear() noexcept;
    /*! @} */

    /*! @name Initialize earth models 
     * @{
     */
    /*!
     * @brief Sets the isotropic earth model.
     * @throws std::invalid_argument if the isotropic model is not valid.
     * @note This will reset the source and receiver depths to the free
     *       surface and set the source/receiver offset to 0.
     */
    void setEarthModel(const IsotropicLayerCakeModel &model);
    /*! @} */

    /*! @name Initialize source/receiver positions
     * @{
     */
    /*!
     * @brief Sets the source depth.
     * @param[in] sourceDepth  The source depth in meters.
     * @throws std::invalid_argument if the sourceDepth is not in the model.
     * @throws std::runtime_error if the earth model is not yet set.
     */
    void setSourceDepth(const double sourceDepth);
    /*!
     * @brief Puts the source at the free surface.
     */
    void setSourceDepthToFreeSurface() noexcept;
    /*!
     * @brief Sets the source/receiver offset.
     * @param[in] receiverOffset  The source/receiver offset in meters.
     * @throws std::invalid_argument if the offset is not positive.
     */ 
    void setReceiverOffset(const double receiverOffset);
    /*!
     * @brief This is an optional routine that sets the receiver depth.
     * @note By default the receiver is placed at the free surface.
     * @param[in] receiverDepth  Sets the receiver depth in meters.
     * @throws std::runtime_error if the receiver is not in the model.
     */
    void setReceiverDepth(const double receiverDepth);
    /*!
     * @brief Puts the receiver at the free surface. 
     */
    void setReceiverDepthToFreeSurface() noexcept;
    /*! @} */

    /*! @name Ray tracers 
     * @{
     */
    /*!
     * @brief Computes the direct ray.
     */
    void computeDirectRay();
    /*!
     * @brief Computes the critically refracted rays.
     */
    void computeCriticallyRefractedRays();
    /*!
     * @brief Comiputes the reflected rays.
     */
    void computeReflectedRays();
    /*! @} */

    
private:
    class TwoPointSolverImpl;
    std::unique_ptr<TwoPointSolverImpl> pImpl; 
};
}
#endif
