#include <cstdio>
#include <cstdlib>
#include <string>
#include "temblor/database/tables/eventTypeTuple.hpp"

using namespace Temblor::Database::Tables;

class EventTypeTuple::EventTypeTupleImpl
{
public:
    Category mCategory = Category::UNKNOWN;
    std::string mName = "unknown";
    std::string mDescription = "unknown type";
};

EventTypeTuple::EventTypeTuple() :
    pImpl(std::make_unique<EventTypeTupleImpl> ())
{
}

EventTypeTuple::EventTypeTuple(const EventTypeTuple &eventType)
{
    *this = eventType;
}

EventTypeTuple& EventTypeTuple::operator=(const EventTypeTuple &eventType)
{
    if (&eventType == this){return *this;}
    pImpl = std::make_unique<EventTypeTupleImpl> (*eventType.pImpl);
    return *this;
}

EventTypeTuple::~EventTypeTuple() = default;

void EventTypeTuple::reset() noexcept
{
    setEventType(Category::UNKNOWN);
}

void EventTypeTuple::setEventType(const Category category) noexcept
{
    if (category == Category::LOCAL)
    {
        pImpl->mCategory = category;
        pImpl->mName = "local";
        pImpl->mDescription = "local earthquake";
    }
    else if (category == Category::REGIONAL)
    {
        pImpl->mCategory = category;
        pImpl->mName = "regional";
        pImpl->mDescription = "regional earthquake";
    }
    else if (category == Category::TELESEISM)
    {
        pImpl->mCategory = category;
        pImpl->mName = "teleseism";
        pImpl->mDescription = "teleseismic earthquake";
    }
    else if (category == Category::QUARRY_BLAST)
    {
        pImpl->mCategory = category;
        pImpl->mName = "quarry";
        pImpl->mDescription = "quarry blast";
    }
    else if (category == Category::UNKNOWN)
    {
        pImpl->mCategory = category;
        pImpl->mName = "unknown";
        pImpl->mDescription = "unknown type";
    }
    else if (category == Category::OTHER)
    {
        pImpl->mCategory = category;
        pImpl->mName = "other";
        pImpl->mDescription = "other miscellaneous";
    }
    else
    {
        fprintf(stderr, "Unhandled event type\n");
    }
}

EventTypeTuple::Category EventTypeTuple::getEventType() const noexcept
{
    return pImpl->mCategory;
}

std::string EventTypeTuple::getName() const noexcept
{
    return pImpl->mName;
}

std::string EventTypeTuple::getDescription() const noexcept
{
    return pImpl->mDescription;
}

bool EventTypeTuple::isValid() const noexcept
{
    if (pImpl->mName.empty()){return false;}
    return true;
}
