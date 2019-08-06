#include <cstdio>
#include <cstdlib>
#include <string>
#include <vector>
#include "temblor/solvers/rayTrace1D/isotropicLayerCakeModel.hpp"
#include "temblor/solvers/rayTrace1D/isotropicLayer.hpp"

using namespace Temblor::Solvers::RayTrace1D;

class IsotropicLayerCakeModel::IsotropicLayerCakeModelImpl
{
public:
    std::vector<class IsotropicLayer> mModel;
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
    pImpl->mHaveTopLayer = true;
    pImpl->mCanAppendLayer = !lLastLayer;
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
    pImpl->mCanAppendLayer = !lLastLayer;
}

/// Appends a critically refracting layer
/*
void IsotropicLayerCakeModel::appendCriticallyRefractingLayer( )
{

}

/// Appends a reflecting layer
void IsotropicLayerCakeModel::appendReflectingLayer( )
{

}
*/

/// Gets a layer
IsotropicLayer IsotropicLayerCakeModel::getLayer(const int layerIndex)
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
    //if (canAppendLayer()){return false;} // Model is not closed
    return true;
}
