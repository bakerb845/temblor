#include <cstdio>
#include <cstdlib>
#include <cmath>
#include "temblor/library/solvers/rayTrace1D/isotropicLayer.hpp"
#include "temblor/library/solvers/rayTrace1D/layerCakeModel.hpp"
#include <gtest/gtest.h>

namespace
{

using namespace Temblor::Library::Solvers::RayTrace1D;

TEST(LibrarySolversRayTrace1D, isotropicLayer)
{
    IsotropicLayer layer;
    const double vp = 5000.0;
    const double vs = vp/std::sqrt(3.0);
    const double rho = 2700.0;
    const double thickness = 1.e3;
    EXPECT_FALSE(layer.isValid());
    layer.setCompressionalVelocity(vp);
    layer.setShearVelocity(vs);
    layer.setDensity(rho);
    layer.setThickness(thickness);
    EXPECT_NEAR(layer.getCompressionalVelocity(), vp, 1.e-10);
    EXPECT_NEAR(layer.getShearVelocity(), vs, 1.e-10);
    EXPECT_NEAR(layer.getDensity(), rho, 1.e-10);
    EXPECT_NEAR(layer.getThickness(), thickness, 1.e-10);
    EXPECT_TRUE(layer.isValid());
    // Make sure vs > vp fails validity
    layer.setShearVelocity(vp + DBL_EPSILON);
    EXPECT_FALSE(layer.isValid());
    layer.setShearVelocity(vs); 
    // Test the copy constructor
    IsotropicLayer layerCopy(layer); 
    EXPECT_NEAR(layerCopy.getCompressionalVelocity(), vp, 1.e-10);
    EXPECT_NEAR(layerCopy.getShearVelocity(), vs, 1.e-10);
    EXPECT_NEAR(layerCopy.getDensity(), rho, 1.e-10);
    EXPECT_NEAR(layerCopy.getThickness(), thickness, 1.e-10);
    EXPECT_TRUE(layerCopy.isValid()); 
    // Test the move operator 
    IsotropicLayer layerMove = std::move(layer);
    EXPECT_NEAR(layerMove.getCompressionalVelocity(), vp, 1.e-10);
    EXPECT_NEAR(layerMove.getShearVelocity(), vs, 1.e-10);
    EXPECT_NEAR(layerMove.getDensity(), rho, 1.e-10);
    EXPECT_NEAR(layerMove.getThickness(), thickness, 1.e-10);
    EXPECT_TRUE(layerMove.isValid());
}

}
