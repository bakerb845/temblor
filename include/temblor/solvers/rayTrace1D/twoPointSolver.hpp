#ifndef TEMBLOR_SOLVERS_RAYTRACE1D_TWOPOINT_HPP
#define TEMBLOR_SOLVERS_RAYTRACE1D_TWOPOINT_HPP
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
};

};
#endif
