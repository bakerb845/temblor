#ifndef TEMBLOR_UI_MODELS_PLOTTRANSFORMATIONS_HPP__ 
#define TEMBLOR_UI_MODELS_PLOTTRANSFORMATIONS_HPP__ 1

namespace Temblor::UserInterface::Models::PlotTransformations
{

/*!
 * @brief Converts from a global position to the unit ball [-1,1].
 * @param[in] position   The position in the global interval.
 * @param[in] limits     The limits of the global interval, i.e.,
 *                       position should be in the range
 *                       [limits.first, limits.second].
 * @result The corresponding position in the unit interval, [-1,1].
 * @throws std::invalid_argument if limits.first == limits.second or
 *         if the position is out outside of the limits.
 */
double convertFromGlobalPositionToUnitBall(
    const double position,
    const std::pair<double,double> &limits);
/*!
 * @brief Converts from the unit ball [-1,1] to a global position.
 * @param[in] xi         The position in the local interval, [-1,1].
 * @param[in] limits     The limits of the global interval.
 * @result The corresponding position in the global interval.
 * @throws std::invalid_argument if xi is less than -1 or greater than +1.
 */
double convertFromUnitBallToGlobalPosition(
    const double xi,
    const std::pair<double, double> &limits);

}

#endif
