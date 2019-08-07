#ifndef TEMBLOR_SOLVERS_RAYTRACE1D_ENUMS_HPP
#define TEMBLOR_SOLVERS_RAYTRACE1D_ENUMS_HPP 1
namespace Temblor::Solvers::RayTrace1D
{

/*!
 * @brief Defines the symmetry of the elastic moduli.
 */
enum class ElasticLayerSymmetry
{
    UNKNOWN,   /*!< Unknown symmetry. */
    ISOTROPIC  /*!< Isotropic symmetry. */
};
 
}
#endif
