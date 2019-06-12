#ifndef TEMBLOR_LIBRARY_SOLVERS_RAYTRACE1D_LAYERCAKEMODEL_HPP
#define TEMBLOR_LIBRARY_SOLVERS_RAYTRACE1D_LAYERCAKEMODEL_HPP

namespace Temblor::Library::Solvers::RayTrace1D
{
class IsotropicLayer;
/*!
 * @brief Defines a 1D layer cake model.
 */
class LayerCakeModel
{
public:
    /*!
     * @brief Constructor.
     */
    LayerCakeModel();
    
    /*!
     * @brief Destructor.
     */
    ~LayerCakeModel();
    
    /*!
     * @brief Clears the model.
     */
    void clear() noexcept;
    
    /*!
     * @brief This begins the model building process by setting the shallowmost
     *        layer in the earth model.  It will clear any previous model.
     */  
    void setTopLayer(const IsotropicLayer &layer);
    
    /*!
     * @brief Appends a layer to the model.  This can be called as many times
     *        as needed after calling \c setTopLayer().
     * @param[in] layer  The layer to append.
     * @throws std::runtime_error if \c setHalfspace() was called or if
     *         \c setTopLayer()
     * @throws std::invalid_argument if layer is not valid.
     */
    void appendLayer(const IsotropicLayer &layer);
    
    /*!
     * @brief Ends the model building process by setting the underling
     *        halfspace.
     */  
    void setHalfspace(const IsotropicLayer &layer);
    
    /*!
     * @brief Gets the number of layers in the model.
     * @result The number of layers in the model.
     */ 
    int getNumberOfLayers() const noexcept;
private:
    class LayerCakeModelImpl;
    std::unique_ptr<LayerCakeModelImpl> pImpl;
};
}

#endif
