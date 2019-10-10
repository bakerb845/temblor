#ifndef TEMBLOR_USERINTERFACE_MODELS_SUBJECT_HPP
#define TEMBLOR_USERINTERFACE_MODELS_SUBJECT_HPP 1
namespace Temblor::UserInterface::Models
{
class IObserver;
/*!
 * @brief Abstract base class describing the subject in the observer pattern.
 * @copyright Ben Baker distributed under the MIT license.
 */
class ISubject
{
public:
    /*!
     * @brief Default destructor.
     */
    virtual ~ISubject() = default;
    /*!
     * @brief Pure virtual function to allow an observer to subscribe to this
     *        model.
     * @param[in,out] observer   The observer wishing to subscribe to the model.
     */
    virtual void subscribe(IObserver *observer) = 0;
    /*!
     * @brief Pure virtual function to allow an observer to unsubscribe from
     *        this model.
     */
    virtual void unsubscribe(IObserver *observer) = 0;
    /*!
     * @brief Pure virtual function to emit notification that parameters have changed.
     */
    virtual void notify() = 0;
};
}
#endif
