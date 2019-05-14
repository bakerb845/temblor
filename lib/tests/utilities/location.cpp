#include <cstdio>
#include <cstdlib>
#include "temblor/library/utilities/geodetic/globalPosition.hpp"
#include "temblor/library/utilities/geodetic/globalPositionPair.hpp"
#include <gtest/gtest.h>

namespace
{

using namespace Temblor::Library::Utilities::Geodetic;

TEST(LibraryUtilitiesGeodetic, GlobalPosition)
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

    // Test whacky longitudes
    position.setLongitude(-111.89);
    EXPECT_NEAR(position.getLongitude(), -111.89+360.0, 1.e-13); 
    position.setLongitude(5.0-360.0);
    EXPECT_NEAR(position.getLongitude(),   5.0, 1.e-13);
    position.setLongitude(0.0-540.0);
    EXPECT_NEAR(position.getLongitude(), 180.0, 1.e-13);
    position.setLongitude(360.0 + 5.0);
    EXPECT_NEAR(position.getLongitude(),   5.0, 1.e-13);
    position.setLongitude(360.0 + 540.0);
    EXPECT_NEAR(position.getLongitude(), 180.0, 1.e-13);
}

}

