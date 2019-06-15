#include <cstdio>
#include <cstdlib>
#include <string>
#include "temblor/library/models/event/origin.hpp"
#include "temblor/library/utilities/geodetic/globalPosition.hpp"
#include "temblor/library/utilities/time.hpp"

using namespace Temblor::Library::Models::Event;

class Origin::OriginImpl
{
public:
    Temblor::Library::Utilities::Geodetic::GlobalPosition mHypocenter;
    Temblor::Library::Utilities::Time mOriginTime;
    std::string mOriginName;
    bool mHaveHypocenter = false;
    bool mHaveOriginTime = false;
};

/// Constructors
Origin::Origin() :
    pImpl(std::make_unique<OriginImpl> ())
{
}

Origin::Origin(const Origin &origin)
{
    *this = origin;
}

Origin::Origin(class Origin &&origin) noexcept
{
    *this = std::move(origin);
}

/// Operators
Origin& Origin::operator=(const Origin &origin)
{
    if (&origin == this){return *this;}
    pImpl = std::make_unique<OriginImpl> (*origin.pImpl);
    return *this;
}

Origin& Origin::operator=(class Origin &&origin) noexcept
{
    if (&origin == this){return *this;}
    pImpl = std::move(origin.pImpl);
    return *this;
}    

/// Destructors
Origin::~Origin() = default;

/// Clears the origin
void Origin::clear() noexcept 
{
    pImpl->mOriginName.clear();
    pImpl->mHypocenter.clear();
    pImpl->mOriginTime.clear();
    pImpl->mHaveHypocenter = false;
    pImpl->mHaveOriginTime = false;
}

/// Sets the origin name
void Origin::setName(const std::string &name)
{
    pImpl->mOriginName = name;
}

/// Gets the origin name
std::string Origin::getName() const noexcept
{
    return pImpl->mOriginName;
}

/// Sets the origin origin time
void Origin::setTime(const Temblor::Library::Utilities::Time &originTime)
{
    pImpl->mHaveOriginTime = true;
    pImpl->mOriginTime = originTime;
}

/// Gets the origin origin time
Temblor::Library::Utilities::Time Origin::getTime() const
{
    if (!pImpl->mHaveOriginTime)
    {
        throw std::runtime_error("Origin time not set");
    }
    return pImpl->mOriginTime;
}

/// Sets the origin hypocenter
void Origin::setHypocenter(
    const Temblor::Library::Utilities::Geodetic::GlobalPosition &hypocenter)
{
    pImpl->mHaveHypocenter = false;
    if (!hypocenter.haveLongitude())
    {
        throw std::invalid_argument("Origin longitude must be set");
    }
    if (!hypocenter.haveLatitude())
    {
        throw std::invalid_argument("Origin latitude must be set");
    }
    if (!hypocenter.haveDepth())
    {
        throw std::invalid_argument("Origin depth must be set");
    }
    pImpl->mHypocenter = hypocenter;
    pImpl->mHaveHypocenter = true;
}

/// Gets the origin hypocenter 
Temblor::Library::Utilities::Geodetic::GlobalPosition Origin::getHypocenter() const
{
    if (!pImpl->mHaveHypocenter)
    {
        throw std::runtime_error("Hypocenter not set");
    }
    return pImpl->mHypocenter;
}
