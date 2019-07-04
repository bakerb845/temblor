#ifndef TEMBLOR_USERINTERFACE_MODELS_RGBA_HPP
#define TEMBLOR_USERINTERFACE_MODELS_RGBA_HPP 1
#include <memory>

namespace Temblor::UserInterface::Models
{
/*!
 * @class RGBA "rgba.hpp"
 * @brief Defines a (Red, Blue, Green, Alpha) tuple.
 * @author Ben Baker, University of Utah
 */
class RGBA
{
public:
    /*! @name Constructors
     * @{
     */
    /*!
     * @brief Default constructor intiializes a black point.
     */
    RGBA();
    /*!
     * @brief Copy constructor.
     */
    RGBA(const RGBA &rgba);
    /*!
     * @brief Move constructor.
     */
    RGBA(RGBA &&rgba) noexcept;
    /*! @} */

    /*! @name Operators
     * @{
     */
    /*!
     * @brief Copy assignment operator.
     * @param[in] rgba   RGBA class to copy.
     * @result A deep copy of the RGBA class.
     */
    RGBA& operator=(const RGBA &rgba);
    /*!
     * @brief Move assignment operator.
     * @param[in,out] rgba   The RGBA class to move to this.  On exit RGBA's
     *                       behavior is undefined.
     * @result The moved contents of RGBA.
     */
    RGBA& operator=(RGBA &&rgba) noexcept;
    /*! @} */

    /*! @name Destructors
     * @{
     */
    /*!
     * @brief Destructor
     */
    ~RGBA();
    /*!
     * @brief Clears the class and resets to the default.
     */
    void clear() noexcept;
    /*! @} */

    /*! @name Red
     * @{
     */
    /*!
     * @brief Sets the red component of the RGBA.
     * @param[in] red   The red RGBA value.  This must be in the range [0,255].
     * @throws std::invalid_argument if the input value is out of range.
     */
    void setRed(const int red); 
    /*!
     * @brief Sets the fractional red component of the RGBA.
     * @param[in] red   The fractional red value.  This must be in the
     *                  range [0,1] where 0 correponds to an integral
     *                  RGBA value of 0 while 1 corresponds to an integral
     *                  RGBA value of 255.
     * @throws std::invalid_argument if the value is out of range.
     */
    void setFractionalRed(const double red);
    /*!
     * @brief Gets the red component of the RGBA.
     * @result The red component.  This is an integer in the range [0,255].
     */
    int getRed() const noexcept;
    /*!
     * @brief Gets the fractional red component of the RGBA. 
     * @result The fractional red component.  This is in the range [0,1] where
     *         0 corresponds to an integral RGBA value of 0 and 1 corresponds to
     *         an integral RGBA value of 255.
     */
    double getFractionalRed() const noexcept;
    /*! @} */

    /*! @name Green 
     * @{
     */
    /*!
     * @brief Sets the green component of the RGBA.
     * @param[in] green   The green RGBA value.  This must be in the
     *                     range [0,255].
     * @throws std::invalid_argument if the input value is out of range.
     */
    void setGreen(const int green);
    /*!
     * @brief Sets the fractional green component of the RGBA.
     * @param[in] green   The fractional red value.  This must be in the
     *                    range [0,1] where 0 correponds to an integral
     *                    RGBA value of 0 while 1 corresponds to an integral
     *                    RGBA value of 255.
     * @throws std::invalid_argument if the value is out of range.
     */
    void setFractionalGreen(const double green);
    /*!
     * @brief Gets the green component of the RGBA.
     * @result The green component.  This is an integer in the range [0,255].
     */
    int getGreen() const noexcept;
    /*!
     * @brief Gets the fractional green component of the RGBA.
     * @result The fractional green component.  This is in the range [0,1] where
     *         0 corresponds to an integral RGBA value of 0 and 1 corresponds to
     *         an integral RGBA value of 255.
     */
    double getFractionalGreen() const noexcept;
    /*! @} */

    /*! @name Blue
     * @{
     */
    /*!
     * @brief Sets the blue component of the RGBA.
     * @param[in] blue   The green RGBA value.  This must be in the
     *                   range [0,255].
     * @throws std::invalid_argument if the input value is out of range.
     */
    void setBlue(const int blue);
    /*!
     * @brief Sets the fractional blue component of the RGBA.
     * @param[in] blue   The fractional red value.  This must be in the
     *                   range [0,1] where 0 correponds to an integral
     *                   RGBA value of 0 while 1 corresponds to an integral
     *                   RGBA value of 255.
     * @throws std::invalid_argument if the value is out of range.
     */
    void setFractionalBlue(const double blue);
    /*!
     * @brief Gets the blue component of the RGBA.
     * @result The blue component.  This is an integer in the range [0,255].
     */
    int getBlue() const noexcept;
    /*!
     * @brief Gets the fractional blue component of the RGBA.
     * @result The fractional blue component.  This is in the range [0,1] where
     *         0 corresponds to an integral RGBA value of 0 and 1 corresponds to
     *         an integral RGBA value of 255.
     */
    double getFractionalBlue() const noexcept;
    /*! @} */

    /*! @name Alpha
     * @{
     */
    /*!
     * @brief Sets the opacity, alpha.
     * @param[in] alpha  The alpha component.  This is in the range [0,1]
     *                   where 0 is transparent and 1 is opaque.
     * @throws std::invalid_argument if alpha is out of bounds.
     */ 
    void setAlpha(const double alpha);
    /*!
     * @brief Gets the opacity, alpha.
     * @result The opacity, alpha.  This is in the range [0,1] where 0 is
     *         transparent and 1 is opaque.
     */
    double getAlpha() const noexcept;
    /*! @} */
private:
    class RGBAImpl;
    std::unique_ptr<RGBAImpl> pImpl;
};
}

#endif
