#include <cstdio>
#include <cstdlib>
#include <string>
#include <vector>
#include "temblor/library/solvers/rayTrace1D/layerCakeModel.hpp"
#include "temblor/library/solvers/rayTrace1D/isotropicLayer.hpp"

using namespace Temblor::Library::Solvers::RayTrace1D;

class LayerCakeModel::LayerCakeModelImpl
{
public:
    std::vector<class IsotropicLayer> mModel;
};

LayerCakeModel::LayerCakeModel() :
    pImpl(std::make_unique<LayerCakeModelImpl> ())
{
}   

LayerCakeModel::~LayerCakeModel() = default;

int LayerCakeModel::getNumberOfLayers() const noexcept
{   
    return static_cast<int> (pImpl->mModel.size());
}   

void LayerCakeModel::clear() noexcept
{   
    pImpl->mModel.clear();
}   

void LayerCakeModel::appendLayer(const IsotropicLayer &layer)
{
    if (!layer.isValid())
    {
        throw std::invalid_argument("Layer is invalid");
    }
    pImpl->mModel.push_back(layer);
}
