
class Location::LocationImpl
{
public:
    Location::Mode mode = Location::Mode::UNKNOWN;
    /// Location::Mode::GLOBAL ///
    /// Latitude in degrees [-90,90]
    double lat = 0;
    /// Longitude in degrees [0,360]
    double lon = 0;
    
    /// Location::Mode::UTM ///
    /// UTM position in x (this is in meters)
    double utmx = 0;
    /// UTM position in y (this is in meters)
    double utym = 0;
    /// The UTM zone
    int zone = 0;

    /// Location::Mode::LOCAL ///
    /// The station's local position in x
    double localx = 0;
    /// The station's local position in y
    double localy = 0;


};

void Location::setGlobalPosition(const double latitude, const double longitude,
                                 const double elevation)
{

}

void Location::setUTMPosition(const double utmx, const double utmy, const int zone,
                              const double elevation)
{

}

void Location::setLocalPosition(const double localx, const double localy,
                                const double elevation)
{

}

