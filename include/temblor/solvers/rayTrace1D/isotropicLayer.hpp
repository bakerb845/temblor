#ifndef TEMBLOR_SOLVERS_RAYTRACE1D_ISOTROPICLAYER_HPP
#define TEMBLOR_SOLVERS_RAYTRACE1D_ISOTROPICLAYER_HPP
#include <memory>

namespace Temblor::Solvers::RayTrace1D
{
/*!
 * @brief Defines an isotropic layer in the layer-cake velocity model.
 */
class IsotropicLayer
{
public:
    /*! 
     * @brief Constructor.
     */
    IsotropicLayer();
    /*! 
     * @brief Copy constructor.
     * @param[in] layer  The isotropic layer to initialize from.
     */
    IsotropicLayer(const IsotropicLayer &layer);
    /*! 
     * @brief Move constructor.
     * @param[in,out] layer  The isotropic layer to initialize from.
     *                       On exit layer's behavior is undefined.
     */
    IsotropicLayer(IsotropicLayer &&layer) noexcept;

    /*! 
     * @brief Copy operator.
     * @param[in] layer  The isotropic layer to copy.
     * @result A deep copy of the input layer.
     */
    IsotropicLayer& operator=(const IsotropicLayer &layer);
    /*! 
     * @brief Move operator.
     * @param[in,out] layer  The isotropic layer to move to this.
     *                       On exit layer's behavior is undefined.
     * @result The memory of layer moved to this.
     */
    IsotropicLayer& operator=(IsotropicLayer &&layer) noexcept;

    /*! 
     * @brief Destructor.
     */
    ~IsotropicLayer();
    /*! 
     * @brief Clears the model.
     */
    void clear() noexcept;

    /*!
     * @brief Sets the compressional velocity in the layer.
     * @param[in] vp   The compressional velocity in meters/second.
     * @throws std::invalid_argument if vp is not positive.
     */
    void setCompressionalVelocity(const double vp);
    /*!
     * @brief Gets the compressional velocity in the layer.
     * @result The compressional velocity in meters/second.
     * @throws std::runtime_error if the compressional velocity was not set.
     */
    double getCompressionalVelocity() const;
    /*!
     * @brief Sets the shear velocity in the layer.
     * @param[in] vs   The shear velocity in meters/second.
     * @throws std::invalid_argument if vs is not positive.
     */
    void setShearVelocity(const double vs);
    /*!
     * @brief Gets the shear velocity in the layer.
     * @result The shear velocity in meters/second.
     * @throws std::invalid_argument if the shear velocity was not set.
     */
    double getShearVelocity() const;
    /*!
     * @brief Sets the density in the layer.
     * @param[in] rho  The density in kilograms/meter**3.
     * @throws std;:invalid_argument if rho is not positive.
     */
    void setDensity(const double rho);
    /*!
     * @brief Gets the density in the layer.
     * @result The density in kilograms/meter**3.
     * @throws std::runtime_error if the density was not set.
     */
    double getDensity() const;
    /*!
     * @brief Sets the layer thickness.
     * @param[in] h   The layer thickness in meters.
     * @throws std::invalid_argument if the thickness is not positive.
     */
    void setThickness(const double h);
    /*!
     * @brief Gets the layer thickness.
     * @result The layer thickness in meters.
     * @throws std::runtime_error if the thickness was not set.
     */
    double getThickness() const;
    /*!
     * @brief Checks if this is a valid layer.
     * @param[in] True indicates that this is a physically sensible layer.
     */
    bool isValid() const noexcept;

    /*!
     * @brief Prints the layer properites.
     * @param[in] handle  The file handle to print to.  If this is NULL then 
     *                    this will print to stdout.
     */
    void print(FILE *handle = nullptr) noexcept;
private:
    class IsotropicLayerImpl;
    std::unique_ptr<IsotropicLayerImpl> pImpl;
};
}

#endif
