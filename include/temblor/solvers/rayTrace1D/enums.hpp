#ifndef TEMBLOR_SOLVERS_RAYTRACE1D_ENUMS_HPP
#define TEMBLOR_SOLVERS_RAYTRACE1D_ENUMS_HPP 1
namespace Temblor::Solvers::RayTrace1D
{

/*!
 * @brief Defines the layer's symmetry.
 */
enum class LayerSymmetry
{
    UNKNOWN,   /*!< Unknown symmetry. */
    ISOTROPIC  /*!< Isotropic symmetry. */
};
 
}
#endif
