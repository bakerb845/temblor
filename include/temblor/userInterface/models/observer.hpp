#ifndef TEMBLOR_USERINTERFACE_MODELS_OBSERVER_HPP
#define TEMBLOR_USERINTERFACE_MODELS_OBSERVER_HPP 1
namespace Temblor::UserInterface::Models
{
/*!
 * @brief Abstract vase class describing the observer in the observer pattern.
 * @copyright Ben Baker distributed under the MIT license.
 */
class IObserver
{
public:
    /*!
     * @brief Default destructor.
     */
    virtual ~IObserver() = default;
    /*!
     * @brief Pure virtual function to allow the observer to receive
     *        notifiactions that the subject was updated.
     */
    virtual void update(const int message) = 0;
};
}
#endif
