#include <cstdio>
#include <cstdlib>
#include <stdexcept>
#include <algorithm>
#include <GeographicLib/UTMUPS.hpp>
#include "temblor/utilities/geodetic/globalPosition.hpp"

using namespace Temblor::Utilities::Geodetic;

class GlobalPosition::GlobalPositionImpl
{
public:
    void computeUTM()
    {
        if (mHaveUTM){return;}
        if (!mHaveLatitude || !mHaveLongitude)
        {
            if (!mHaveLatitude){throw std::runtime_error("Latitude not set");}
            throw std::runtime_error("Longitude not set");
        }
        bool northp;
        try
        {
            GeographicLib::UTMUPS::Forward(mLatitude, mLongitude,
                                           mZone, northp, mEasting, mNorthing);
        }
        catch (const std::exception &e)
        {
            std::runtime_error(e.what());
        }
        mHaveUTM = true;
    }

    double mLatitude = 0;
    double mLongitude = 0;
    double mDepth = 0;
    double mEasting = 0;
    double mNorthing = 0;
    int mZone =-1;
    bool mHaveLatitude = false;
    bool mHaveLongitude = false;
    bool mHaveDepth = false;
    bool mHaveUTM = false;
};

GlobalPosition::GlobalPosition() :
    pImpl(std::make_unique<GlobalPositionImpl> ())
{
}

GlobalPosition::GlobalPosition(const double latitude,
                               const double longitude) :
    pImpl(std::make_unique<GlobalPositionImpl> ()) 
{
    GlobalPosition temp;
    temp.setLatitude(latitude);   // Will throw
    temp.setLongitude(longitude); // Will throw
    // The arguments are valid - copy
    *this = temp;
}

GlobalPosition::GlobalPosition(const double latitude,
                               const double longitude,
                               const double depth) :
    pImpl(std::make_unique<GlobalPositionImpl> ())
{
    GlobalPosition temp;
    temp.setLatitude(latitude);   // Will throw
    temp.setLongitude(longitude); // Will throw
    temp.setDepth(depth);
    // The arguments are valid - copy
    *this = temp;
}

GlobalPosition::GlobalPosition(const GlobalPosition &position)
{
    *this = position;
}

GlobalPosition::GlobalPosition(GlobalPosition &&position)
{
    *this = std::move(position);
}

GlobalPosition::~GlobalPosition() = default;

void GlobalPosition::clear() noexcept
{
    pImpl->mLatitude = 0;
    pImpl->mLongitude = 0;
    pImpl->mDepth = 0;
    pImpl->mEasting = 0;
    pImpl->mNorthing = 0;
    pImpl->mZone =-1;
    pImpl->mHaveLatitude = false;
    pImpl->mHaveLongitude = false;
    pImpl->mHaveDepth = false;
    pImpl->mHaveUTM = false;
}

GlobalPosition& GlobalPosition::operator=(const GlobalPosition &position)
{
    if (&position == this){return *this;}
    pImpl = std::make_unique<GlobalPositionImpl> (*position.pImpl);
    return *this;
}

GlobalPosition& GlobalPosition::operator=(GlobalPosition &&position)
{
    if (&position == this){return *this;}
    pImpl = std::move(position.pImpl);
    return *this;
}

// Depth
bool GlobalPosition::haveDepth() const noexcept
{
    return pImpl->mHaveDepth;
}

void GlobalPosition::setDepth(const double depth) noexcept
{
    pImpl->mHaveDepth = true;
    pImpl->mDepth = depth;
}

double GlobalPosition::getDepth() const
{
    if (!pImpl->mHaveLongitude)
    {
        throw std::runtime_error("Depth not set");
    }
    return pImpl->mDepth;
}

// Longitude
bool GlobalPosition::haveLongitude() const noexcept
{
    return pImpl->mHaveLongitude;
}

void GlobalPosition::setLongitude(const double longitude)
{
    pImpl->mHaveLongitude = false;
    pImpl->mHaveUTM = false;
    if (longitude <-540 || longitude > 540)
    {
        std::string errmsg = "Longitude = " + std::to_string(longitude)
                           + " must be in the range (-540,540)";
    }
    // Deal with modularity
    double longitudeUse = longitude;
    if (longitudeUse < 0)
    {
        for (auto k=0; k<2; ++k)
        {
            longitudeUse = longitudeUse + 360;
            if (longitudeUse >= 0){break;}
        }
    }
    if (longitudeUse > 360)
    {
        for (auto k=0; k<2; ++k)
        {
            longitudeUse = longitudeUse - 360;
            if (longitudeUse < 360){break;}
        }
    }
    // Force into interval 
    longitudeUse = std::min(360.0, std::max(0.0, longitudeUse));
    pImpl->mHaveLongitude = true;
    pImpl->mLongitude = longitudeUse;
}

double GlobalPosition::getLongitude() const
{
    if (!pImpl->mHaveLongitude)
    {
        throw std::runtime_error("Longitude not set");
    }
    return pImpl->mLongitude;
}

// Latitude
bool GlobalPosition::haveLatitude() const noexcept
{
    return pImpl->mHaveLatitude;
}

void GlobalPosition::setLatitude(const double latitude)
{
    pImpl->mHaveLatitude = false;
    pImpl->mHaveUTM = false;
    if (latitude < -90 || latitude > 90)
    {
        std::string errmsg = "Latitude = " + std::to_string(latitude)
                           + " must be in range [-90,90]";
        throw std::runtime_error(errmsg);
    }
    pImpl->mHaveLatitude = true;
    pImpl->mLatitude = latitude;
}

double GlobalPosition::getLatitude() const
{
    if (!pImpl->mHaveLatitude)
    {
        throw std::runtime_error("Latitude not set");
    }
    return pImpl->mLatitude;
}

/// UTM info
double GlobalPosition::computeEasting() const
{
    pImpl->computeUTM();
    return pImpl->mEasting;
}

double GlobalPosition::computeNorthing() const
{
    pImpl->computeUTM();
    return pImpl->mNorthing;
}

int GlobalPosition::computeUTMZone() const
{
    pImpl->computeUTM();
    return pImpl->mZone;
}
