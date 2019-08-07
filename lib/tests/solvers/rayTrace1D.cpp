#include <cstdio>
#include <cstdlib>
#include <cmath>
#include <vector>
#include "temblor/solvers/rayTrace1D/isotropicLayer.hpp"
#include "temblor/solvers/rayTrace1D/isotropicLayerCakeModel.hpp"
#include "temblor/solvers/rayTrace1D/raySegment.hpp"
#include <gtest/gtest.h>

namespace
{

using namespace Temblor::Solvers::RayTrace1D;

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

TEST(LibrarySolversRayTrace1D, isotropicLayerCakeModel)
{
    // Create a few layers in the Wasatch Front
    const double datum = 2;
    std::vector<double> hBWF({0.0, 1.54 + datum, 17.1 + datum,
                              28.0 + datum, 42.0 + datum});
    std::vector<double> vpBWF({3.4, 5.9, 6.4, 7.5, 7.9});
    std::vector<double> rhBWF({2.7, 2.8, 2.9, 3.1, 3.2}); // I just made this up
    const double vpvs = 1.74;
    std::vector<double> vsBWF({3.4/vpvs, 5.9/vpvs, 6.4/vpvs,
                               7.5/vpvs, 7.9/vpvs});
    int nLayers = vsBWF.size() - 1; 
    // Create the layer cake model
    IsotropicLayerCakeModel model;
    EXPECT_FALSE(model.isValid());
    for (auto ilayer=0; ilayer<nLayers; ilayer++)
    {
        IsotropicLayer layer;
        layer.setCompressionalVelocity(vpBWF[ilayer]*1.e3);
        layer.setShearVelocity(vpBWF[ilayer]*1.e3/vpvs);
        layer.setDensity(rhBWF[ilayer]*1.e3);
        layer.setThickness( (hBWF[ilayer+1] - hBWF[ilayer])*1.e3 );
        //printf("setting thickness: %lf\n", layer.getThickness());
        bool lastLayer = false;
        if (ilayer == nLayers - 1){lastLayer = true;}
        if (ilayer == 0)
        {
            model.setTopLayer(layer, lastLayer);
        }
        else
        {
            model.appendLayer(layer, lastLayer);
        }
        EXPECT_EQ(ilayer + 1, model.getNumberOfLayers());
        EXPECT_EQ(model.canAppendLayer(), !lastLayer);
        if (ilayer == 0)
        {
            EXPECT_EQ(model.getLayerIndexFromDepth(0), 0);
            EXPECT_EQ(model.getLayerIndexFromDepth(1), 0);
            EXPECT_EQ(model.getLayerIndexFromDepth(3539), 0);
        }
        else if (ilayer == 1)
        {
            EXPECT_EQ(model.getLayerIndexFromDepth(3540), 1);
            EXPECT_EQ(model.getLayerIndexFromDepth(3541), 1);
            EXPECT_EQ(model.getLayerIndexFromDepth(19099), 1);
        }
    }
    EXPECT_TRUE(model.isValid());
    // Test map from depth to layer index
    EXPECT_EQ(model.getLayerIndexFromDepth(0), 0);
    EXPECT_EQ(model.getLayerIndexFromDepth(1), 0);
    EXPECT_EQ(model.getLayerIndexFromDepth(3539), 0);

    EXPECT_EQ(model.getLayerIndexFromDepth(3540), 1);
    EXPECT_EQ(model.getLayerIndexFromDepth(3541), 1);
    EXPECT_EQ(model.getLayerIndexFromDepth(19099), 1); 

    EXPECT_EQ(model.getLayerIndexFromDepth(19100), 2);
    EXPECT_EQ(model.getLayerIndexFromDepth(19101), 2);
    EXPECT_EQ(model.getLayerIndexFromDepth(29999), 2);

    EXPECT_EQ(model.getLayerIndexFromDepth(30000), 3);
    EXPECT_EQ(model.getLayerIndexFromDepth(30001), 3);
    EXPECT_EQ(model.getLayerIndexFromDepth(43999), 3);

    EXPECT_EQ(model.getLayerIndexFromDepth(44000), 4);
    EXPECT_EQ(model.getLayerIndexFromDepth(44001), 4);
    EXPECT_EQ(model.getLayerIndexFromDepth(6400.e3), 4);
    // Test the copy constructor
    IsotropicLayerCakeModel modelCopy(model);
    EXPECT_EQ(nLayers, modelCopy.getNumberOfLayers());
    // Verify the model 
    for (auto ilayer=0; ilayer< static_cast<int> (nLayers); ilayer++)
    {
        auto layer = modelCopy.getLayer(ilayer);
        IsotropicLayer layerRef;
        double vp = vpBWF[ilayer]*1.e3;
        double vs = vp/vpvs;
        double rho = rhBWF[ilayer]*1.e3;
        double thickness = (hBWF[ilayer+1] - hBWF[ilayer])*1.e3;
        EXPECT_NEAR(layer.getCompressionalVelocity(), vp, 1.e-10);
        EXPECT_NEAR(layer.getShearVelocity(), vs, 1.e-10);
        EXPECT_NEAR(layer.getDensity(), rho, 1.e-10);
        EXPECT_NEAR(layer.getThickness(), thickness, 1.e-10);
        EXPECT_TRUE(layer.isValid());
    }
}

TEST(LibrarySolversRayTrace1D, raySegment)
{
    /// Just knock it out - constructor calls methods 
    std::pair<double, double> startPoint(1000, 2000);
    std::pair<double, double> endPoint(4000, 6000);
    const double velocity = 5000;
    RaySegment segment(startPoint, endPoint, velocity);
    EXPECT_NEAR(segment.getDistance(), 5000, 1.e-10);
    EXPECT_NEAR(segment.getVelocity(), 5000, 1.e-10);
    EXPECT_NEAR(segment.getTravelTime(), 1, 1.e-10);
    // Test copy
    RaySegment segmentCopy(segment);
    EXPECT_NEAR(segmentCopy.getDistance(), 5000, 1.e-10);
    EXPECT_NEAR(segmentCopy.getVelocity(), 5000, 1.e-10);
    EXPECT_NEAR(segmentCopy.getTravelTime(), 1, 1.e-10);
}

TEST(LibrarySolversRayTrace1D, twoPointRayTrace)
{

}

}
