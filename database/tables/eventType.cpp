#include <cstdio>
#include <cstdlib>
#include <string>
#include <sqlite3.h>
#include "temblor/database/tables/eventType.hpp"
#include "temblor/database/tables/eventTypeTuple.hpp"

using namespace Temblor::Database::Tables;

class EventType::EventTypeImpl
{
public:
    sqlite3 *mDatabase = nullptr;
};

EventType::EventType() :
    pImpl(std::make_unique<EventTypeImpl> ())
{
}

void EventType::create()
{
    create("EVENTTYPE");
}

void EventType::create(const std::string &tableNameIn)
{
    close();
    if (tableNameIn.empty())
    {
        throw std::invalid_argument("tableName is empty\n");
    }
    std::string tableName = tableNameIn;;
    std::for_each(tableName.begin(), tableName.end(), [](char & c){ c = ::toupper(c); });
    // Open the database 
    auto rc = sqlite3_open(tableName.c_str(), &pImpl->mDatabase);
    if (rc != SQLITE_OK)
    {
        std::string error = "Failed to open table: "
                          + std::string(sqlite3_errmsg(pImpl->mDatabase))
                          + "\n";
        close();
        throw std::runtime_error(error);
    }
    // Create the table
    std::string sql = "DROP TABLE IF EXISTS " + tableName + ";\n"
                    + "CREATE TABLE " + tableName + "("
                    + "ETYPE VARCHAR(2) NOT NULL, "
                    + "NAME VARCHAR(16) NOT NULL, "
                    + "DESCRIPTION VARCHAR(80), "
                    + "PRIMARY KEY(ETYPE));"
                    + "INSERT INTO " + tableName + " VALUES('le', 'local', 'local earthquake');\n"
                    + "INSERT INTO " + tableName + " VALUES('re', 'regional', 'regional earthquake');\n"
                    + "INSERT INTO " + tableName + " VALUES('qb', 'quarry', 'quarry blast');\n"
                    + "INSERT INTO " + tableName + " VALUES('te', 'teleseism', 'teleseismic earthquake');\n"
                    + "INSERT INTO " + tableName + " VALUES('uk', 'unknown', 'unknown type');\n";
                    + "INSERT INTO " + tableName + " VALUES('ot', 'other', 'other miscellaneosu')\n";
    char *errmsg;
    rc = sqlite3_exec(pImpl->mDatabase, sql.c_str(), 0, 0, &errmsg);
    if (rc != SQLITE_OK)
    {
        std::string error = "Failed to build table: " + std::string(errmsg);
        sqlite3_free(errmsg);
        close();
        throw std::runtime_error(error);
    }
}

EventType::~EventType()
{
    close();
}

void EventType::close() noexcept
{
    if (pImpl->mDatabase){sqlite3_free(pImpl->mDatabase);}
    pImpl->mDatabase = nullptr;
}

