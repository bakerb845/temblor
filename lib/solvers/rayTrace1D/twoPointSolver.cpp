#include <cstdio>
#include <cstdlib>
#include <string>
#include "temblor/solvers/rayTrace1D/twoPointSolver.hpp"
#include "temblor/solvers/rayTrace1D/isotropicLayer.hpp"
#include "temblor/solvers/rayTrace1D/isotropicLayerCakeModel.hpp"

using namespace Temblor::Solvers::RayTrace1D;

class TwoPointSolver::TwoPointSolverImpl
{
public:
    IsotropicLayerCakeModel mIsoModel; 
    double mSourceDepth = 0;
    double mReceiverDepth = 0;
    double mReceiverOffset = 0;
    int mSourceLayer = 0;
    int mReceiverLayer = 0;
    bool mUseIsotropicSolver = true;
    bool mHaveModel = false;
};

TwoPointSolver::TwoPointSolver() :
    pImpl(std::make_unique<TwoPointSolverImpl> ())
{
}

/// Destructor
TwoPointSolver::~TwoPointSolver() = default;

/// Clears the class
void TwoPointSolver::clear() noexcept
{
    pImpl->mIsoModel.clear();
    pImpl->mSourceDepth = 0;
    pImpl->mReceiverDepth = 0;
    pImpl->mReceiverOffset = 0;
    pImpl->mSourceLayer = 0;
    pImpl->mReceiverLayer = 0;
    pImpl->mUseIsotropicSolver = true;
    pImpl->mHaveModel = false;
}

/// Sets the structural model
void TwoPointSolver::setEarthModel(const IsotropicLayerCakeModel &model)
{
    if (!model.isValid())
    {
        throw std::invalid_argument("Model is invalid\n");
    }
    pImpl->mIsoModel = model;
    pImpl->mSourceDepth = 0;
    pImpl->mReceiverDepth = 0;
    pImpl->mReceiverOffset = 0;
    pImpl->mSourceLayer = 0;
    pImpl->mReceiverLayer = 0;
    pImpl->mUseIsotropicSolver = true;
    pImpl->mHaveModel = true;
}

/// Sets the source/receiver geometry
void TwoPointSolver::setSourceDepth(const double sourceDepth)
{
    if (!pImpl->mHaveModel)
    {
        throw std::invalid_argument("Velocity model not yet set");
    }
    if (sourceDepth < 0)
    {
        throw std::invalid_argument("Source depth = "
                                  + std::to_string(sourceDepth)
                                  + " must be positive\n");
    }
    auto layer = pImpl->mIsoModel.getLayerIndexFromDepth(sourceDepth);
    pImpl->mSourceDepth = sourceDepth;
    pImpl->mSourceLayer = layer;
}

void TwoPointSolver::setSourceDepthToFreeSurface() noexcept
{
    pImpl->mSourceDepth = 0;
    pImpl->mSourceLayer = 0;
}

void TwoPointSolver::setReceiverDepth(const double receiverDepth)
{
    if (!pImpl->mHaveModel)
    {
        throw std::invalid_argument("Velocity model not yet set");
    }
    if (receiverDepth < 0)
    {
        throw std::invalid_argument("Receiver depth = "
                                  + std::to_string(receiverDepth)
                                  + " must be positive\n");
    }
    auto layer = pImpl->mIsoModel.getLayerIndexFromDepth(receiverDepth);
    pImpl->mReceiverDepth = receiverDepth;
    pImpl->mReceiverLayer = layer;
}

void TwoPointSolver::setReceiverDepthToFreeSurface() noexcept
{
    pImpl->mReceiverDepth = 0;
    pImpl->mReceiverLayer = 0;
}

void TwoPointSolver::setReceiverOffset(const double receiverOffset)
{
    if (receiverOffset < 0)
    {
        throw std::invalid_argument("Receiver offset = "
                                  + std::to_string(receiverOffset)
                                  + " must be positive\n");
    } 
    pImpl->mReceiverOffset = receiverOffset;
}

/// Compute the direct arrival
void TwoPointSolver::computeDirectRay()
{
    if (!pImpl->mHaveModel)
    {
        throw std::runtime_error("The earth model is not yet set\n");
    }
/*
    double zs = pImpl->mSourceDepth.first;
    double zr = pImpl->mReceiverDepth.first;
    int sourceLayer = pImpl->mSourceDepth.second;
    int receiverLayer = pImpl->mReceiverDepth.second;
    // Handle the easy case where source/receiver are in the same layer
    if (sourceLayer == receiverLayer)
    {
        // Horizontal slowness
         
        if (pImpl->mSourceDepth.first == pImpl->mReceiverDepth.first)
        
    }
    // Do general optimization for ray parameter
*/
}

