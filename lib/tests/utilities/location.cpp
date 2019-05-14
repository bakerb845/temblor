#include <cstdio>
#include <cstdlib>
#include "temblor/library/utilities/location/globalPosition.hpp"
#include "temblor/library/utilities/location/globalPositionPair.hpp"
#include <gtest/gtest.h>

namespace
{

using namespace Temblor::Library::Utilities::Location;
TEST(LibraryUtilitiesPosition, GlobalPosition)
{
    GlobalPosition position; 
    EXPECT_FALSE(position.haveLatitude());
    EXPECT_FALSE(position.haveLongitude());
    EXPECT_FALSE(position.haveDepth());

    position.setLatitude(30.);
    position.setLongitude(120.5);
    position.setDepth(500.0);
    EXPECT_TRUE(position.haveLatitude());
    EXPECT_TRUE(position.haveLongitude());
    EXPECT_TRUE(position.haveDepth());
    EXPECT_NEAR(position.getLatitude(),      30, 1.e-13);
    EXPECT_NEAR(position.getLongitude(),  120.5, 1.e-13);
    EXPECT_NEAR(position.getDepth(),      500.0, 1.e-13);
    EXPECT_NEAR(position.getEasting(),   258842.24462241694, 1.e-3); // Close to mm
    EXPECT_NEAR(position.getNorthing(), 3321417.0304058315,  1.e-3); // Close to mm
    EXPECT_EQ(position.getUTMZone(),    51);

    // Test copy constructor
    GlobalPosition position2(position);
    EXPECT_TRUE(position2.haveLatitude());
    EXPECT_TRUE(position2.haveLongitude());
    EXPECT_TRUE(position2.haveDepth());
    EXPECT_NEAR(position2.getLatitude(),      30, 1.e-13);
    EXPECT_NEAR(position2.getLongitude(),  120.5, 1.e-13);
    EXPECT_NEAR(position2.getDepth(),      500.0, 1.e-13);

    EXPECT_NEAR(position2.getEasting(),   258842.24462241694, 1.e-3); // Close to mm
    EXPECT_NEAR(position2.getNorthing(), 3321417.0304058315,  1.e-3); // Close to mm
    EXPECT_EQ(position2.getUTMZone(),    51);

}

}

