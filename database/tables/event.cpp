#include <cstdio>
#include <cstdlib>
#include <string>
#include <sqlite3.h>
#include "temblor/database/tables/event.hpp"

using namespace Temblor::Database::Tables;

class Event::EventImpl
{
public:
    sqlite3 *mDatabase = nullptr;
};

Event::Event() :
    pImpl(std::make_unique<EventImpl> ())
{
}

Event::~Event() = default;

void Event::open(const std::string &dataBaseName)
{
    close(); 
    auto rc = sqlite3_open(dataBaseName.c_str(), &pImpl->mDatabase);
    if (rc != SQLITE_OK)
    {
        std::string error = "Cannot open database: "
                          + std::string(sqlite3_errmsg(pImpl->mDatabase))
                          + "\n";
        close();
        throw std::runtime_error(error);
    }
    std::string sql = "CREATE TABLE EVENT(" \
                      "EVID INT8" \
                      ");";
}

void Event::close()
{
    if (pImpl->mDatabase)
    {
         sqlite3_close(pImpl->mDatabase);
    }
    pImpl->mDatabase = nullptr;
}

