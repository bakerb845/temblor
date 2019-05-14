#include <cstdio>
#include <cstdlib>
#include <cmath>
#include <GeographicLib/Geodesic.hpp>
#include <GeographicLib/Constants.hpp>
#include "temblor/library/utilities/geodetic/globalPositionPair.hpp"
#include "temblor/library/utilities/geodetic/globalPosition.hpp"

using namespace Temblor::Library::Utilities::Geodetic;

class GlobalPositionPair::GlobalPositionPairImpl
{
public: 
    /// Constructor
    GlobalPositionPairImpl() = default; 
    /// Destructor
    ~GlobalPositionPairImpl() = default;
    
    void computeDistAz()
    {
        if (mHaveDistAz){return;}
        if (!mHaveSource)
        {
            throw std::runtime_error("A valid source position not yet set");
        }
        if (!mHaveReceiver)
        {
            throw std::runtime_error("A valid receiver position not yet set");
        }
        double sourceLat = mSource.getLatitude();
        double sourceLon = mSource.getLongitude();
        double recvLat = mReceiver.getLatitude();
        double recvLon = mReceiver.getLongitude();
        // Vincenty computation for distance
        try
        {
            mGreatCircleDistance = geodesic.Inverse(sourceLat, sourceLon,
                                                    recvLat, recvLon,
                                                    mDistance, mAzimuth,
                                                    mBackAzimuth);
        }
        catch (const std::exception &e)
        {
            std::string errmsg = "Vincenty inverse failed "
                               + std::string(e.what());
            throw std::runtime_error(errmsg);
        }
        // Translate azimuth from [-180,180] to [0,360].
        if (mAzimuth < 0){mAzimuth = mAzimuth + 360;}
        // Translate azimuth from [-180,180] to [0,360] then convert to a 
        // back-azimuth by subtracting 180, i.e., +180.
        mBackAzimuth = mBackAzimuth + 180;
        mHaveDistAz = true;
    }
//private:
    GeographicLib::Geodesic geodesic{GeographicLib::Constants::WGS84_a(),
                                     GeographicLib::Constants::WGS84_f()};
    GlobalPosition mSource;
    GlobalPosition mReceiver;
    // Great-circle distance in degrees
    double mGreatCircleDistance = 0; 
    // Source-receiver distance in meters
    double mDistance = 0;
    // Source-receiver azimuth in degrees [0,360]
    double mAzimuth = 0;
    // Receiver-source azimuth in degrees [0,360]
    double mBackAzimuth = 0;
    bool mHaveSource = false;
    bool mHaveReceiver = false; 
    bool mHaveDistAz = false;
};

GlobalPositionPair::GlobalPositionPair() :
    pImpl(std::make_unique<GlobalPositionPairImpl> ())
{
}

GlobalPositionPair::GlobalPositionPair(const GlobalPositionPair &pair)
{
    *this = pair;
}

GlobalPositionPair::GlobalPositionPair(GlobalPositionPair &&pair) noexcept
{
    *this = std::move(pair);
}

GlobalPositionPair::GlobalPositionPair(const GlobalPosition &source,
                                       const GlobalPosition &receiver) :
    pImpl(std::make_unique<GlobalPositionPairImpl> ())
{
    GlobalPositionPair temp;
    temp.setSourcePosition(source);
    temp.setReceiverPosition(receiver);
    *this = temp;
}

GlobalPositionPair::~GlobalPositionPair() = default;

void GlobalPositionPair::clear() noexcept
{
    pImpl->mSource.clear();
    pImpl->mReceiver.clear();
    pImpl->mGreatCircleDistance = 0;  
    pImpl->mDistance = 0;
    pImpl->mAzimuth = 0;
    pImpl->mBackAzimuth = 0;
    pImpl->mHaveSource = false;
    pImpl->mHaveReceiver = false; 
    pImpl->mHaveDistAz = false;
}

GlobalPositionPair&
GlobalPositionPair::operator=(const GlobalPositionPair &pair)
{
    if (&pair == this){return *this;}
    pImpl = std::make_unique<GlobalPositionPairImpl> (*pair.pImpl);
    return *this;
}

GlobalPositionPair&
GlobalPositionPair::operator=(GlobalPositionPair &&pair) noexcept
{
    if (&pair == this){return *this;}
    pImpl = std::move(pair.pImpl);
    return *this;
}

void GlobalPositionPair::setSourcePosition(const GlobalPosition &source)
{
    pImpl->mHaveDistAz = false;
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

GlobalPosition GlobalPositionPair::getSourcePosition() const
{
    if (!pImpl->mHaveSource)
    {
        throw std::invalid_argument("Source position not yet set");
    }
    return pImpl->mSource;
}

void GlobalPositionPair::setReceiverPosition(const GlobalPosition &receiver)
{
    pImpl->mHaveDistAz = false;
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

GlobalPosition GlobalPositionPair::getReceiverPosition() const
{
    if (!pImpl->mHaveReceiver)
    {   
        throw std::invalid_argument("Receiver position not yet set");
    }   
    return pImpl->mReceiver;
}

double GlobalPositionPair::computeAzimuth() const
{
    pImpl->computeDistAz(); 
    return pImpl->mAzimuth;
}

double GlobalPositionPair::computeBackAzimuth() const
{
    pImpl->computeDistAz();
    return pImpl->mBackAzimuth;
}

double GlobalPositionPair::computeDistance() const
{
    pImpl->computeDistAz();
    return pImpl->mDistance;
}

double GlobalPositionPair::computeGreatCircleDistance() const
{
    pImpl->computeDistAz(); 
    return pImpl->mGreatCircleDistance;
}
