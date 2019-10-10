#ifndef TEMBLOR_USERINTERFACE_MODELS_SUBJECTS_HPP
#define TEMBLOR_USERINTERFACE_MODELS_SUBJECTS_HPP
#include <memory>

namespace Temblor::UserInterface::Models
{

/*!
 * @brief Defines the subjects that observers in the library can bind to and
 *        manipulate..
 */
class Subjects
{
    /*!
     * @brief Gets the IIR filter parameters subject.
     */
    IIRFilterParametersSubject getIIRFilterParametersSubject();
private:
    class SubjectsImpl;
    std::unique_ptr<SubjectsImpl> pImpl;
};

}
#endif
