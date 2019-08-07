#include <cstdio>
#include <cstdlib>
#include <cmath>
#include <string>
#include "temblor/solvers/rayTrace1D/raySegment.hpp"

using namespace Temblor::Solvers::RayTrace1D;

class RaySegment::RaySegmentImpl
{
public:
    std::pair<double, double> mStartPoint{0, 0};
    std::pair<double, double> mEndPoint{0, 0};
    double mTravelTime =-1;
    double mDistance =-1;
    double mVelocity =-1;
    bool mHaveStartPoint = false;
    bool mHaveEndPoint = false;
};

/// Constructors
RaySegment::RaySegment() :
    pImpl(std::make_unique<RaySegmentImpl> ())
{
}

RaySegment::RaySegment(const std::pair<double, double> &startPoint,
                       const std::pair<double, double> &endPoint,
                       const double velocity) :
    pImpl(std::make_unique<RaySegmentImpl> ())
{
    setVelocity(velocity); // Throws
    setStartPoint(startPoint);
    setEndPoint(endPoint);
    getTravelTime(); // Computes the distance and travel time
}

RaySegment::RaySegment(const RaySegment &segment)
{
    *this = segment;
}

RaySegment::RaySegment(RaySegment &&segment) noexcept
{
    *this = std::move(segment);
}

/// Operators
RaySegment& RaySegment::operator=(const RaySegment &segment)
{
    if (&segment == this){return *this;}
    pImpl = std::make_unique<RaySegmentImpl> (*segment.pImpl);
    return *this;
}

RaySegment& RaySegment::operator=(RaySegment &&segment) noexcept
{
    if (&segment == this){return *this;}
    pImpl = std::move(segment.pImpl);
    return *this;
}

/// Destructors
RaySegment::~RaySegment() = default;

void RaySegment::clear() noexcept
{
    pImpl->mStartPoint = std::make_pair<double, double> (0, 0);
    pImpl->mEndPoint = std::make_pair<double, double> (0, 0);
    pImpl->mTravelTime =-1;
    pImpl->mDistance =-1;
    pImpl->mVelocity =-1;
    pImpl->mHaveStartPoint = false;
    pImpl->mHaveEndPoint = false;
}

/// Start point
void RaySegment::setStartPoint(
    const std::pair<double, double> &startPoint) noexcept
{
    pImpl->mStartPoint = startPoint;
    pImpl->mHaveStartPoint = true;
    pImpl->mDistance =-1;
    pImpl->mTravelTime =-1;
}

std::pair<double, double> RaySegment::getStartPoint() const
{
    if (!pImpl->mHaveStartPoint)
    {
        throw std::runtime_error("Start point not yet set\n");
    }
    return pImpl->mStartPoint;
}

/// End point
void RaySegment::setEndPoint(
    const std::pair<double, double> &endPoint) noexcept
{
    pImpl->mEndPoint = endPoint;
    pImpl->mHaveEndPoint = true;
    pImpl->mDistance =-1;
    pImpl->mTravelTime =-1;
}

std::pair<double, double> RaySegment::getEndPoint() const
{
    if (!pImpl->mHaveEndPoint)
    {
        throw std::runtime_error("End point not yet set\n");
    }
    return pImpl->mEndPoint;
}

/// Velocity
void RaySegment::setVelocity(const double velocity)
{
    pImpl->mVelocity =-1;
    pImpl->mTravelTime =-1;
    if (velocity <= 0)
    {
        throw std::invalid_argument("Velocity = "
                                  + std::to_string(velocity)
                                  + " must be positive\n");
    }
    pImpl->mVelocity = velocity;
}

double RaySegment::getVelocity() const
{
    if (pImpl->mVelocity <= 0)
    {
        throw std::runtime_error("Velocity not yet set\n");
    }
    return pImpl->mVelocity;
}

/// Distance
double RaySegment::getDistance() const
{
    if (!pImpl->mHaveStartPoint)
    {
        throw std::runtime_error("Start point not yet set\n");
    }
    if (!pImpl->mHaveEndPoint)
    {
        throw std::runtime_error("End point not yet set\n");
    }
    if (pImpl->mDistance < 0)
    {
        double dx = pImpl->mEndPoint.first  - pImpl->mStartPoint.first;
        double dz = pImpl->mEndPoint.second - pImpl->mStartPoint.second;
        pImpl->mDistance = hypot(dx, dz);
    }
    return pImpl->mDistance;
}

/// Travel time
double RaySegment::getTravelTime() const
{
    if (pImpl->mTravelTime < 0)
    {
        double velocity = getVelocity(); // Throws
        double distance = getDistance(); // Throws
        pImpl->mTravelTime = distance/velocity;
    }
    return pImpl->mTravelTime;
}
