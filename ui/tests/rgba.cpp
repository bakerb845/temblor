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

}

}
