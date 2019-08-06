#include <cstdio>
#include <cstdlib>
#include "temblor/solvers/rayTrace1D/twoPoint.hpp"
#include "temblor/solvers/rayTrace1D/isotropicLayer.hpp"
#include "temblor/solvers/rayTrace1D/isotropicLayerCakeModel.hpp"

using namespace Temblor::Solvers::RayTrace1D;

class TwoPointRayTracer1D::TwoPointRayTracer1DImpl
{
public:
 
}

TwoPointRayTracer1D::TwoPointRayTracer() :
    pImpl(std::make_unique<TwoPointRayTracer1DImpl> ())
{
}

TwoPointRayTracer1D::~TwoPointRayTracer1D() = default;

void TwoPointRayTracer1D::
