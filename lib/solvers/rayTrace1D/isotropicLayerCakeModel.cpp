#include <cstdio>
#include <cstdlib>
#include <string>
#include <cassert>
#include <cfloat>
#include <vector>
#include "temblor/solvers/rayTrace1D/isotropicLayerCakeModel.hpp"
#include "temblor/solvers/rayTrace1D/isotropicLayer.hpp"

using namespace Temblor::Solvers::RayTrace1D;

class IsotropicLayerCakeModel::IsotropicLayerCakeModelImpl
{
public:
    std::vector<class IsotropicLayer> mModel;
    std::vector<double> mInterfaces;
    bool mHaveTopLayer = false;
    bool mCanAppendLayer = false;
};

/// Constructor
IsotropicLayerCakeModel::IsotropicLayerCakeModel() :
    pImpl(std::make_unique<IsotropicLayerCakeModelImpl> ())
{
}

IsotropicLayerCakeModel::IsotropicLayerCakeModel(
    const IsotropicLayerCakeModel &model)
{
    *this = model;
}

IsotropicLayerCakeModel::IsotropicLayerCakeModel(
    IsotropicLayerCakeModel &&model) noexcept
{
    *this = std::move(model);
}

/// Operators
IsotropicLayerCakeModel&
IsotropicLayerCakeModel::operator=(const IsotropicLayerCakeModel &model)
{
    if (&model == this){return *this;}
    pImpl = std::make_unique<IsotropicLayerCakeModelImpl> (*model.pImpl);
    return *this;
}

IsotropicLayerCakeModel&
IsotropicLayerCakeModel::operator=(IsotropicLayerCakeModel &&model) noexcept
{
    if (&model == this){return *this;}
    pImpl = std::move(model.pImpl);
    return *this;
}

/// Destructor
IsotropicLayerCakeModel::~IsotropicLayerCakeModel() = default;

/// Clears the model
void IsotropicLayerCakeModel::clear() noexcept
{   
    pImpl->mModel.clear();
    pImpl->mInterfaces.clear();
    pImpl->mHaveTopLayer = false;
    pImpl->mCanAppendLayer = false;
}

/// Sets the top layer
void IsotropicLayerCakeModel::setTopLayer(const IsotropicLayer &layer,
                                          const bool lLastLayer)
{
    clear();
    if (!layer.isValid())
    {
        throw std::invalid_argument("Layer is invalid\n");
    }
    pImpl->mModel.push_back(layer);
    pImpl->mInterfaces.push_back(0); // Start at 0
    pImpl->mHaveTopLayer = true;
    pImpl->mCanAppendLayer = true;
    if (!lLastLayer)
    {
        pImpl->mInterfaces.push_back(layer.getThickness());
    }
    else
    {
        closeModelConstruction();
    }
}

/// Appends a layer to the model
void IsotropicLayerCakeModel::appendLayer(const IsotropicLayer &layer,
                                          const bool lLastLayer)
{
    if (!pImpl->mHaveTopLayer)
    {
        throw std::runtime_error("Must set top layer of model\n");
    }
    if (!canAppendLayer())
    {
        throw std::runtime_error("Haflspace set - can't append layer\n");
    } 
    if (!layer.isValid())
    {
        throw std::invalid_argument("Layer is invalid\n");
    }
    pImpl->mModel.push_back(layer);
    // Add current layer thickness to interface depths 
    pImpl->mInterfaces.push_back(layer.getThickness()
                               + pImpl->mInterfaces.back());
    if (lLastLayer){closeModelConstruction();}
}

/// Appends a critically refracting layer
/*
void IsotropicLayerCakeModel::appendCriticallyRefractingLayer(const IsotropicLayer &layer, const bool lLastLayer)
{
    appendLayer(layer, lLastLayer);
}

/// Appends a reflecting layer
void IsotropicLayerCakeModel::appendReflectingLayer( )
{
    appendLayer(layer, lLastLayer);
}
*/

void IsotropicLayerCakeModel::closeModelConstruction()
{
    if (!pImpl->mHaveTopLayer)
    {
        throw std::runtime_error("Must set top layer of model\n");
    }
    pImpl->mInterfaces.push_back(DBL_MAX - 1);
    pImpl->mCanAppendLayer = false;
}

/// Gets the layer index corresponding to the depth
int IsotropicLayerCakeModel::getLayerIndexFromDepth(const double depth) const
{
    int nLayers = getNumberOfLayers();
    if (nLayers < 1)
    {
        throw std::runtime_error("No layers in model\n");
    }
    if (depth < 0)
    {
        throw std::invalid_argument("depth = " + std::to_string(depth)
                                  + " must be non-negative\n");
    }
    double depthMax = pImpl->mInterfaces.back();
    if (depth > depthMax)
    {
        throw std::invalid_argument("depth = " + std::to_string(depth)
                                  + " exceeds max depth = " 
                                  + std::to_string(depthMax) + "\n");
    }
    // Edge case - frequently will happen when querying free surface
    if (depth == 0){return 0;}
    // Only one layer - it can't be anywhere else
    if (nLayers == 1){return 0;}
    // In last layer
    //if (depth >= pImpl->mInterfaces[nLayers]){return nLayers - 1;}
    // Hunt for it
    auto layerPtr = std::upper_bound(pImpl->mInterfaces.begin(),
                                     pImpl->mInterfaces.end(),
                                     depth);
    int layer = static_cast<int> (layerPtr - pImpl->mInterfaces.begin()) - 1;
#ifdef DEBUG
    assert(layer >= 0 && layer < nLayers);
    assert(pImpl->mInterfaces[layer] <= depth &&
           depth < pImpl->mInterfaces[layer+1]);
#endif
/*
    for (int i=0; i<nLayers; ++i)
    {
        if (pImpl->mInterfaces[i] <= depth && depth < pImpl->mInterfaces[i+1])
        {
            return i;
        }
    }
    assert(false, "Shouldn't be here");
    return -1;
*/
    return layer;
}

/// Gets a layer
IsotropicLayer IsotropicLayerCakeModel::getLayer(const int layerIndex) const
{
    int nLayers = getNumberOfLayers();
    if (nLayers < 1)
    {
        throw std::runtime_error("No layers in model\n");
    }
    if (layerIndex < 0 || layerIndex >= nLayers)
    {
        throw std::invalid_argument("layerIndex = " 
                                  + std::to_string(layerIndex)
                                  + " must be in range [0," 
                                  + std::to_string(nLayers-1)  + "]\n");
    }
    return pImpl->mModel[layerIndex];
}

/// Gets number of layers in model
int IsotropicLayerCakeModel::getNumberOfLayers() const noexcept
{
    return static_cast<int> (pImpl->mModel.size());
}

/// Can append a layer
bool IsotropicLayerCakeModel::canAppendLayer() const noexcept
{
    return pImpl->mCanAppendLayer;
}

/// Determines if the model 
bool IsotropicLayerCakeModel::isValid() const noexcept
{
    if (getNumberOfLayers() < 1){return false;} // No layers
    for (auto i=0; i<getNumberOfLayers(); ++i)
    {
        if (!pImpl->mModel[i].isValid()){return false;}
    }
    if (canAppendLayer()){return false;} // Model is not closed
    return true;
}
