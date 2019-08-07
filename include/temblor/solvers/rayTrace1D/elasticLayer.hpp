#ifndef TEMBLOR_SOLVERS_RAYTRACE1D_LAYER_HPP
#define TEMBLOR_SOLVERS_RAYTRACE1D_LAYER_HPP
#include "temblor/solvers/rayTrace1D/enums.hpp"
#include <memory>

namespace Temblor::Solvers::RayTrace1D
{
class IsotropicLayer;
/*!
 * @brief This is a container for elastic layers with varying degrees of 
 *        symmetries.
 * @copyright Ben Baker (University of Utah) distributed under the MIT license.
 */
class ElasticLayer
{
public:
    /*! @name Constructors
     * @{
     */
    /*!
     * @brief Default constructor.
     */
    ElasticLayer();
    /*!
     * @brief Copy constructor.
     * @param[in] layer  The layer from which to initialize this class.
     */
    ElasticLayer(const ElasticLayer &layer);
    /*!
     * @brief Move constructor.
     * @param[in,out] layer  The layer from which to initialize this class.
     *                       On exit, layer's behavior will be undefined.
     */
    ElasticLayer(ElasticLayer &&layer) noexcept;
    /*! @} */

    /*! @name Operators
     * @{
     */
    /*!
     * @brief Copy assignment operator.
     * @param[in] layer  The layer to copy to this.
     * @result A deep copy of the input layer.
     */
    ElasticLayer& operator=(const ElasticLayer& layer);
    /*!
     * @brief Move assignment operator.
     * @param[in,out] layer  The layer whose memory will be moved to this.
     *                       On exit, layer's behavior is undefined.
     * @result The moved memory from layer.
     */
    ElasticLayer& operator=(ElasticLayer &&layer) noexcept;
    /*! @} */

    /*! @name Destructors
     * @{
     */
    /*!
     * @brief Destructor.
     */
    ~ElasticLayer();
    /*!
     * @brief Clears the layer properties.
     */
    void clear() noexcept;
    /*! @} */

    /*!
     * @brief Gets the layer symmetry.
     * @result The layer symmetry.
     */
    ElasticLayerSymmetry getSymmetry() const noexcept;

    /*! @name Isotropic layer
     * @{
     */
    /*!
     * @brief Sets an isotropic layer.
     * @param[in] layer  The isotropic layer to set.
     * @throws std::invalid_argument if the isotropic layer isn't valid.
     */
    void setLayer(const IsotropicLayer &layer);
    //void setLayer(const TransverselyIsotropicLayer &layer);
    /*!
     * @brief Gets an isotropic layer.
     * @throws std::runtime_error if the layer symmetry is not isotropic.
     * @sa \c getSymmetry()
     */
    IsotropicLayer getLayer() const;
    /*! @} */

    /*!
     * @brief Gets the group velocity of of the quasi-P wave.
     * @param[in] theta    The propagation angle in degrees.
     * @param[in] azimuth  The propagation azimuth in degrees. 
     * @result The group velocity of the qP wave in meters/second.
     * @throws std::runtime_error if \c setLayer() was never called. 
     */
    double getQpGroupVelocity(const double theta,
                              const double azimuth) const;
    /*!
     * @brief Gets the layer's density.
     * @result The density in kg/m**3.
     * @throws std::runtime_error if \c setLayer() was never called. 
     */
    double getDensity() const;
private:
    class ElasticLayerImpl;
    std::unique_ptr<ElasticLayerImpl> pImpl;
};
}
#endif
