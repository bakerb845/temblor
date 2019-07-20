#ifndef TEMBLOR_DATABASE_TABLES_EVENTTYPETUPLE_HPP
#define TEMBLOR_DATABASE_TABLES_EVENTTYPETUPLE_HPP 1
#include <memory>
namespace Temblor::Database::Tables
{
/*!
 * @brief Defines an event type.  This is required to have a category and
 *        name.
 */
class EventTypeTuple
{
public:
    /*!
     * @brief Defines the event type's category.
     */
    enum class Category
    {
        LOCAL,        /*!< Local earthquake. */
        REGIONAL,     /*!< Regional earthquake. */
        TELESEISM,    /*!< Teleseismic event. */
        QUARRY_BLAST, /*!< Quarry blast. */
        UNKNOWN,      /*!< Unknown. */
        OTHER         /*!< Other - miscellaneous. */
    };

    /*!
     * @brief Constructor.
     */
    EventTypeTuple();
    /*!
     * @brief Copy constructor.
     */
    EventTypeTuple(const EventTypeTuple &eventType);
    /*!
     * @brief Copy assignment operator.
     */
    EventTypeTuple& operator=(const EventTypeTuple &eventType);
    /*!
     * @brief Destructor.
     */
    ~EventTypeTuple();
    /*!
     * @brief Resets the event type to the unknown state.
     */
    void reset() noexcept;
    /*!
     * @brief Sets the event type.
     * @param[in] category   The event category.
     */
    void setEventType(const Category category) noexcept;
    /*!
     * @brief Gets the event type.
     */
    Category getEventType() const noexcept;
    /*!
     * @brief Gets the expanded name of the event.
     */
    std::string getName() const noexcept;
    /*!
     * @brief Gets the event description.
     */
    std::string getDescription() const noexcept;
    /*!
     * @brief Determines if the class represents a valid tuple.
     * @retval True indicates that the class is valid.
     */
    bool isValid() const noexcept;
private:
    class EventTypeTupleImpl;
    std::unique_ptr<EventTypeTupleImpl> pImpl; 
};
}
#endif
