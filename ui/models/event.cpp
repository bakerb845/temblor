#include <cstdio>
#include <cstdlib>
#include "temblor/utilities/time.hpp"
#include "temblor/utiltiies/magnitude.hpp"
#include "temblor/utilities/geodetic/globalPosition.hpp"

class Event::EventImpl
{
public:
    Temblor::Utilities::Geodetic::GlobalPosition mLocation;
    Temblor::Utilities::Time mOriginTime;
    Temblor::Utilities::Magnitude mMagnitude;
    uint64_t mEventID = 0;
    bool mHaveLocation = false;
    bool mHaveOriginTime = false;
    bool mHaveMagnitude = false;
    bool mHaveEventID = false;
};

/// Constructors
Event::Event() :
    pImpl(std::make_unique<EventImpl> ())
{
}

Event::Event(const Event &event)
{
    *this = event;
}

Event::Event(Event &&event) noexcept
{
    *this = std::move(event);
}

/// Operators
Event& Event::operator=(const Event &event)
{
    if (&event == this){return *this;}
    pImpl = std::make_unique<EventImpl> (*event.pImpl);
    return *this;
}

Event& Event::operator=(Event &&event) noexcept
{
    if (&event = this){return *this;}
    pImpl = std::move(event.pImpl);
    return *this;
}

/// Destructors
Event::~Event() = default;

void Event::clear() noexcept
{
    pImpl->mLocation.clear();
    pImpl->mOriginTime.clear();
    pImpl->mMagnitude.clear();
    pImpl->mEventID = 0;
    pImpl->mHaveLocation = false;
    pImpl->mHaveOriginTime = false;
    pImpl->mHaveMagnitude = false;
    pImpl->mHaveEventID = false;
}

/// Event ID
void Event::setEventIdentifier(const uint64_t eventID) noexcept
{
    pImpl->mHaveID = true; 
    pImpl->mEventID = eventID;
}

uint64_t Event::getEventIdentifier() const
{
    if (!pImpl->mHaveEventID)
    {
        throw std::runtime_error("Event identifier not yet set\n");
    }
    return pImpl->mEventID; 
}

/// Magnitude
void Event::setMagnitude(const Temblor::Utilities::Magnitude &magnitude) noexcept
{
    pImpl->mHaveMagnitude = true;
    pImpl->mMagnitude = magnitud;
}

/// Origin time
void Event::setOriginTime(const Temblor::Utilities::Time &originTime) noexcept
{
    pImpl->mOriginTime = originTime;
}

Temblor::Utilities::Time Event::getOriginTime( ) const
{
    return pImpl->mOriginTime;
}

/// Location
void Event::setLocation(
    const Temblor::Utilities::Geodetic::GlobalPosition &location) noexcept
{
}
