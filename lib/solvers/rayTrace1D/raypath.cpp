#include <cstdio>
#include <cstdlib>
#include <cmath>
#include <exception>
#include <vector>
#include <string>
#include <algorithm>
#include <numeric>
#include <memory>

//namespace Temblor::Library::Solvers::RayTrace1D;

namespace
{

/*!
 * @brief Defines a point in 3D.
 */
class Point 
{
public:
    Point() = default;
    Point(const double x0, const double y0, const double z0) :
       x(x0), y(y0), z(z0)
    {
    }
    /// The x position of the point.
    double x = 0;
    /// The y position of the point.
    double y = 0;
    /// The z position of the point.
    double z = 0;
};

/*!
 * @brief Computes the sum of points, p1 + p2.
 */
Point operator +(const Point &p1, const Point &p2)
{
    Point res;
    res.x = p1.x + p2.x;
    res.y = p1.y + p2.y;
    res.z = p1.z + p2.z;
    return res;
}

/*!
 * @brief Computes the difference of points, p1 - p2.
 */
Point operator -(const Point &p1, const Point &p2)
{
    Point res;
    res.x = p1.x - p2.x;
    res.y = p1.y - p2.y;
    res.z = p1.z - p2.z;
    return res;
}

/*!
 * @brief Defines a segment which connects a start and end point.
 */ 
class Segment
{
public:
    /*!
     * @brief Clears the ray segment.
     */
    void clear() noexcept
    {
        Point p(0, 0, 0);
        mStartPoint = p;
        mEndPoint = p;
        mLength =-1;
    } 
    /*!
     * @brief Sets the segment start point.
     * @param[in] startPoint  Position of start point.
     */
    void setStartPoint(const Point &startPoint) noexcept
    {
        mLength =-1;
        mStartPoint = startPoint;
    }
    /*!
     * @brief Gets the start point of the segment.
     * @result The starting position of the segment.
     */
    Point getStartPoint() const noexcept
    {
        return mStartPoint;
    }
    /*!
     * @brief Sets the segment end point.
     * @param[in] endPoint  Position of end point.
     */
    void setEndPoint(const Point &endPoint)  noexcept
    {
        mLength =-1;
        mEndPoint = endPoint;
    }
    /*!
     * @brief Gets the segment end point.
     * @result The position of the terminal point of the segment.
     */
    Point getEndPoint() const noexcept
    {
        return mEndPoint;
    }
    /*!
     * @brief Computes the segment length.
     * @result The length of the segment.
     */ 
    double getLength() const noexcept
    {
        if (mLength < 0)
        {
            Point diff = mEndPoint - mStartPoint;
            mLength = std::sqrt(diff.x*diff.x + diff.y*diff.y + diff.z*diff.z);
        }
        return mLength;
    }
//private:
    Point mStartPoint;
    Point mEndPoint;
    mutable double mLength =-1;
};

}

/*!
 * @brief Defines an isotropic layer in the layer-cake velocity model.
 */
class IsotropicLayer
{
public:
    /*!
     * @brief Constructor.
     */
    IsotropicLayer();
    /*!
     * @brief Copy constructor.
     * @param[in] layer  The isotropic layer to initialize from.
     */
    IsotropicLayer(const IsotropicLayer &layer);
    /*!
     * @brief Move constructor.
     * @param[in,out] layer  The isotropic layer to initialize from.
     *                       On exit layer's behavior is undefined.
     */
    IsotropicLayer(IsotropicLayer &&layer) noexcept;

    /*!
     * @brief Copy operator.
     * @param[in] layer  The isotropic layer to copy.
     * @result A deep copy of the input layer.
     */
    IsotropicLayer& operator=(const IsotropicLayer &layer);
    /*!
     * @brief Move operator.
     * @param[in,out] layer  The isotropic layer to move to this.
     *                       On exit layer's behavior is undefined.
     * @result The memory of layer moved to this.
     */
    IsotropicLayer& operator=(IsotropicLayer &&layer) noexcept;

    /*!
     * @brief Destructor.
     */
    ~IsotropicLayer();
    /*!
     * @brief Clears the model.
     */
    void clear() noexcept;

