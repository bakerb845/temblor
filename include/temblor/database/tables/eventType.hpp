#ifndef TEMBLOR_DATABASE_TABLES_EVENTTYPE_HPP
#define TEMBLOR_DATABASE_TABLES_EVENTTYPE_HPP 1
#include <memory>

namespace Temblor::Database::Tables
{
class EventType
{
public:
    /*! @name Constructors
     * @{
     */
    /*!
     * @brief Constructor
     */
    EventType();
    /*!@ } */

    /*! @name Destructors
     * @{
     */
    /*!
     * @brief Default destructor.
     */
    ~EventType();
    /*!
     * @brief Closes the connection to the database.
     */
    void close() noexcept;
    /*! @} */

    /*!
     * @brief Creates the table eventtype.db.
     */
    void create();
    /*!
     * @brief Creates the eventtype table.
     * @param[in] tableName  The name of the eventtype table.
     */
    void create(const std::string &tableName);
private:
    class EventTypeImpl;
    std::unique_ptr<EventTypeImpl> pImpl;
};
}
#endif
