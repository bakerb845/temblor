#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <string>
#include <cmath>
#include <vector>
#include <limits>
#include <boost/align/aligned_allocator.hpp>
//#include "temblor/solvers/eikonal/fastSweeping2D.hpp"

//using namespace Temblor::Solvers::Eikonal;

namespace
{
void compuateUpdateNodes(const int nx, const int nz, const int sweepNumber,
                         std::vector<bool> &isUpdateNode)
{
    isUpdateNode.resize(nx*nz);
    std::fill(isUpdateNode.begin(), isUpdateNode.end(), false);
    if (sweepNumber == 1)
    {
         
    }
    else if (sweepNumber == 2)
    {
    }
    else if (sweepNumber == 3)
    {
    }
    else if (sweepNumber == 4)
    {
    }
 
}

/// Performs the local update after the solver is inititalized
template<class T>
void localSolverInitialized(const int nUpdateNodes, // Number of nodes to update
                            const T dx, // Grid spacing in x
                            const T dz, // Grid spacing in z
                            const T dx2i, // 1/(dx^2)
                            const T dz2i, // 1/(dz^2)
                            const T dz2i_dx2i, // 1/(dx^2*dz^2)
                            const T dz2i_p_dx2i, // 1/dx^2 + 1/dz^2
                            const T dz2i_p_dx2i_inv, // (dz^2 + dx^2)/(dx^2*dz^2)
                            const T fteikHuge,   // Large value  
                            const T *__restrict__ localSlowness, // Slowness
                            const T *__restrict__ localTravelTimes, // Travel times
                            T *__restrict__ tupd) // Nodes to update
{
    for (auto i=0; i<nUpdateNodes; ++i)
    {
        auto s1 = localSlowness[4*i];
        auto s2 = localSlowness[4*i+1];
        auto s3 = localSlowness[4*i+2];
        auto tv  = localTravelTimes[4*i];
        auto te  = localTravelTimes[4*i+1];
        auto tev = localTravelTimes[4*i+2];
        auto tt  = localTravelTimes[4*i+3];
        auto temtv = te - tv;
        // 1D operators (refracted times)
        auto t12min = std::min(tv + dz*s1, te + dx*s2);
        // 2D operator
        auto t1_2d = fteikHuge;
        if (temtv < dz*s3 && -temtv < dx*s3)
        {
            auto sref2 = s3*s3;
            auto ta = tev + temtv;
            auto tb = tev - temtv;
            auto tab = ta - tb;
            auto tab2 = tab*tab;
            auto four_sref2 = 4*sref2;
            auto t1_2d = ( (tb*dz2i + ta*dx2i)
                         + std::sqrt(four_sref2*dz2i_p_dx2i - dz2i_dx2i*tab2) )
                        *dz2i_p_dx2i_inv;
        }
        tupd[i] = std::min(t12min, t1_2d);
    }
}

}
/*
template<class T>
class FastSweeping2D<T>::FastSweeping2DImpl
{
public:
    void localSolverInitial(const int nUpdateNodes,
                            const T *__restrict__ localSlowness,
                            const T *__restrict__ localTravelTimes )
    {
        for (int i=0; i<nUpdateNodes; ++i)
        {
        }
    }
    /// The travel time vector
    std::vector<T> mTravelTimes;
    /// Determines if the i'th node is an update node
    std::vector<bool> mUpdate;

    const T mHuge = std::numeric_limits<<T>::max(); 
};
*/
