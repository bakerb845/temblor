#include <cstdio>
#include <cstdlib>
#include <string>
#include <sqlite3.h>
#include <sqlite_orm.h>
#include "temblor/database/tables/eventType.hpp"
#include "temblor/database/tables/eventTypeTuple.hpp"

using namespace Temblor::Database::Tables;
namespace SQL3 = sqlite_orm;

namespace
{
int callback(void *unUsed, int argc, char **argv, char **columnName)
{
    for (int i=0; i<argc; ++i)
    {
        if (strcmp(columnName[i], "ETYPE") == 0)
        {
        }
        else if (strcmp(columnName[i], "NAME") == 0)
        {
        }
        else if (strcmp(columnName[i], "DESCRIPTION") == 0)
        {
        }
        else
        {
            fprintf(stderr, "Unhandled: %s\n", columnName[i]);
        }
    }
    return 0;
}

struct EventTypeStruct
{
    std::string eType;
    std::string name;
    std::unique_ptr<std::string> description; // Optional
};

inline auto makeStorage()
{
    return SQL3::make_storage("./eventType.sqlite",
                              SQL3::make_table("EVENTTYPE",
                              SQL3::make_column("ETYPE", &EventTypeStruct::eType), //, SQL3::primary_key()),
                              SQL3::make_column("NAME",  &EventTypeStruct::name),
                              SQL3::make_column("DESCRIPTION", &EventTypeStruct::description)));
}
 
using Storage = decltype(makeStorage());

}

class EventType::EventTypeImpl
{
public:
    Storage mStorage{makeStorage()}; //Table mTable{makeTable()};
};

EventType::EventType() :
    pImpl(std::make_unique<EventTypeImpl> ())
{
    //Storage mStorage = makeStorage();
    //pImpl->mStorage = std::move(mStorage);
}

void EventType::create()
{
    create("EVENTTYPE");
}

void EventType::create(const std::string &tableNameIn)
{
    EventTypeStruct le{"le", "local", std::make_unique<std::string>("local earthquake")};
    EventTypeStruct re{"re", "regional", std::make_unique<std::string>("regional earthquake")};
    EventTypeStruct qb{"qb", "quarry", std::make_unique<std::string>("quarry blast")};
    EventTypeStruct te{"te", "teleseism", std::make_unique<std::string>("teleseismic earthquake")};
    EventTypeStruct uk{"uk", "unknown", std::make_unique<std::string>("unknown type")};
    EventTypeStruct ot{"ot", "other", std::make_unique<std::string>("other miscellaneous")};

    try
    {
        pImpl->mStorage.sync_schema();
        pImpl->mStorage.remove_all<EventTypeStruct> ();
        pImpl->mStorage.insert(le);
        pImpl->mStorage.insert(re);
        pImpl->mStorage.insert(qb);
        pImpl->mStorage.insert(te);
        pImpl->mStorage.insert(uk);
        pImpl->mStorage.insert(ot);
    }
    catch (const std::exception &e)
    {
        fprintf(stderr, "Failed to create eventType %s\n", e.what());
    }
//    events.push_back(re);
//    events.push_back(qb);
//    events.push_back(te);
//    events.push_back(uk);
//    events.push_back(ot);

/*
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
*/
}

EventType::~EventType() = default;

void EventType::close() noexcept
{
/*
    if (pImpl->mDatabase){sqlite3_free(pImpl->mDatabase);}
    pImpl->mDatabase = nullptr;
*/
}

