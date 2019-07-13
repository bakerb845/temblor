#include <cstdio>
#include <cstdlib>
#include <string>
#include <algorithm>
#include "temblor/userInterface/models/plotTransformations.hpp"

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

double convertFromGlobalPositionToUnitBall(
    const double position,
    const std::pair<double,double> &limits)
{
    if (limits.first == limits.second)
    {
        throw std::invalid_argument("limits.first == limits.second");
    }
    if (position < limits.first || position > limits.second) 
    {
        throw std::invalid_argument("position = " + std::to_string(position)
                                  + " must be in range ["
                                  + std::to_string(limits.first)
                                  + ","
                                  + std::to_string(limits.second) + "]\n");
    }
    // TJR Hughes pg 38 Eqn 1.12.2.
    // Map from [x_A, x_{A+1}] to [-1,1]
    double dh = limits.second - limits.first;
    double unitPosition = (2.0*position - limits.first - limits.second)/dh;
    return unitPosition;
}

double convertFromUnitBallToGlobalPosition(
    const double xi,
    const std::pair<double, double> &limits)
{
    if (xi < -1 || xi > +1)
    {
        throw std::invalid_argument("xi = " + std::to_string(xi)
                                  + " must be in range [-1,1]\n");
    }
    double dh = limits.second - limits.first;
    double globalPosition = 0.5*(dh*xi + limits.first + limits.second);
    return globalPosition;
}
/*
int main()
{
    std::pair<double, double> limits(5, 15);
    printf("%lf\n", convertFromGlobalPositionToUnitBall(  5.0, limits));
    printf("%lf\n", convertFromGlobalPositionToUnitBall(  7.5, limits));
    printf("%lf\n", convertFromGlobalPositionToUnitBall( 10.0, limits));
    printf("%lf\n", convertFromGlobalPositionToUnitBall( 12.5, limits));
    printf("%lf\n", convertFromGlobalPositionToUnitBall( 15.0, limits));

    printf("%lf\n", convertFromUnitBallToGlobalPosition(-1.0, limits));
    printf("%lf\n", convertFromUnitBallToGlobalPosition(-0.5, limits));
    printf("%lf\n", convertFromUnitBallToGlobalPosition( 0.0, limits));
    printf("%lf\n", convertFromUnitBallToGlobalPosition( 0.5, limits));
    printf("%lf\n", convertFromUnitBallToGlobalPosition( 1.0, limits));
    return 0;
}
*/
