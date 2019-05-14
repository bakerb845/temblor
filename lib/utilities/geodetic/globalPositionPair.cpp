#include <cstdio>
#include <cstdlib>
#include "temblor/library/utilities/geodetic/globalPositionPair.hpp"
#include "temblor/library/utilities/geodetic/globalPosition.hpp"

using namespace Temblor::Library::Utilities::Geodetic;

class GlobalPositionPair::GlobalPositionPairImpl
{
public:
    GlobalPosition mSource;
    GlobalPosition mReceiver;
    bool mHaveSource = false;
    bool mHaveReceiver = false; 
};

GlobalPositionPair::GlobalPositionPair() :
    pImpl(std::make_unique<GlobalPositionPairImpl> ())
{
}

GlobalPositionPair::GlobalPositionPair(const GlobalPositionPair &pair)
{
    *this = pair;
}

/*
GlobalPositionPair::GlobalPositionPair(const GlobalPositionPair &source,
                                       const GlobalPositionPair &receiver) :
    pImpl(std::make_unique<GlobalPositionPairImpl> ())
{
    //setPositionPair(p1, p2); 
}
*/

GlobalPositionPair::~GlobalPositionPair() = default;

GlobalPositionPair&
GlobalPositionPair::operator=(const GlobalPositionPair &pair)
{
    if (&pair == this){return *this;}
    pImpl = std::make_unique<GlobalPositionPairImpl> (*pair.pImpl);
    return *this;
}

void GlobalPositionPair::setSourcePosition(const GlobalPosition &source)
{
    pImpl->mHaveSource = false;
    if (!source.haveLatitude())
    {
        throw std::invalid_argument("Source position's latitude is not set");
    }
    if (!source.haveLongitude())
    {
        throw std::invalid_argument("Source position's longitude is not set");
    }
    pImpl->mHaveSource = true; 
    pImpl->mSource = source;
}

void GlobalPositionPair::setReceiverPosition(const GlobalPosition &receiver)
{
    pImpl->mHaveReceiver = false;
    if (!receiver.haveLatitude())
    {
        throw std::invalid_argument("Receiver position's latitude is not set");
    }
    if (!receiver.haveLongitude())
    {
        throw std::invalid_argument("Receiver position's longitude is not set");
    }
    pImpl->mHaveReceiver = true;
    pImpl->mReceiver = receiver;
}
