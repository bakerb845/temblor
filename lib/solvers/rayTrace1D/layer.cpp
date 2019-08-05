#include <cstdio>
#include <cstdlib>
#include <array>
#include "temblor/solvers/rayTrace1D/layer.hpp"
#include "temblor/solvers/rayTrace1D/isotropicLayer.hpp"

using namespace Temblor::Solvers::RayTrace1D;

namespace cij2index(const int i, const int j)
{
    return 6*i + j;
}
 
class Layer::LayerImpl
{
public:
    std::array<double, 36> mCij{0, 0, 0, 0, 0, 0, 
                                0, 0, 0, 0, 0, 0,
                                0, 0, 0, 0, 0, 0,
                                0, 0, 0, 0, 0, 0,
                                0, 0, 0, 0, 0, 0,
                                0, 0, 0, 0, 0, 0};
    double mRho = 0;
    double mThickness = 0;
    Symmetry mSymmetry = Symmetry::UNKNOWN;
}

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

/// Gets the layer symmetry
Symmetry Layer::getSymmetry() const noexcept
{
    return pImpl->mSymmetry;
}
