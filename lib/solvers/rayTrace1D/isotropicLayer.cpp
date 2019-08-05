#include <cstdio>
#include <cstdlib>
#include <string>
#include "temblor/solvers/rayTrace1D/isotropicLayer.hpp"

using namespace Temblor::Solvers::RayTrace1D;

class IsotropicLayer::IsotropicLayerImpl
{
public:
    double mVp =-1;
    double mVs =-1;
    double mRho =-1;
    double mThickness =-1;
};

IsotropicLayer::IsotropicLayer() :
    pImpl(std::make_unique<IsotropicLayerImpl> ())
{
}

IsotropicLayer::IsotropicLayer(const IsotropicLayer &layer)
{
    *this = layer;
}

IsotropicLayer::IsotropicLayer(IsotropicLayer &&layer) noexcept
{
    *this = std::move(layer);
}

IsotropicLayer& IsotropicLayer::operator=(const IsotropicLayer &layer)
{
    if (&layer == this){return *this;}
    pImpl = std::make_unique<IsotropicLayerImpl> (*layer.pImpl);
    return *this;
}

IsotropicLayer& IsotropicLayer::operator=(IsotropicLayer &&layer) noexcept
{
    if (&layer == this){return *this;}
    pImpl = std::move(layer.pImpl);
    return *this;
}

IsotropicLayer::~IsotropicLayer() = default;

void IsotropicLayer::clear() noexcept
{
    pImpl->mVp =-1;
    pImpl->mVs =-1;
    pImpl->mRho =-1;
    pImpl->mThickness =-1;
}

void IsotropicLayer::setCompressionalVelocity(const double vp)
{
    pImpl->mVp =-1;
    if (vp <= 0)
    {
        throw std::invalid_argument("Vp = " + std::to_string(vp)
                                  + "must be positive");
    }
    pImpl->mVp = vp;
}

double IsotropicLayer::getCompressionalVelocity() const
{
    if (pImpl->mVp < 0)
    {
        throw std::runtime_error("Vp was not set");
    }
    return pImpl->mVp;
}

void IsotropicLayer::setShearVelocity(const double vs)
{
    pImpl->mVs =-1;
    if (vs <= 0)
    {
        throw std::invalid_argument("Vs = " + std::to_string(vs)
                                  + " must be positive");
    }
    pImpl->mVs = vs;
}

double IsotropicLayer::getShearVelocity() const
{
    if (pImpl->mVs < 0)
    {
        throw std::runtime_error("Vs was not set");
    }
    return pImpl->mVs;
}

void IsotropicLayer::setDensity(const double rho)
{
    pImpl->mRho =-1;
    if (rho <= 0)
    {
        throw std::invalid_argument("Density = " + std::to_string(rho)
                                  + " must be positive");
    }
    pImpl->mRho = rho;
}

double IsotropicLayer::getDensity() const
{
   if (pImpl->mRho < 0)
   {
       throw std::runtime_error("Density was not set");
   }
   return pImpl->mRho;
}

void IsotropicLayer::setThickness(const double h)
{
   pImpl->mThickness =-1;
   if (h <= 0)
   {
       throw std::invalid_argument("Thickness = " + std::to_string(h)
                                 + " must be postiive");
   }
   pImpl->mThickness = h;
}

double IsotropicLayer::getThickness() const
{
   if (pImpl->mThickness < 0)
   {
       throw std::invalid_argument("Thickness not set");
   }
   return pImpl->mThickness;
}

bool IsotropicLayer::isValid() const noexcept
{
    if (pImpl->mVp <= 0){return false;}
    if (pImpl->mVp <= pImpl->mVs){return false;}
    if (pImpl->mRho <= 0){return false;}
    if (pImpl->mThickness <= 0){return false;}
    return true;
}

void IsotropicLayer::print(FILE *handle) noexcept
{
    if (!isValid()){return;}
    FILE *ftemp = stdout;
    if (handle){ftemp = handle;}
    fprintf(ftemp, "Vp: %lf (m/s)", pImpl->mVp);
    fprintf(ftemp, "Vs: %lf (m/s)", pImpl->mVs);
    fprintf(ftemp, "Density: %lf (kg/m**3)", pImpl->mRho);
    fprintf(ftemp, "Thickness: %lf (m)", pImpl->mThickness);
}

