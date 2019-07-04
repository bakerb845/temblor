#include <cstdio>
#include <cstdlib>
#include "temblor/userInterface/models/rgba.hpp"
#include <gtest/gtest.h>

namespace {

using namespace Temblor::UserInterface::Models;

TEST(uiModels, RGBA)
{
    RGBA rgba;
    EXPECT_NO_THROW(rgba.setRed(15));
    EXPECT_EQ(rgba.getRed(), 15);
    EXPECT_NEAR(rgba.getFractionalRed(), 15/255.0, 1.e-14);

    EXPECT_NO_THROW(rgba.setGreen(151));
    EXPECT_EQ(rgba.getGreen(), 151);
    EXPECT_NEAR(rgba.getFractionalGreen(), 151/255.0, 1.e-14);

    EXPECT_NO_THROW(rgba.setBlue(37));
    EXPECT_EQ(rgba.getBlue(), 37);
    EXPECT_NEAR(rgba.getFractionalBlue(), 37/255.0, 1.e-14);

    EXPECT_NO_THROW(rgba.setAlpha(0.9));
    EXPECT_EQ(rgba.getAlpha(), 0.9); 

    // Test copy
    RGBA rgbaCopy(rgba);
    EXPECT_EQ(rgbaCopy.getRed(), 15);
    EXPECT_NEAR(rgbaCopy.getFractionalRed(), 15/255.0, 1.e-14);
    EXPECT_EQ(rgbaCopy.getGreen(), 151);
    EXPECT_NEAR(rgbaCopy.getFractionalGreen(), 151/255.0, 1.e-14);
    EXPECT_EQ(rgbaCopy.getBlue(), 37);
    EXPECT_NEAR(rgbaCopy.getFractionalBlue(), 37/255.0, 1.e-14);
    EXPECT_NO_THROW(rgbaCopy.setAlpha(0.9));
    EXPECT_EQ(rgbaCopy.getAlpha(), 0.9);

    // Test the fractional conversions
    EXPECT_NO_THROW(rgba.setFractionalRed(0.0));
    EXPECT_EQ(rgba.getRed(), 0);
    EXPECT_NO_THROW(rgba.setFractionalRed(0.1));
    auto ired = static_cast<int> (0.1*255 + 0.5);
    EXPECT_EQ(rgba.getRed(), ired);
    EXPECT_NO_THROW(rgba.setFractionalGreen(0.5));
    auto igreen = static_cast<int> (0.5*255 + 0.5);
    EXPECT_EQ(rgba.getGreen(), igreen);
    EXPECT_NO_THROW(rgba.setFractionalBlue(1.0));
    EXPECT_EQ(rgba.getBlue(), 255);
}

}