    /*!
     * @brief Sets the compressional velocity in the layer.
     * @param[in] vp   The compressional velocity in meters/second.
     * @throws std::invalid_argument if vp is not positive.
     */
    void setCompressionalVelocity(const double vp);
    /*!
     * @brief Gets the compressional velocity in the layer.
     * @result The compressional velocity in meters/second.
     * @throws std::runtime_error if the compressional velocity was not set.
     */
    double getCompressionalVelocity() const;
    /*!
     * @brief Sets the shear velocity in the layer.
     * @param[in] vs   The shear velocity in meters/second.
     * @throws std::invalid_argument if vs is not positive.
     */
    void setShearVelocity(const double vs);
    /*!
     * @brief Gets the shear velocity in the layer.
     * @result The shear velocity in meters/second.
     * @throws std::invalid_argument if the shear velocity was not set.
     */
    double getShearVelocity() const;
    /*!
     * @brief Sets the density in the layer.
     * @param[in] rho  The density in kilograms/meter**3.
     * @throws std;:invalid_argument if rho is not positive.
     */
    void setDensity(const double rho);
    /*!
     * @brief Gets the density in the layer.
     * @result The density in kilograms/meter**3.
     * @throws std::runtime_error if the density was not set.
     */
    double getDensity() const;
    /*!
     * @brief Sets the layer thickness.
     * @param[in] h   The layer thickness in meters.
     * @throws std::invalid_argument if the thickness is not positive.
     */
    void setThickness(const double h);
    /*!
     * @brief Gets the layer thickness.
     * @result The layer thickness in meters.
     * @throws std::runtime_error if the thickness was not set.
     */
    double getThickness() const;
    /*!
     * @brief Checks if this is a valid layer.
     * @param[in] True indicates that this is a physically sensible layer.
     */
    bool isValid() const noexcept;

    /*!
     * @brief Prints the layer properites.
     * @param[in] handle  The file handle to print to.  If this is NULL then 
     *                    this will print to stdout.
     */
    void print(FILE *handle = nullptr) noexcept;
private:
    class IsotropicLayerImpl;
    std::unique_ptr<IsotropicLayerImpl> pImpl;
};

class IsotropicLayer::IsotropicLayerImpl
{
public:
    double mVp =-1;
    double mVs =-1;
    double mRho =-1;
    double mThickness =-1;
};

IsotropicLayer::IsotropicLayer() :
    pImpl(std::make_unique<IsotropicLayerImpl> ())
{
}

IsotropicLayer::IsotropicLayer(const IsotropicLayer &layer)
{
    *this = layer;
}

IsotropicLayer::IsotropicLayer(IsotropicLayer &&layer) noexcept
{
    *this = std::move(layer);
}

IsotropicLayer& IsotropicLayer::operator=(const IsotropicLayer &layer)
{
    if (&layer == this){return *this;}
    pImpl = std::make_unique<IsotropicLayerImpl> (*layer.pImpl);
    return *this;
}

IsotropicLayer& IsotropicLayer::operator=(IsotropicLayer &&layer) noexcept
{
    if (&layer == this){return *this;}
    pImpl = std::move(layer.pImpl);
    return *this;
}

IsotropicLayer::~IsotropicLayer() = default;

void IsotropicLayer::clear() noexcept
{
    pImpl->mVp =-1;
    pImpl->mVs =-1;
    pImpl->mRho =-1;
    pImpl->mThickness =-1;
}

void IsotropicLayer::setCompressionalVelocity(const double vp)
{
    pImpl->mVp =-1;
    if (vp <= 0)
    {
        throw std::invalid_argument("Vp = " + std::to_string(vp)
                                  + "must be positive");
    }
    pImpl->mVp = vp;
}

double IsotropicLayer::getCompressionalVelocity() const
{
    if (pImpl->mVp < 0)
    {
        throw std::runtime_error("Vp was not set");
    }
    return pImpl->mVp;
}

void IsotropicLayer::setShearVelocity(const double vs)
{
    pImpl->mVs =-1;
    if (vs <= 0)
    {
        throw std::invalid_argument("Vs = " + std::to_string(vs)
                                  + " must be positive");
    }
    pImpl->mVs = vs;
}

double IsotropicLayer::getShearVelocity() const
{
    if (pImpl->mVs < 0)
    {
        throw std::runtime_error("Vs was not set");
    }
    return pImpl->mVs;
}

void IsotropicLayer::setDensity(const double rho)
{
    pImpl->mRho =-1;
    if (rho <= 0)
    {
        throw std::invalid_argument("Density = " + std::to_string(rho)
                                  + " must be positive");
    }
    pImpl->mRho = rho;
}

double IsotropicLayer::getDensity() const
{
   if (pImpl->mRho < 0)
   {
       throw std::runtime_error("Density was not set");
   }
   return pImpl->mRho;
}

void IsotropicLayer::setThickness(const double h)
{
   pImpl->mThickness =-1;
   if (h <= 0)
   {
       throw std::invalid_argument("Thickness = " + std::to_string(h)
                                 + " must be postiive");
   }
   pImpl->mThickness = h;
}

double IsotropicLayer::getThickness() const
{
   if (pImpl->mThickness < 0)
   {
       throw std::invalid_argument("Thickness not set");
   }
   return pImpl->mThickness;
}

bool IsotropicLayer::isValid() const noexcept
{
    if (pImpl->mVp <= 0){return false;}
    if (pImpl->mVp <= pImpl->mVs){return false;}
    if (pImpl->mRho <= 0){return false;}
    if (pImpl->mThickness <= 0){return false;}
    return false;
}

