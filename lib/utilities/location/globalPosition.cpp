#include <cstdio>
#include <cstdlib>
#include "temblor/library/utilities/location/globalPosition.hpp"

using namespace Temblor::Library::Utilities::Location;

class GlobalPosition::GlobalPositionImpl
{
public:
    double mLatitude = 0;
    double mLongitude = 0;
    bool mHavePosition = false;
};

GlobalPosition::GlobalPosition() :
    pImpl(std::make_unique<GlobalPositionImpl> ())
{
}

//GlobalPosition::~GlobalPosition = default;

