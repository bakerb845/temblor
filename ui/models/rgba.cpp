#include <cstdio>
#include <cstdlib>
#include <string>
#include <cmath>
#include "temblor/userInterface/models/rgba.hpp"

using namespace Temblor::UserInterface::Models;

namespace
{
double integerToFraction(const int value)
{
    return static_cast<double> (value)/255.0;
}
int fractionToInteger(const double value)
{
    return std::min(static_cast<int> (value*255.0 + 0.5), 255);
}         
}

class RGBA::RGBAImpl
{
public:
    double mAlpha = 1;
    int mRed = 0;
    int mGreen = 0;
    int mBlue = 0;
};

/// Constructors
RGBA::RGBA() : 
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
    pImpl->mRed = fractionToInteger(value);
}

int RGBA::getRed() const noexcept
{
    return pImpl->mRed;
}

double RGBA::getFractionalRed() const noexcept
{
    return integerToFraction(pImpl->mRed);
}

// Green
void RGBA::setGreen(const int value)
{
    if (value < 0 || value > 255)
    {
        throw std::invalid_argument("green = " + std::to_string(value)
                                  + " must be in range [0,255]");
    }
    pImpl->mGreen = value;
}

void RGBA::setFractionalGreen(const double value)
{
    if (value < 0 || value > 1)
    {
        throw std::invalid_argument("green = " + std::to_string(value)
                                  + " must be in range [0,1]");
    }
    pImpl->mGreen = fractionToInteger(value);
}

int RGBA::getGreen() const noexcept
{
    return pImpl->mGreen;
}

double RGBA::getFractionalGreen() const noexcept
{
    return integerToFraction(pImpl->mGreen);
}

// Blue
void RGBA::setBlue(const int value)
{
    if (value < 0 || value > 255)
    {
        throw std::invalid_argument("blue = " + std::to_string(value)
                                  + " must be in range [0,255]");
    }
    pImpl->mBlue = value;
}

void RGBA::setFractionalBlue(const double value)
{
    if (value < 0 || value > 1)
    {
        throw std::invalid_argument("blue = " + std::to_string(value)
                                  + " must be in range [0,1]");
    }
    pImpl->mBlue = fractionToInteger(value);
}

int RGBA::getBlue() const noexcept
{
    return pImpl->mBlue;
}

double RGBA::getFractionalBlue() const noexcept
{
    return integerToFraction(pImpl->mBlue);
}

// Alpha
void RGBA::setAlpha(const double alpha)
{
    if (alpha < 0 || alpha > 1)
    {
       throw std::invalid_argument("alpha = " + std::to_string(alpha)
                                 + " must be in range [0,1]");
    }
    pImpl->mAlpha = alpha;
}

double RGBA::getAlpha() const noexcept
{
    return pImpl->mAlpha;
}