void IsotropicLayer::print(FILE *handle) noexcept
{
    if (!isValid()){return;}
    FILE *ftemp = stdout;
    if (handle){ftemp = handle;}
    fprintf(ftemp, "Vp: %lf (m/s)", pImpl->mVp);
    fprintf(ftemp, "Vs: %lf (m/s)", pImpl->mVs);
    fprintf(ftemp, "Density: %lf (kg/m**3)", pImpl->mRho);
    fprintf(ftemp, "Thickness: %lf (m)", pImpl->mThickness);
}

/*!
 * @brief Defines a 1D layer cake model.
 */
class LayerCakeModel
{
public:
    /*!
     * @brief Constructor.
     */
    LayerCakeModel();

    /*!
     * @brief Destructor.
     */
    ~LayerCakeModel();

    /*!
     * @brief Clears the model.
     */
    void clear() noexcept;

    /*!
     * @brief This begins the model building process by setting the shallowmost
     *        layer in the earth model.  It will clear any previous model.
     */ 
    void setTopLayer(const IsotropicLayer &layer);

    /*!
     * @brief Appends a layer to the model.  This can be called as many times
     *        as needed after calling \c setTopLayer().
     * @param[in] layer  The layer to append.
     * @throws std::runtime_error if \c setHalfspace() was called or if
     *         \c setTopLayer()
     * @throws std::invalid_argument if layer is not valid.
     */
    void appendLayer(const IsotropicLayer &layer);

    /*!
     * @brief Ends the model building process by setting the underling
     *        halfspace.
     */ 
    void setHalfspace(const IsotropicLayer &layer);

    /*!
     * @brief Gets the number of layers in the model.
     * @result The number of layers in the model.
     */
    int getNumberOfLayers() const noexcept;
private:
    class LayerCakeModelImpl;
    std::unique_ptr<LayerCakeModelImpl> pImpl; 
};

class LayerCakeModel::LayerCakeModelImpl
{
public:
    std::vector<class IsotropicLayer> mModel;
};

LayerCakeModel::LayerCakeModel() :
    pImpl(std::make_unique<LayerCakeModelImpl> ())
{
}

LayerCakeModel::~LayerCakeModel() = default;

int LayerCakeModel::getNumberOfLayers() const noexcept
{
    return static_cast<int> (pImpl->mModel.size());
}

void LayerCakeModel::clear() noexcept
{
    pImpl->mModel.clear();
}

void LayerCakeModel::appendLayer(const IsotropicLayer &layer)
{
    if (!layer.isValid())
    {
        throw std::invalid_argument("Layer is invalid");
    }
    pImpl->mModel.push_back(layer);
}

/*!
 * @brief Defines a raypath as a set of segments.  Each segment has a travel 
 *        time and constant velocity.
 */
class Raypath
{
public:
    /*!
     * @brief Constructor.
     */
    Raypath();
    /*!
     * @brief Destructor.
     */
    ~Raypath();
    /*!
     * @brief Clears the raypath.
     */
    void clear() noexcept;
    /*!
     * @brief Computes the travel time along a raypath.
     * @result The traveltime along the raypath in seconds.
     */
    double computeTravelTime() const;
    /*!
     * @brief Computes the length of the ray pathpath.
     */
    double computeLength() const;

    void appendSegment(const Segment &segment, const double travelTime);
private:
    class RaypathImpl;
    std::unique_ptr<RaypathImpl> pImpl;
};

class Raypath::RaypathImpl
{
public:
    std::vector<Segment> mRayPath;
    std::vector<double> mSegmentTimes;
    bool mTerminatedRaypath = false;
};

Raypath::Raypath() :
    pImpl(std::make_unique<RaypathImpl> ())
{
}

Raypath::~Raypath() = default;


void Raypath::clear() noexcept
{
    pImpl->mRayPath.clear();
    pImpl->mSegmentTimes.clear();
    pImpl->mTerminatedRaypath = false;
}
/*

void Raypath::begin(const Segment &segment, const double travelTime)
{
    clear();
    mRayPath.appendSegment(segment, travelTime); 
}
*/

void Raypath::appendSegment(const Segment &segment, const double travelTime)
{
    if (pImpl->mTerminatedRaypath)
    {
        throw std::runtime_error("Cannot append to terminated raypath");
    }
    pImpl->mRayPath.push_back(segment);
    pImpl->mSegmentTimes.push_back(travelTime);
}
/*

void Raypath::terminate(const Segment &segment, const double travelTime)
{
    appendSegment(segment, travelTime);
    pImpl->mTerminatedRaypath = true;
}
*/

double Raypath::computeTravelTime() const
{
    auto ttime = std::accumulate(pImpl->mSegmentTimes.begin(),
                                 pImpl->mSegmentTimes.end(),
                                 0);
    return ttime;
}

double Raypath::computeLength() const
{
    double length = 0;
    for (auto i=0; i<pImpl->mRayPath.size(); ++i)
    {
        length = length + pImpl->mRayPath[i].getLength();
    }
    return length;
}

/*
double Raypath::getTakeoffAngle( ) const
{

}

double Raypath::getTakeoffAzimuth( ) const
{

}

double Raypath::getIncidenceAngle( ) const
{

}

*/

int main()
{
return 0;
}
