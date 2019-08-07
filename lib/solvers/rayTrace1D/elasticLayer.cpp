#include <cstdio>
#include <cstdlib>
#include <array>
#include <cmath>
#include <mkl_lapacke.h>
#include "temblor/solvers/rayTrace1D/enums.hpp"
#include "temblor/solvers/rayTrace1D/elasticLayer.hpp"
#include "temblor/solvers/rayTrace1D/isotropicLayer.hpp"

using namespace Temblor::Solvers::RayTrace1D;

namespace
{

/// Uses Voigt notation to flatten a 2d tensor to 1d
int ij2index(const int i, const int j)
{
    int index[3][3] = { {0, 5, 4},
                        {5, 1, 3},
                        {4, 3, 2} };
    return index[i][j];
}

/// Uses Voigt notation to flatten a 4d tesnor to 2d
int ijkl2index(const int i, const int j, const int k, const int l)
{
    int index =  6*ij2index(i, j) + ij2index(k, l);
    return index;
}

/// @brief Computes the direction cosines of a ray in geographical coordinates.
///        Udias - Source Mechanics pg 78 or Grechka - Microseismic Monitoring
///        pg 60.
/// @param[in] takeOffAngle  The take-off angle (radians).  This is measured
///                          from the downward positive vertical.
/// @param[in] azimuth       The source-receiver azimuth (radians).  This is
///                          measured from north.
/// @param[out] n            The unit normal in (north, east, nadir).
void computeDirectionCosines(const double takeOffAngle,
                             const double azimuth,
                             double n[3])
{
    double sinToA = sin(takeOffAngle);
    n[0] = sinToA*cos(azimuth);
    n[1] = sinToA*sin(azimuth);
    n[2] = cos(takeOffAngle);
}

/// Computes the Christoffel matrix in row major format
void computeChristoffelMatrix(const double C[36], double rho, const double n[3], double gammaMat[9])
{
    std::memset(gammaMat, 0, 9*sizeof(double));
    // Compute the geometric products and normalize by density
    double njlrhoi[9];
    double rhoi = 1.0/rho;
    for (auto j=0; j<3; ++j)
    {
        njlrhoi[3*j+0] = (n[j]*n[0])*rhoi;
        njlrhoi[3*j+1] = (n[j]*n[1])*rhoi;
        njlrhoi[3*j+2] = (n[j]*n[2])*rhoi;
    }
    // Fill the christoffel matrix - loop on rows
    for (auto i=0; i<3; ++i)
    {
        // Loop on columns
        for (auto k=0; k<3; ++k) 
        {
            double gammaik = 0;
            #pragma omp simd collapse(2), reduction(+:gammaik)
            for (auto j=0; j<3; ++j)
            {
                auto cijk1 = C[ijkl2index(i, j, k, 0)];
                auto cijk2 = C[ijkl2index(i, j, k, 1)];
                auto cijk3 = C[ijkl2index(i, j, k, 2)];
                gammaik = gammaik
                        + cijk1*njlrhoi[3*j+0]
                        + cijk2*njlrhoi[3*j+1]
                        + cijk3*njlrhoi[3*j+2];
            }
            gammaMat[3*i+k] = gammaik;
        }
    }
}

void fillIsotropicC(const double c11, 
                    const double c13,
                    const double c44,
                    std::array<double, 36> &cij)
{
    std::memset(cij.data(), 0, 36*sizeof(double));
    // Upper left - first column 
    cij[0] = c11;
    cij[1] = c13;
    cij[2] = c13;
    // Second column
    cij[6] = c13;
    cij[7] = c11;
    cij[8] = c13;
    // Third column
    cij[12] = c13;
    cij[13] = c13;
    cij[14] = c11;
    // Lower right of isotropic stiffness matrix
    cij[21] = c44;
    cij[28] = c44;
    cij[35] = c44;
}

void computePhase(const double C[36], const double rho, const double takeOffAngle, const double azimuth)
{
    double n[3], gammaMat[9];
    computeDirectionCosines(takeOffAngle, azimuth, n);
    computeChristoffelMatrix(C, rho, n, gammaMat);
    // Balance, reduce to upper Hessenberg, compute Q,
    double scale, tau[2], wr[3], wi[3], z[9], vl[9], vr[9];
    int ilo, ihi, select[3], m;
    LAPACKE_dgebal(LAPACK_COL_MAJOR, 'B', 3, gammaMat, 3, &ilo, &ihi, &scale);
    LAPACKE_dgehrd(LAPACK_COL_MAJOR, 3, ilo, ihi, gammaMat, 3, tau);
    LAPACKE_dorghr(LAPACK_COL_MAJOR, 3, ilo, ihi, gammaMat, 3, tau);
    LAPACKE_dhseqr(LAPACK_COL_MAJOR, 'S', 'V', 3, ilo, ihi, gammaMat, 3, wr, wi, z, 3);
    LAPACKE_dtrevc(LAPACK_COL_MAJOR, 'R', 'A', select, 3, gammaMat, 3, vl, 3, vr, 3, 3, &m);
 
}

}
 
