#include <cstdio>
#include <cstdlib>
#include <array>
#include <cmath>
#include "temblor/solvers/rayTrace1D/enums.hpp"
#include "temblor/solvers/rayTrace1D/layer.hpp"
#include "temblor/solvers/rayTrace1D/isotropicLayer.hpp"

using namespace Temblor::Solvers::RayTrace1D;

namespace
{

void fillIsotropicC(const double c11, 
                    const double c13,
                    const double c44,
                    std::array<double, 36> &cij)
{
    std::memset(cij.data(), 0, 36*sizeof(double));
    // Upper left - first column 
    cij[0] = c11;
    cij[1] = c13;
    cij[2] = c13;
    // Second column
    cij[6] = c13;
    cij[7] = c11;
    cij[8] = c13;
    // Third column
    cij[12] = c13;
    cij[13] = c13;
    cij[14] = c11;
    // Lower right of isotropic stiffness matrix
    cij[21] = c44;
    cij[28] = c44;
    cij[35] = c44;
}

}
 
class Layer::LayerImpl
{
public:
    class IsotropicLayer mIsotropicLayer;
    std::array<double, 36> mCij{0, 0, 0, 0, 0, 0, 
                                0, 0, 0, 0, 0, 0,
                                0, 0, 0, 0, 0, 0,
                                0, 0, 0, 0, 0, 0,
                                0, 0, 0, 0, 0, 0,
                                0, 0, 0, 0, 0, 0};
    double mRho = 0;
    double mThickness = 0;
    LayerSymmetry mSymmetry = LayerSymmetry::UNKNOWN;
};

/// Constructors
Layer::Layer() :
    pImpl(std::make_unique<LayerImpl> ())
{
}

Layer::Layer(const Layer &layer)
{
    *this = layer;
}

Layer::Layer(Layer &&layer) noexcept
{
    *this = std::move(layer);
}

/// Operators
Layer& Layer::operator=(const Layer &layer)
{
    if (&layer == this){return *this;}
    pImpl = std::make_unique<LayerImpl> (*layer.pImpl); 
    return *this;
}

Layer& Layer::operator=(Layer &&layer) noexcept
{
    if (&layer == this){return *this;}
    pImpl = std::move(layer.pImpl);
    return *this;
}

/// Destructors
Layer::~Layer() = default;

void Layer::clear() noexcept
{
    pImpl->mIsotropicLayer.clear();
    std::memset(pImpl->mCij.data(), 0, 36*sizeof(double));
    pImpl->mRho = 0;
    pImpl->mThickness = 0;
    pImpl->mSymmetry = LayerSymmetry::UNKNOWN;
}

/// Set an isotropic layer
void Layer::setLayer(const IsotropicLayer &layer)
{
    if (!layer.isValid())
    {
        throw std::invalid_argument("Layer is invalid\n");
    }
    // Compute elastic moduli
    double vp  = layer.getCompressionalVelocity();
    double vs  = layer.getShearVelocity();
    double rho = layer.getDensity();
    double mu = rho*(vs*vs);
    double lam = rho*(vp*vp) - 2.0*mu;
    double c11 = lam + 2*mu;
    double c13 = lam;
    double c44 = mu;
    pImpl->mIsotropicLayer = layer;
    fillIsotropicC(c11, c13, c44, pImpl->mCij);
    pImpl->mRho = rho;
    pImpl->mThickness = layer.getThickness();
    pImpl->mSymmetry = LayerSymmetry::ISOTROPIC;
}

/// Gets the layer symmetry
LayerSymmetry Layer::getSymmetry() const noexcept
{
    return pImpl->mSymmetry;
}

/// Gets an isotropic layer
IsotropicLayer Layer::getLayer() const
{
    if (pImpl->mSymmetry != LayerSymmetry::ISOTROPIC)
    {
        throw std::runtime_error("Layer is not isotropic\n");
    }
    // Create an isotropic layer
    return pImpl->mIsotropicLayer;
}
