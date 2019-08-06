#ifndef TEMBLOR_SOLVERS_RAYTRACE1D_LAYER_HPP
#define TEMBLOR_SOLVERS_RAYTRACE1D_LAYER_HPP
#include "temblor/solvers/rayTrace1D/enums.hpp"
#include <memory>

namespace Temblor::Solvers::RayTrace1D
{
class IsotropicLayer;
/*!
 * @brief This is a container for layers with varying degrees of symmetries.
 * @copyright Ben Baker (University of Utah) distributed under the MIT license.
 */
class Layer
{
public:
    /*! @name Constructors
     * @{
     */
    /*!
     * @brief Default constructor.
     */
    Layer();
    /*!
     * @brief Copy constructor.
     * @param[in] layer  The layer from which to initialize this class.
     */
    Layer(const Layer &layer);
    /*!
     * @brief Move constructor.
     * @param[in,out] layer  The layer from which to initialize this class.
     *                       On exit, layer's behavior will be undefined.
     */
    Layer(Layer &&layer) noexcept;
    /*! @} */

    /*! @name Operators
     * @{
     */
    /*!
     * @brief Copy assignment operator.
     * @param[in] layer  The layer to copy to this.
     * @result A deep copy of the input layer.
     */
    Layer& operator=(const Layer& layer);
    /*!
     * @brief Move assignment operator.
     * @param[in,out] layer  The layer whose memory will be moved to this.
     *                       On exit, layer's behavior is undefined.
     * @result The moved memory from layer.
     */
    Layer& operator=(Layer &&layer) noexcept;
    /*! @} */

    /*! @name Destructors
     * @{
     */
    /*!
     * @brief Destructor.
     */
    ~Layer();
    /*!
     * @brief Clears the layer properties.
     */
    void clear() noexcept;
    /*! @} */

    /*!
     * @brief Gets the layer symmetry.
     * @result The layer symmetry.
     */
    LayerSymmetry getSymmetry() const noexcept;

    /*! @name Isotropic layer
     * @{
     */
    /*!
     * @brief Sets an isotropic layer.
     * @param[in] layer  The isotropic layer to set.
     * @throws std::invalid_argument if the isotropic layer isn't valid.
     */
    void setLayer(const IsotropicLayer &layer); 
    /*!
     * @brief Gets an isotropic layer.
     * @throws std::runtime_error if the layer symmetry is not isotropic.
     * @sa \c getSymmetry()
     */
    IsotropicLayer getLayer() const;
    /*! @} */ 
private:
    class LayerImpl;
    std::unique_ptr<LayerImpl> pImpl;
};
}
#endif