class ElasticLayer::ElasticLayerImpl
{
public:
    class IsotropicLayer mIsotropicLayer;
    std::array<double, 36> mCij{0, 0, 0, 0, 0, 0, 
                                0, 0, 0, 0, 0, 0,
                                0, 0, 0, 0, 0, 0,
                                0, 0, 0, 0, 0, 0,
                                0, 0, 0, 0, 0, 0,
                                0, 0, 0, 0, 0, 0};
    double mRho = 0;
    double mThickness = 0;
    ElasticLayerSymmetry mSymmetry = ElasticLayerSymmetry::UNKNOWN;
};

/// Constructors
ElasticLayer::ElasticLayer() :
    pImpl(std::make_unique<ElasticLayerImpl> ())
{
}

ElasticLayer::ElasticLayer(const ElasticLayer &layer)
{
    *this = layer;
}

ElasticLayer::ElasticLayer(ElasticLayer &&layer) noexcept
{
    *this = std::move(layer);
}

/// Operators
ElasticLayer& ElasticLayer::operator=(const ElasticLayer &layer)
{
    if (&layer == this){return *this;}
    pImpl = std::make_unique<ElasticLayerImpl> (*layer.pImpl); 
    return *this;
}

ElasticLayer& ElasticLayer::operator=(ElasticLayer &&layer) noexcept
{
    if (&layer == this){return *this;}
    pImpl = std::move(layer.pImpl);
    return *this;
}

/// Destructors
ElasticLayer::~ElasticLayer() = default;

void ElasticLayer::clear() noexcept
{
    pImpl->mIsotropicLayer.clear();
    std::memset(pImpl->mCij.data(), 0, 36*sizeof(double));
    pImpl->mRho = 0;
    pImpl->mThickness = 0;
    pImpl->mSymmetry = ElasticLayerSymmetry::UNKNOWN;
}

/// Set an isotropic layer
void ElasticLayer::setLayer(const IsotropicLayer &layer)
{
    if (!layer.isValid())
    {
        throw std::invalid_argument("Layer is invalid\n");
    }
    // Compute elastic moduli
    double vp  = layer.getCompressionalVelocity();
    double vs  = layer.getShearVelocity();
    double rho = layer.getDensity();
    double mu = rho*(vs*vs);
    double lam = rho*(vp*vp) - 2.0*mu;
    double c11 = lam + 2*mu;
    double c13 = lam;
    double c44 = mu;
    pImpl->mIsotropicLayer = layer;
    fillIsotropicC(c11, c13, c44, pImpl->mCij);
    pImpl->mRho = rho;
    pImpl->mThickness = layer.getThickness();
    pImpl->mSymmetry = ElasticLayerSymmetry::ISOTROPIC;
}

/// Gets the qP velocity at the given propagation angle
double ElasticLayer::getQpGroupVelocity(const double theta,
                                        const double azimuth) const
{
    if (pImpl->mSymmetry == ElasticLayerSymmetry::ISOTROPIC)
    {
        return pImpl->mIsotropicLayer.getCompressionalVelocity();
    }
    fprintf(stderr, "%s: other symmetrices not yet programmed\n", __func__);
    return 0;
}

double ElasticLayer::getDensity() const
{
    return pImpl->mRho;
}

/// Gets the layer symmetry
ElasticLayerSymmetry ElasticLayer::getSymmetry() const noexcept
{
    return pImpl->mSymmetry;
}

/// Gets an isotropic layer
IsotropicLayer ElasticLayer::getLayer() const
{
    if (pImpl->mSymmetry != ElasticLayerSymmetry::ISOTROPIC)
    {
        throw std::runtime_error("Layer is not isotropic\n");
    }
    // Create an isotropic layer
    return pImpl->mIsotropicLayer;
}
