#ifndef TEMBLOR_SOLVERS_RAYTRACE1D_ISOTROPICLAYERCAKEMODEL_HPP
#define TEMBLOR_SOLVERS_RAYTRACE1D_ISOTROPICLAYERCAKEMODEL_HPP
#include <memory>

namespace Temblor::Solvers::RayTrace1D
{
class IsotropicLayer;
/*!
 * @brief Defines a 1D layer cake model made up of isotropic layers.
 */
class IsotropicLayerCakeModel
{
public:
    /*! @name Constructors
     * @{
     */
    /*!
     * @brief Constructor.
     */
    IsotropicLayerCakeModel();
    /*!
     * @brief Copy constructor.
     * @param[in] model  The 1D layer cake model from which to initialize.
     */
    IsotropicLayerCakeModel(const IsotropicLayerCakeModel &model);
    /*!
     * @brief Move constructor.
     * @param[in,out] model  The 1D layer cake model from which to initialize.
     *                       On exit model's behavior is undefined.
     */
    IsotropicLayerCakeModel(IsotropicLayerCakeModel &&model) noexcept;
    /*! @} */ 

    /*! @brief Operators
     * @{
     */
    /*!
     * @brief Copy assignment operator.
     * @param[in] model  The layer cake model to copy.
     * @result A deep copy of the layer cake model.
     */
    IsotropicLayerCakeModel& operator=(const IsotropicLayerCakeModel &model);
    /*!
     * @brief Move assignment operator.
     * @param[in,out] model  The layer cake model's memory which is to be
     *                       moved to this.  On exit, model's behavior is
     *                       undefined.
     * @result Model's memory moved to this.
     */
    IsotropicLayerCakeModel& operator=(IsotropicLayerCakeModel &&model) noexcept;
    /*! @} */

    /*! @name Destructors
     * @{
     */
    /*!
     * @brief Destructor.
     */
    ~IsotropicLayerCakeModel();
    /*!
     * @brief Clears the model.
     */
    void clear() noexcept;
    /*! @} */
    
    /*!
     * @brief This begins the model building process by setting the shallowmost
     *        layer in the earth model.  It will clear any previous model.
     * @param[in] layer       The material properties of the top layer.
     * @param[in] lLastLayer  If true then this indicates that this is the
     *                        last (i.e., halfspace) layer and the model
     *                        construction will close.
     * @throws std::invalid_argument if layer is not valid.
     */  
    void setTopLayer(const IsotropicLayer &layer,
                     const bool lLastLayer = false);
    /*!
     * @brief Appends a layer to the model.  This can be called as many times
     *        as needed after calling \c setTopLayer().
     * @param[in] layer       The layer to append.
     * @param[in] lLastLayer  If true then this indicates that this is the last 
     *                        last (i.e., halfspace) layer and the model
     *                        construction will close.
     * @throws std::runtime_error if \c setTopLayer() was not yet called
     *         or the model construction phase is closed.
     * @sa \c canAppendLayer()
     * @throws std::invalid_argument if layer is not valid.
     */
    void appendLayer(const IsotropicLayer &layer,
                     const bool lLastLayer = false);
    /*!
     * @brief Appends a critically refracting layer to the model.
     * @param[in] layer       The layer to append.  The compressional and shear
     *                        velocities in this layer must exceed the 
     *                        compressional and shear velocities in the previous
     *                        layer.
     * @param[in] lLastLayer  If true then this indicates that this is the last
     *                        last (i.e., halfspace) layer and the model
     *                        construction will close.
     */
    void appendCriticallyRefractingLayer(const IsotropicLayer &layer,
                                         const bool lLastLayer = false);
    /*!
     * @brief Appends a reflecting layer to the model.
     * @param[in] layer       The layer to append.  The compressional and shear
     *                        velocities in this layer match the compressional
     *                        and shear velocities in the previous layer.
     * @param[in] lLastLayer  If true then this indicates that this is the last
     *                        last (i.e., halfspace) layer and the model
     *                        construction will close.
     */
    void apendReflectingLayer(const IsotropicLayer &layer,
                              const bool lLastLayer = false);
   
    /*!
     * @brief Gets the number of layers in the model.
     * @result The number of layers in the model.
     */ 
    int getNumberOfLayers() const noexcept;
    /*!
     * @brief Determines if layers can be appended to the model. 
     * @result True indicates that \c appendLayer() can be called.
     */
    bool canAppendLayer() const noexcept;
    /*!
     * @brief Extracts the layerIndex'th layer.
     * @param[in] layerIndex  The index of the layer to extract.
     * @result The layerIndex'th isotropic layer's parameters.
     * @throws std::invalid_argument if layerIndex is not in the range
     *         [0, \c getNumberOfLayers()-1].
     * @throws std::runtime_error if there are no layers in the model.
     * @sa \c getNumberOfLayers()
     */
    IsotropicLayer getLayer(const int layerIndex);
private:
    class IsotropicLayerCakeModelImpl;
    std::unique_ptr<IsotropicLayerCakeModelImpl> pImpl;
};
}

#endif
