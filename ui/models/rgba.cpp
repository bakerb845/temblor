
#include "temblor/userInterface/models/rgba.hpp"

namespace Temblor::UserInterface::Models
{
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
private:
    class RGBAImpl;
    std::unique_ptr<RGBAImpl> pImpl;
};
}

using namespace Temblor::UserInterface::Models;

class RGBA::RGBAImpl
{
public:
    double mAlpha = 1;
    int mRed = 0;
    int mGreen = 0;
    int mBlue = 0;
};

/// Constructors
RGBA::RGBA::() : 
    pImpl(std::make_unique<RGBAImpl> ())
{
}

/// Destructors
RGBA::~RGBA() = default;

/// Operators 
RGBA& RGBA::operator=(const RGBA &rgba)
{
    if (&rgba == this){return *this;}
    pImpl = std::make_unique<RGBAImpl> (*rgba.pImpl);
    return *this;
}

/// Red 
void RGBA::setRed(const int value)
{
    if (value < 0 || value > 255)
    {
        throw std::invalid_argument("red = " + std::to_string(value)
                                  + " must be in range [0,255]");
    }
    pImpl->mRed = value;
}

void RGBA::setFractionalRed(const double value)
{
    if (value < 0 || value > 1)
    {
        throw std::invalid_argument("red = " + std::to_string(value)
                                  + " must be in range [0,1]");
    }
    auto intColor = static_cast<int> (value*255.0);
    pImpl->mRed = std::min(255, std::max(0, intColor));
}

int RGBA::getRed() const noexcept
{
    return pImpl->mRed;
}

double RGBA::getFractionalRed() const noexcept
{
    return static_cast<double> (pImpl->mRed)/255.0;
}

