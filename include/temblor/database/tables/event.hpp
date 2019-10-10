#ifndef TEMBLOR_DATABASE_TABLES_EVENT_HPP
#define TEMBLOR_DATABASE_TABLES_EVENT_HPP 1
#include <memory>

namespace Temblor::Database::Tables
{
/*!
 * @brief Schema defining the event table.
 */
class Event
{
public:
    Event(); 
    ~Event();
    void open(const std::string &dataBaseName);
    void close();
private:
    class EventImpl;
    std::unique_ptr<EventImpl> pImpl;
};
}
#endif
