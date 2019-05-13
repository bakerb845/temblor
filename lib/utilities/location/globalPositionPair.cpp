#include <cstdio>
#include <cstdlib>
#include "temblor/library/utilities/location/globalPositionPair.hpp"
#include "temblor/library/utilities/location/globalPosition.hpp"

using namespace Temblor::Library::Utilities::Location;

class GlobalPositionPair::GlobalPositionPairImpl
{
public:
    GlobalPosition mPosition1;
    GlobalPosition mPosition2;
    
};

GlobalPositionPair::GlobalPositionPair() :
    pImpl(std::make_unique<GlobalPositionPairImpl> ())
{
}

/*
GlobalPositionPair::GlobalPositionPair(const GlobalPositionPair &p1,
                                       const GlobalPositionPair &p2) :
    pImpl(std::make_unique<GlobalPositionPairImpl> ())
{
    //setPositionPair(p1, p2); 
}
*/

GlobalPositionPair::~GlobalPositionPair() = default;

/*
void GlobalPositionPair::setPair(const GlobalPositionPair &p1,
                                 const GlobalPositionPair &p2)
{
}
*/
