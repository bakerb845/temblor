#include <cstdio>
#include <cstdlib>
#include <cassert>
#include <string>
#include <stdexcept>
#include "temblor/library/dataReaders/sac/header.hpp"

#define NULL_DOUBLE -12345
#define NULL_INT -12345
#define NULL_STRING "-12345\0\0"

using namespace Temblor::Library::DataReaders::SAC;

static std::string copyTruncatedString(const std::string &value, std::string &result);
static int unpacki4(const char c4[4], const bool lswap);
static double unpackf4(const char c4[4], const bool lswap);

static std::string copyTruncatedString(const std::string &value, std::string &result)
{
    size_t ncopy = std::min(result.length(), value.length());
    for (size_t i=0; i<ncopy; ++i)
    {
        result[i] = value[i];
    }
    for (size_t i=ncopy; i<result.length(); ++i)
    {
        result[i] = '\0';
    }
    return result;
}

static double unpackf4(const char c4[4], const bool lswap)
{
    union
    {
        char cd[4];
        float f4;
    };
    if (!lswap)
    {
        cd[0] = c4[0];
        cd[1] = c4[1];
        cd[2] = c4[2];
        cd[3] = c4[3];
    }
    else
    {
        cd[0] = c4[3];
        cd[1] = c4[2];
        cd[2] = c4[1];
        cd[3] = c4[0];
    }
    return static_cast<double> (f4);
}

static int unpacki4(const char c4[4], const bool lswap)
{
    union
    {
        char cd[4];
        int i4;
    };
    if (!lswap)
    {
        cd[0] = c4[0];
        cd[1] = c4[1];
        cd[2] = c4[2];
        cd[3] = c4[3];
    }
    else
    {
        cd[0] = c4[3];
        cd[1] = c4[2];
        cd[2] = c4[1];
        cd[3] = c4[0];
    }
    return i4;
}

class Header::HeaderImpl
{
public:
    // Doubles
    double delta     = NULL_DOUBLE;
    double depmin    = NULL_DOUBLE;
    double depmax    = NULL_DOUBLE;
    double scale     = NULL_DOUBLE;
    double odelta    = NULL_DOUBLE;
    double b         = NULL_DOUBLE;
    double e         = NULL_DOUBLE;
    double o         = NULL_DOUBLE;
    double a         = NULL_DOUBLE;
    double internal1 = NULL_DOUBLE;
    double t0        = NULL_DOUBLE;
    double t1        = NULL_DOUBLE;
    double t2        = NULL_DOUBLE;
    double t3        = NULL_DOUBLE;
    double t4        = NULL_DOUBLE;
    double t5        = NULL_DOUBLE;
    double t6        = NULL_DOUBLE;
    double t7        = NULL_DOUBLE;
    double t8        = NULL_DOUBLE;
    double t9        = NULL_DOUBLE;
    double f         = NULL_DOUBLE;
    double resp0     = NULL_DOUBLE;
    double resp1     = NULL_DOUBLE;
    double resp2     = NULL_DOUBLE;
    double resp3     = NULL_DOUBLE;
    double resp4     = NULL_DOUBLE;
    double resp5     = NULL_DOUBLE;
    double resp6     = NULL_DOUBLE;
    double resp7     = NULL_DOUBLE;
    double resp8     = NULL_DOUBLE;
    double resp9     = NULL_DOUBLE;
    double stla      = NULL_DOUBLE;
    double stlo      = NULL_DOUBLE;
    double stel      = NULL_DOUBLE;
    double stdp      = NULL_DOUBLE;
    double evla      = NULL_DOUBLE;
    double evlo      = NULL_DOUBLE;
    double evel      = NULL_DOUBLE;
    double evdp      = NULL_DOUBLE;
    double mag       = NULL_DOUBLE;
    double user0     = NULL_DOUBLE;
    double user1     = NULL_DOUBLE;
    double user2     = NULL_DOUBLE;
    double user3     = NULL_DOUBLE;
    double user4     = NULL_DOUBLE;
    double user5     = NULL_DOUBLE;
    double user6     = NULL_DOUBLE;
    double user7     = NULL_DOUBLE;
    double user8     = NULL_DOUBLE;
    double user9     = NULL_DOUBLE;
    double dist      = NULL_DOUBLE;
    double az        = NULL_DOUBLE;
    double baz       = NULL_DOUBLE;
    double gcarc     = NULL_DOUBLE;
    double internal2 = NULL_DOUBLE;
    double internal3 = NULL_DOUBLE;
    double depmen    = NULL_DOUBLE;
    double cmpaz     = NULL_DOUBLE;
    double cmpinc    = NULL_DOUBLE;
    double xminimum  = NULL_DOUBLE;
    double xmaximum  = NULL_DOUBLE;
    double yminimum  = NULL_DOUBLE;
    double ymaximum  = NULL_DOUBLE;
    double unused0   = NULL_DOUBLE;
    double unused1   = NULL_DOUBLE;
    double unused2   = NULL_DOUBLE;
    double unused3   = NULL_DOUBLE;
    double unused4   = NULL_DOUBLE;
    double unused5   = NULL_DOUBLE;
    double unused6   = NULL_DOUBLE;
    // Integers
    int nzyear     = NULL_INT;
    int nzjday     = NULL_INT;
    int nzhour     = NULL_INT;
    int nzmin      = NULL_INT;
    int nzsec      = NULL_INT;
    int nzmsec     = NULL_INT;
    int nvhdr      = NULL_INT;
    int norid      = NULL_INT;
    int nevid      = NULL_INT;
    int npts       = NULL_INT;
    int iinternal1 = NULL_INT;
    int nwfid      = NULL_INT;
    int nxsize     = NULL_INT;
    int nysize     = NULL_INT;
    int iunused0   = NULL_INT;
    int iftype     = NULL_INT;
    int idep       = NULL_INT;
    int iztype     = NULL_INT;
    int iunused1   = NULL_INT;
    int iinst      = NULL_INT;
    int istreg     = NULL_INT;
    int ievreg     = NULL_INT;
    int ievtyp     = NULL_INT;
    int iqual      = NULL_INT;
    int isynth     = NULL_INT;
    int imagtyp    = NULL_INT;
    int imagsrc    = NULL_INT;
    int iunused2   = NULL_INT;
    int iunused3   = NULL_INT;
    int iunused4   = NULL_INT;
    int iunused5   = NULL_INT;
    int iunused6   = NULL_INT;
    int iunused7   = NULL_INT;
    int iunused8   = NULL_INT;
    int iunused9   = NULL_INT;
    // Logicals
    int leven   = NULL_INT;
    int lpspol  = NULL_INT;
    int lovrok  = NULL_INT;
    int lcalda  = NULL_INT;
    int lunused = NULL_INT;
    // Character
    std::string kstnm{NULL_STRING,  8};
    std::string kevnm{NULL_STRING, 16};
    std::string khole{NULL_STRING, 8};
    std::string ko{NULL_STRING, 8};
    std::string ka{NULL_STRING, 8};
    std::string kt0{NULL_STRING, 8};
    std::string kt1{NULL_STRING, 8};
    std::string kt2{NULL_STRING, 8};
    std::string kt3{NULL_STRING, 8};
    std::string kt4{NULL_STRING, 8};
    std::string kt5{NULL_STRING, 8};
    std::string kt6{NULL_STRING, 8}; 
    std::string kt7{NULL_STRING, 8}; 
    std::string kt8{NULL_STRING, 8}; 
    std::string kt9{NULL_STRING, 8}; 
    std::string kf{NULL_STRING, 8};
    std::string kuser0{NULL_STRING, 8};
    std::string kuser1{NULL_STRING, 8};
    std::string kuser2{NULL_STRING, 8};
    std::string kcmpnm{NULL_STRING, 8};
    std::string knetwk{NULL_STRING, 8};
    std::string kdatrd{NULL_STRING, 8};
    std::string kinst{NULL_STRING, 8};
};

Header::Header() :
    pImpl(std::make_unique<HeaderImpl> ())
{
}

Header::Header(const char header[], const bool lswap)
{
    // Floats (convert to double)
    pImpl->delta      = unpackf4(  &header[0], lswap);
    pImpl->depmin     = unpackf4(  &header[4], lswap);
    pImpl->depmax     = unpackf4(  &header[8], lswap);
    pImpl->scale      = unpackf4( &header[12], lswap);
    pImpl->odelta     = unpackf4( &header[16], lswap);
    pImpl->b          = unpackf4( &header[20], lswap);
    pImpl->e          = unpackf4( &header[24], lswap);
    pImpl->o          = unpackf4( &header[28], lswap);
    pImpl->a          = unpackf4( &header[32], lswap);
    pImpl->internal1  = unpackf4( &header[36], lswap);
    pImpl->t0         = unpackf4( &header[40], lswap);
    pImpl->t1         = unpackf4( &header[44], lswap);
    pImpl->t2         = unpackf4( &header[48], lswap);
    pImpl->t3         = unpackf4( &header[52], lswap);
    pImpl->t4         = unpackf4( &header[56], lswap);
    pImpl->t5         = unpackf4( &header[60], lswap);
    pImpl->t6         = unpackf4( &header[64], lswap);
    pImpl->t7         = unpackf4( &header[68], lswap);
    pImpl->t8         = unpackf4( &header[72], lswap);
    pImpl->t9         = unpackf4( &header[76], lswap);
    pImpl->f          = unpackf4( &header[80], lswap);
    pImpl->resp0      = unpackf4( &header[84], lswap);
    pImpl->resp1      = unpackf4( &header[88], lswap);
    pImpl->resp2      = unpackf4( &header[92], lswap);
    pImpl->resp3      = unpackf4( &header[96], lswap);
    pImpl->resp4      = unpackf4(&header[100], lswap);
    pImpl->resp5      = unpackf4(&header[104], lswap);
    pImpl->resp6      = unpackf4(&header[108], lswap);
    pImpl->resp7      = unpackf4(&header[112], lswap);
    pImpl->resp8      = unpackf4(&header[116], lswap);
    pImpl->resp9      = unpackf4(&header[120], lswap);
    pImpl->stla       = unpackf4(&header[124], lswap);
    pImpl->stlo       = unpackf4(&header[128], lswap);
    pImpl->stel       = unpackf4(&header[132], lswap);
    pImpl->stdp       = unpackf4(&header[136], lswap);
    pImpl->evla       = unpackf4(&header[140], lswap);
    pImpl->evlo       = unpackf4(&header[144], lswap);
    pImpl->evel       = unpackf4(&header[148], lswap);
    pImpl->evdp       = unpackf4(&header[152], lswap);
    pImpl->mag        = unpackf4(&header[156], lswap);
    pImpl->user0      = unpackf4(&header[160], lswap);
    pImpl->user1      = unpackf4(&header[164], lswap);
    pImpl->user2      = unpackf4(&header[168], lswap);
    pImpl->user3      = unpackf4(&header[172], lswap);
    pImpl->user4      = unpackf4(&header[176], lswap);
    pImpl->user5      = unpackf4(&header[180], lswap);
    pImpl->user6      = unpackf4(&header[184], lswap);
    pImpl->user7      = unpackf4(&header[188], lswap);
    pImpl->user8      = unpackf4(&header[192], lswap);
    pImpl->user9      = unpackf4(&header[196], lswap);
    pImpl->dist       = unpackf4(&header[200], lswap);
    pImpl->az         = unpackf4(&header[204], lswap);
    pImpl->baz        = unpackf4(&header[208], lswap);
    pImpl->gcarc      = unpackf4(&header[212], lswap);
    pImpl->internal2  = unpackf4(&header[216], lswap);
    pImpl->internal3  = unpackf4(&header[220], lswap);
    pImpl->depmen     = unpackf4(&header[224], lswap);
    pImpl->cmpaz      = unpackf4(&header[228], lswap);
    pImpl->cmpinc     = unpackf4(&header[232], lswap);
    pImpl->xminimum   = unpackf4(&header[236], lswap);
    pImpl->xmaximum   = unpackf4(&header[240], lswap);
    pImpl->yminimum   = unpackf4(&header[244], lswap);
    pImpl->ymaximum   = unpackf4(&header[248], lswap);
    pImpl->unused0    = unpackf4(&header[252], lswap);
    pImpl->unused1    = unpackf4(&header[256], lswap);
    pImpl->unused2    = unpackf4(&header[260], lswap);
    pImpl->unused3    = unpackf4(&header[264], lswap);
    pImpl->unused4    = unpackf4(&header[268], lswap);
    pImpl->unused5    = unpackf4(&header[272], lswap);
    pImpl->unused6    = unpackf4(&header[276], lswap);
    // Integers
    pImpl->nzyear     = unpacki4(&header[280], lswap);
    pImpl->nzjday     = unpacki4(&header[284], lswap);
    pImpl->nzhour     = unpacki4(&header[288], lswap);
    pImpl->nzmin      = unpacki4(&header[292], lswap);
    pImpl->nzsec      = unpacki4(&header[296], lswap);
    pImpl->nzmsec     = unpacki4(&header[300], lswap);
    pImpl->nvhdr      = unpacki4(&header[304], lswap);
    pImpl->norid      = unpacki4(&header[308], lswap);
    pImpl->nevid      = unpacki4(&header[312], lswap);
    pImpl->npts       = unpacki4(&header[316], lswap);
    pImpl->iinternal1 = unpacki4(&header[320], lswap);
    pImpl->nwfid      = unpacki4(&header[324], lswap);
    pImpl->nxsize     = unpacki4(&header[328], lswap);
    pImpl->nysize     = unpacki4(&header[332], lswap);
    pImpl->iunused0   = unpacki4(&header[336], lswap);
    pImpl->iftype     = unpacki4(&header[340], lswap);
    pImpl->idep       = unpacki4(&header[344], lswap);
    pImpl->iztype     = unpacki4(&header[348], lswap);
    pImpl->iunused1   = unpacki4(&header[352], lswap);
    pImpl->iinst      = unpacki4(&header[356], lswap);
    pImpl->istreg     = unpacki4(&header[360], lswap);
    pImpl->ievreg     = unpacki4(&header[364], lswap);
    pImpl->ievtyp     = unpacki4(&header[368], lswap);
    pImpl->iqual      = unpacki4(&header[372], lswap);
    pImpl->isynth     = unpacki4(&header[376], lswap);
    pImpl->imagtyp    = unpacki4(&header[380], lswap);
    pImpl->imagsrc    = unpacki4(&header[384], lswap);
    pImpl->iunused2   = unpacki4(&header[388], lswap);
    pImpl->iunused3   = unpacki4(&header[392], lswap);
    pImpl->iunused4   = unpacki4(&header[396], lswap);
    pImpl->iunused5   = unpacki4(&header[400], lswap);
    pImpl->iunused6   = unpacki4(&header[404], lswap);
    pImpl->iunused7   = unpacki4(&header[408], lswap);
    pImpl->iunused8   = unpacki4(&header[412], lswap);
    pImpl->iunused9   = unpacki4(&header[416], lswap);
    // Logicals
    pImpl->leven   = unpacki4(&header[420], lswap);
    pImpl->lpspol  = unpacki4(&header[424], lswap);
    pImpl->lovrok  = unpacki4(&header[428], lswap);
    pImpl->lcalda  = unpacki4(&header[432], lswap);
    pImpl->lunused = unpacki4(&header[436], lswap);
    // Strings
}

Header::Header(const Header &header)
{
    *this = header;
}

Header::Header(Header &&header) noexcept
{
    *this = std::move(header);
}

Header& Header::operator=(const Header &header)
{
    if (&header == this){return *this;}
    if (pImpl){pImpl.reset();}
    pImpl = std::make_unique<HeaderImpl> (*header.pImpl);
    return *this;
}

Header& Header::operator=(Header &&header) noexcept
{
    if (&header == this){return *this;}
    if (pImpl){pImpl.reset();}
    pImpl = std::move(header.pImpl);
    return *this;
}

Header::~Header() = default;

void Header::clear() noexcept
{
    Header header; // Create a default header
    *this = std::move(header); // Move it to this header to avoid copying
}

double Header::getHeader(const Double variableName) const noexcept
{
    if (variableName == Double::DELTA)
    {
        return pImpl->delta;
    }
    else if (variableName == Double::DEPMIN)
    {
        return pImpl->depmin;
    }
    else if (variableName == Double::DEPMAX)
    {
        return pImpl->depmax;
    }
    else if (variableName == Double::SCALE)
    {
        return pImpl->scale;
    }
    else if (variableName == Double::ODELTA)
    {
        return pImpl->odelta;
    }
    else if (variableName == Double::B)
    {
        return pImpl->b;
    }
    else if (variableName == Double::E)
    {
        return pImpl->e;
    }
    else if (variableName == Double::O)
    {
        return pImpl->o;
    }
    else if (variableName == Double::A)
    {
        return pImpl->a;
    }
    else if (variableName == Double::INTERNAL1)
    {
        return pImpl->internal1;
    }
    else if (variableName == Double::T0)
    {
        return pImpl->t0;
    }
    else if (variableName == Double::T1)
    {
        return pImpl->t1;
    }
    else if (variableName == Double::T2)
    {
        return pImpl->t2;
    }
    else if (variableName == Double::T3)
    {
        return pImpl->t3;
    }
    else if (variableName == Double::T4)
    {
        return pImpl->t4;
    }
    else if (variableName == Double::T5)
    {
        return pImpl->t5;
    }
    else if (variableName == Double::T6)
    {
        return pImpl->t6;
    }
    else if (variableName == Double::T7)
    {
        return pImpl->t7;
    }
    else if (variableName == Double::T8)
    {
        return pImpl->t8;
    }
    else if (variableName == Double::T9)
    {
        return pImpl->t9;
    }
    else if (variableName == Double::F)
    {
        return pImpl->f;
    }
    else if (variableName == Double::RESP0)
    {
        return pImpl->resp0;
    }
    else if (variableName == Double::RESP1)
    {
        return pImpl->resp1;
    }
    else if (variableName == Double::RESP2)
    {
        return pImpl->resp2;
    }
    else if (variableName == Double::RESP3)
    {
        return pImpl->resp3;
    }
    else if (variableName == Double::RESP4)
    {
        return pImpl->resp4;
    }
    else if (variableName == Double::RESP5)
    {
        return pImpl->resp5;
    }
    else if (variableName == Double::RESP6)
    {
        return pImpl->resp6;
    }
    else if (variableName == Double::RESP7)
    {
        return pImpl->resp7;
    }
    else if (variableName == Double::RESP8)
    {
        return pImpl->resp8;
    }
    else if (variableName == Double::RESP9)
    {
        return pImpl->resp9;
    }
    else if (variableName == Double::STLA)
    {
        return pImpl->stla;
    }
    else if (variableName == Double::STLO)
    {
        return pImpl->stlo;
    }
    else if (variableName == Double::STEL)
    {
        return pImpl->stel;
    }
    else if (variableName == Double::STDP)
    {
        return pImpl->stdp;
    }
    else if (variableName == Double::EVLA)
    {
        return pImpl->evla;
    }
    else if (variableName == Double::EVLO)
    {
        return pImpl->evlo;
    }
    else if (variableName == Double::EVEL)
    {
        return pImpl->evel;
    }
    else if (variableName == Double::EVDP)
    {
        return pImpl->evdp;
    }
    else if (variableName == Double::MAG)
    {
        return pImpl->mag;
    }
    else if (variableName == Double::USER0)
    {
        return pImpl->user0;
    }
    else if (variableName == Double::USER1)
    {
        return pImpl->user1;
    }
    else if (variableName == Double::USER2)
    {
        return pImpl->user2;
    }
    else if (variableName == Double::USER3)
    {
        return pImpl->user3;
    }
    else if (variableName == Double::USER4)
    {
        return pImpl->user4;
    }
    else if (variableName == Double::USER5)
    {
        return pImpl->user5;
    }
    else if (variableName == Double::USER6)
    {
        return pImpl->user6;
    }
    else if (variableName == Double::USER7)
    {
        return pImpl->user7;
    }
    else if (variableName == Double::USER8)
    {
        return pImpl->user8;
    }
    else if (variableName == Double::USER9)
    {
        return pImpl->user9;
    }
    else if (variableName == Double::DIST)
    {
        return pImpl->dist;
    }
    else if (variableName == Double::AZ)
    {
        return pImpl->az;
    }
    else if (variableName == Double::BAZ)
    {
        return pImpl->baz;
    }
    else if (variableName == Double::GCARC)
    {
        return pImpl->gcarc;
    }
    else if (variableName == Double::INTERNAL2)
    {
        return pImpl->internal2;
    }
    else if (variableName == Double::INTERNAL3)
    {
        return pImpl->internal3;
    }
    else if (variableName == Double::DEPMEN)
    {
        return pImpl->depmen;
    }
    else if (variableName == Double::CMPAZ)
    {
        return pImpl->cmpaz;
    }
    else if (variableName == Double::CMPINC)
    {
        return pImpl->cmpinc;
    }
    else if (variableName == Double::XMINIMUM)
    {
        return pImpl->xminimum;
    }
    else if (variableName == Double::XMAXIMUM)
    {
        return pImpl->xmaximum;
    }
    else if (variableName == Double::YMINIMUM)
    {
        return pImpl->yminimum;
    }
    else if (variableName == Double::YMAXIMUM)
    {
        return pImpl->ymaximum;
    }
    else if (variableName == Double::UNUSED0)
    {
        return pImpl->unused0;
    }
    else if (variableName == Double::UNUSED1)
    {
        return pImpl->unused1;
    }
    else if (variableName == Double::UNUSED2)
    {
        return pImpl->unused2;
    }
    else if (variableName == Double::UNUSED3)
    {
        return pImpl->unused3;
    }
    else if (variableName == Double::UNUSED4)
    {
        return pImpl->unused4;
    }
    else if (variableName == Double::UNUSED5)
    {
        return pImpl->unused5;
    }
    else if (variableName == Double::UNUSED6)
    {
        return pImpl->unused6;
    }
#ifdef DEBUG
    else
    {
        assert(false);
    }
#endif
    return NULL_DOUBLE;
}

void Header::setHeader(const Double variableName, const double value)
{
    if (variableName == Double::DELTA)
    {
        if (value <= 0)
        {
            std::string errmsg = "Sampling period = "
                               + std::to_string(value) + " must be positive";
            throw std::invalid_argument(errmsg);
        }
        pImpl->delta = value;
    }
    else if (variableName == Double::DEPMIN)
    {
        pImpl->depmin = value;
    }
    else if (variableName == Double::DEPMAX)
    {
        pImpl->depmax = value;
    }
    else if (variableName == Double::SCALE)
    {
        pImpl->scale = value;
    }
    else if (variableName == Double::ODELTA)
    {
        pImpl->odelta = value;
    }
    else if (variableName == Double::B)
    {
        pImpl->b = value;
    }
    else if (variableName == Double::E)
    {
        pImpl->e = value;
    }
    else if (variableName == Double::O)
    {
        pImpl->o = value;
    }
    else if (variableName == Double::A)
    {
        pImpl->a = value;
    }
    else if (variableName == Double::INTERNAL1)
    {
        pImpl->internal1 = value;
    }
    else if (variableName == Double::T0)
    {
        pImpl->t0 = value;
    }
    else if (variableName == Double::T1)
    {
        pImpl->t1 = value;
    }
    else if (variableName == Double::T2)
    {
        pImpl->t2 = value;
    }
    else if (variableName == Double::T3)
    {
        pImpl->t3 = value;
    }
    else if (variableName == Double::T4)
    {
        pImpl->t4 = value;
    }
    else if (variableName == Double::T5)
    {
        pImpl->t5 = value;
    }
    else if (variableName == Double::T6)
    {
        pImpl->t6 = value;
    }
    else if (variableName == Double::T7)
    {
        pImpl->t7 = value;
    }
    else if (variableName == Double::T8)
    {
        pImpl->t8 = value;
    }
    else if (variableName == Double::T9)
    {
        pImpl->t9 = value;
    }
    else if (variableName == Double::F)
    {
        pImpl->f = value;
    }
    else if (variableName == Double::RESP0)
    {
        pImpl->resp0 = value;
    }
    else if (variableName == Double::RESP1)
    {
        pImpl->resp1 = value;
    }
    else if (variableName == Double::RESP2)
    {
        pImpl->resp2 = value;
    }
    else if (variableName == Double::RESP3)
    {
        pImpl->resp3 = value;
    }
    else if (variableName == Double::RESP4)
    {
        pImpl->resp4 = value;
    }
    else if (variableName == Double::RESP5)
    {
        pImpl->resp5 = value;
    }
    else if (variableName == Double::RESP6)
    {
        pImpl->resp6 = value;
    }
    else if (variableName == Double::RESP7)
    {
        pImpl->resp7 = value;
    }
    else if (variableName == Double::RESP8)
    {
        pImpl->resp8 = value;
    }
    else if (variableName == Double::RESP9)
    {
        pImpl->resp9 = value;
    }
    else if (variableName == Double::STLA)
    {
        pImpl->stla = value;
    }
    else if (variableName == Double::STLO)
    {
        pImpl->stlo = value;
    }
    else if (variableName == Double::STEL)
    {
        pImpl->stel = value;
    }
    else if (variableName == Double::STDP)
    {
        pImpl->stdp = value;
    }
    else if (variableName == Double::EVLA)
    {
        pImpl->evla = value;
    }
    else if (variableName == Double::EVLO)
    {
        pImpl->evlo = value;
    }
    else if (variableName == Double::EVEL)
    {
        pImpl->evel = value;
    }
    else if (variableName == Double::EVDP)
    {
        pImpl->evdp = value;
    }
    else if (variableName == Double::MAG)
    {
        pImpl->mag = value;
    }
    else if (variableName == Double::USER0)
    {
        pImpl->user0 = value;
    }
    else if (variableName == Double::USER1)
    {
        pImpl->user1 = value;
    }
    else if (variableName == Double::USER2)
    {
        pImpl->user2 = value;
    }
    else if (variableName == Double::USER3)
    {
        pImpl->user3 = value;
    }
    else if (variableName == Double::USER4)
    {
        pImpl->user4 = value;
    }
    else if (variableName == Double::USER5)
    {
        pImpl->user5 = value;
    }
    else if (variableName == Double::USER6)
    {
        pImpl->user6 = value;
    }
    else if (variableName == Double::USER7)
    {
        pImpl->user7 = value;
    }
    else if (variableName == Double::USER8)
    {
        pImpl->user8 = value;
    }
    else if (variableName == Double::USER9)
    {
        pImpl->user9 = value;
    }
    else if (variableName == Double::DIST)
    {
        pImpl->dist = value;
    }
    else if (variableName == Double::AZ)
    {
        pImpl->az = value;
    }
    else if (variableName == Double::BAZ)
    {
        pImpl->baz = value;
    }
    else if (variableName == Double::GCARC)
    {
        pImpl->gcarc = value;
    }
    else if (variableName == Double::INTERNAL2)
    {
        pImpl->internal2 = value;
    }
    else if (variableName == Double::INTERNAL3)
    {
        pImpl->internal3 = value;
    }
    else if (variableName == Double::DEPMEN)
    {
        pImpl->depmen = value;
    }
    else if (variableName == Double::CMPAZ)
    {
        pImpl->cmpaz = value;
    }
    else if (variableName == Double::CMPINC)
    {
        pImpl->cmpinc = value;
    }
    else if (variableName == Double::XMINIMUM)
    {
        pImpl->xminimum = value;
    }
    else if (variableName == Double::XMAXIMUM)
    {
        pImpl->xmaximum = value;
    }
    else if (variableName == Double::YMINIMUM)
    {
        pImpl->yminimum = value;
    }
    else if (variableName == Double::YMAXIMUM)
    {
        pImpl->ymaximum = value;
    }
    else if (variableName == Double::UNUSED0)
    {
        pImpl->unused0 = value;
    }
    else if (variableName == Double::UNUSED1)
    {
        pImpl->unused1 = value;
    }
    else if (variableName == Double::UNUSED2)
    {
        pImpl->unused2 = value;
    }
    else if (variableName == Double::UNUSED3)
    {
        pImpl->unused3 = value;
    }
    else if (variableName == Double::UNUSED4)
    {
        pImpl->unused4 = value;
    }
    else if (variableName == Double::UNUSED5)
    {
        pImpl->unused5 = value;
    }
    else if (variableName == Double::UNUSED6)
    {
        pImpl->unused6 = value;
    }
#ifdef DEBUG
    else
    {
        assert(false);
    }
#endif
}

//============================================================================//

void Header::setHeader(const Integer variableName, const int value)
{
    if (variableName == Integer::NZYEAR)
    {
        pImpl->nzyear = value;
    }
    else if (variableName == Integer::NZJDAY)
    {
        if (value < 1 || value > 366)
        {
            std::string errmsg = "nzjday = " + std::to_string(value)
                               + "must be in range [1,366]";
            throw std::invalid_argument(errmsg);
        }
        pImpl->nzjday = value;
    }
    else if (variableName == Integer::NZHOUR)
    {
        if (value < 0 || value > 23)
        {
            std::string errmsg = "nzhour = " + std::to_string(value)
                               + "must be in range [0,23]";
            throw std::invalid_argument(errmsg);
        }
        pImpl->nzhour = value;
    }
    else if (variableName == Integer::NZMIN)
    {
        if (value < 0 || value > 59)
        {
            std::string errmsg = "nzmin = " + std::to_string(value)
                               + "must be in range [0,59]";
            throw std::invalid_argument(errmsg);
        }
        pImpl->nzmin = value;
    }
    else if (variableName == Integer::NZSEC)
    {
        if (value < 0 || value > 59)
        {
            std::string errmsg = "nzsec = " + std::to_string(value)
                               + "must be in range [0,59]";
            throw std::invalid_argument(errmsg);
        }
        pImpl->nzsec = value;
    }
    else if (variableName == Integer::NZMSEC)
    {
        if (value < 0 || value > 999)
        {
            std::string errmsg = "nzmseec = " + std::to_string(value)
                               + "must be in range [0,999]";
            throw std::invalid_argument(errmsg);
        }
        pImpl->nzmsec = value;
    }
    else if (variableName == Integer::NVHDR)
    {
        pImpl->nvhdr = value;
    }
    else if (variableName == Integer::NORID)
    {
        pImpl->norid = value;
    }
    else if (variableName == Integer::NEVID)
    {
        pImpl->nevid = value;
    }
    else if (variableName == Integer::NPTS)
    {
        if (value < 0)
        {
            std::string errmsg = "npts = " + std::to_string(value)
                               + "cannot be negative";
            throw std::invalid_argument(errmsg);
        }
        pImpl->npts = value;
    }
    else if (variableName == Integer::INTERNAL1)
    {
        pImpl->iinternal1 = value;
    }
    else if (variableName == Integer::NWFID)
    {
        pImpl->nwfid = value;
    }
    else if (variableName == Integer::NXSIZE)
    {
        pImpl->nxsize = value;
    }
    else if (variableName == Integer::NYSIZE)
    {
        pImpl->nysize = value;
    }
    else if (variableName == Integer::UNUSED0)
    {
        pImpl->iunused0 = value;
    }
    else if (variableName == Integer::IFTYPE)
    {
        pImpl->iftype = value;
    }
    else if (variableName == Integer::IDEP)
    {
        pImpl->idep = value;
    }
    else if (variableName == Integer::IZTYPE)
    {
        pImpl->iztype = value;
    }
    else if (variableName == Integer::UNUSED1)
    {
        pImpl->iunused1 = value;
    }
    else if (variableName == Integer::IINST)
    {
        pImpl->iinst = value;
    }
    else if (variableName == Integer::ISTREG)
    {
        pImpl->istreg = value;
    }
    else if (variableName == Integer::IEVREG)
    {
        pImpl->ievreg = value;
    }
    else if (variableName == Integer::IEVTYP)
    {
        pImpl->ievtyp = value;
    }
    else if (variableName == Integer::IQUAL)
    {
        pImpl->iqual = value;
    }
    else if (variableName == Integer::ISYNTH)
    {
        pImpl->isynth = value;
    }
    else if (variableName == Integer::IMAGTYP)
    {
        pImpl->imagtyp = value;
    }
    else if (variableName == Integer::IMAGSRC)
    {
        pImpl->imagsrc = value;
    }
    else if (variableName == Integer::UNUSED2)
    {
        pImpl->iunused2 = value;
    }
    else if (variableName == Integer::UNUSED3)
    {
        pImpl->iunused3 = value;
    }
    else if (variableName == Integer::UNUSED4)
    {
        pImpl->iunused4 = value;
    }
    else if (variableName == Integer::UNUSED5)
    {
        pImpl->iunused5 = value;
    }
    else if (variableName == Integer::UNUSED6)
    {
        pImpl->iunused6 = value;
    }
    else if (variableName == Integer::UNUSED7)
    {
        pImpl->iunused7 = value;
    }
    else if (variableName == Integer::UNUSED8)
    {
        pImpl->iunused8 = value;
    }
    else if (variableName == Integer::UNUSED9)
    {
        pImpl->iunused9 = value;
    }
#ifdef DEBUG
    else
    {
        assert(false);
    }
#endif
}

int Header::getHeader(const Integer variableName) const noexcept
{
    if (variableName == Integer::NZYEAR)
    {
        return pImpl->nzyear;
    }
    else if (variableName == Integer::NZJDAY)
    {
        return pImpl->nzjday;
    }
    else if (variableName == Integer::NZHOUR)
    {
        return pImpl->nzhour;
    }
    else if (variableName == Integer::NZMIN)
    {
        return pImpl->nzmin;
    }
    else if (variableName == Integer::NZSEC)
    {
        return pImpl->nzsec;
    }
    else if (variableName == Integer::NZMSEC)
    {
        return pImpl->nzmsec;
    }
    else if (variableName == Integer::NVHDR)
    {
        return pImpl->nvhdr;
    }
    else if (variableName == Integer::NORID)
    {
        return pImpl->norid;
    }
    else if (variableName == Integer::NEVID)
    {
        return pImpl->nevid;
    }
    else if (variableName == Integer::NPTS)
    {
        return pImpl->npts;
    }
    else if (variableName == Integer::INTERNAL1)
    {
        return pImpl->iinternal1;
    }
    else if (variableName == Integer::NWFID)
    {
        return pImpl->nwfid;
    }
    else if (variableName == Integer::NXSIZE)
    {
        return pImpl->nxsize;
    }
    else if (variableName == Integer::NYSIZE)
    {
        return pImpl->nysize;
    }
    else if (variableName == Integer::UNUSED0)
    {
        return pImpl->iunused0;
    }
    else if (variableName == Integer::IFTYPE)
    {
        return pImpl->iftype;
    }
    else if (variableName == Integer::IDEP)
    {
        return pImpl->idep;
    }
    else if (variableName == Integer::IZTYPE)
    {
        return pImpl->iztype;
    }
    else if (variableName == Integer::UNUSED1)
    {
        return pImpl->iunused1;
    }
    else if (variableName == Integer::IINST)
    {
        return pImpl->iinst;
    }
    else if (variableName == Integer::ISTREG)
    {
        return pImpl->istreg;
    }
    else if (variableName == Integer::IEVREG)
    {
        return pImpl->ievreg;
    }
    else if (variableName == Integer::IEVTYP)
    {
        return pImpl->ievtyp;
    }
    else if (variableName == Integer::IQUAL)
    {
        return pImpl->iqual;
    }
    else if (variableName == Integer::ISYNTH)
    {
        return pImpl->isynth;
    }
    else if (variableName == Integer::IMAGTYP)
    {
        return pImpl->imagtyp;
    }
    else if (variableName == Integer::IMAGSRC)
    {
        return pImpl->imagsrc;
    }
    else if (variableName == Integer::UNUSED2)
    {
        return pImpl->iunused2;
    }
    else if (variableName == Integer::UNUSED3)
    {
        return pImpl->iunused3;
    }
    else if (variableName == Integer::UNUSED4)
    {
        return pImpl->iunused4;
    }
    else if (variableName == Integer::UNUSED5)
    {
        return pImpl->iunused5;
    }
    else if (variableName == Integer::UNUSED6)
    {
        return pImpl->iunused6;
    }
    else if (variableName == Integer::UNUSED7)
    {
        return pImpl->iunused7;
    }
    else if (variableName == Integer::UNUSED8)
    {
        return pImpl->iunused8;
    }
    else if (variableName == Integer::UNUSED9)
    {
        return pImpl->iunused9;
    }
#ifdef DEBUG
    else
    {
        assert(false);
    }
#endif
    return NULL_INT;
}

//============================================================================//

//============================================================================//

void Header::setHeader(const Character variableName,
                       const std::string &value) noexcept
{
    if (variableName == Character::KSTNM)
    {
        copyTruncatedString(value, pImpl->kstnm);
    }
    else if (variableName == Character::KEVNM)
    {
        copyTruncatedString(value, pImpl->kevnm);
    }
    else if (variableName == Character::KHOLE)
    {
        copyTruncatedString(value, pImpl->khole);
    }
    else if (variableName == Character::KO)
    {
        copyTruncatedString(value, pImpl->ko);
    }
    else if (variableName == Character::KA)
    {
        copyTruncatedString(value, pImpl->ka);
    }
    else if (variableName == Character::KT0)
    {
        copyTruncatedString(value, pImpl->kt0);
    }
    else if (variableName == Character::KT1)
    {
        copyTruncatedString(value, pImpl->kt1);
    }
    else if (variableName == Character::KT2)
    {
        copyTruncatedString(value, pImpl->kt2);
    }
    else if (variableName == Character::KT3)
    {
        copyTruncatedString(value, pImpl->kt3);
    }
    else if (variableName == Character::KT4)
    {
        copyTruncatedString(value, pImpl->kt4);
    }
    else if (variableName == Character::KT5)
    {
        copyTruncatedString(value, pImpl->kt5);
    }
    else if (variableName == Character::KT6)
    {
        copyTruncatedString(value, pImpl->kt6);
    }
    else if (variableName == Character::KT7)
    {
        copyTruncatedString(value, pImpl->kt7);
    }
    else if (variableName == Character::KT8)
    {
        copyTruncatedString(value, pImpl->kt8);
    }
    else if (variableName == Character::KT9)
    {
        copyTruncatedString(value, pImpl->kt9);
    }
    else if (variableName == Character::KF)
    {
        copyTruncatedString(value, pImpl->kf);
    }
    else if (variableName == Character::KUSER0)
    {
        copyTruncatedString(value, pImpl->kuser0);
    }
    else if (variableName == Character::KUSER1)
    {
        copyTruncatedString(value, pImpl->kuser1);
    }
    else if (variableName == Character::KUSER2)
    {
        copyTruncatedString(value, pImpl->kuser2);
    }
    else if (variableName == Character::KCMPNM)
    {
        copyTruncatedString(value, pImpl->kcmpnm);
    }
    else if (variableName == Character::KDATRD)
    {
        copyTruncatedString(value, pImpl->kdatrd);
    }
    else if (variableName == Character::KINST)
    {
        copyTruncatedString(value, pImpl->kinst);
    }
#ifdef DEBUG
    else
    {
        assert(false);
    }
#endif
}

std::string Header::getHeader(const Character variableName) const noexcept
{
    if (variableName == Character::KSTNM)
    {
        return pImpl->kstnm;
    }
    else if (variableName == Character::KEVNM)
    {
        return pImpl->kevnm;
    }
    else if (variableName == Character::KHOLE)
    {
        return pImpl->khole;
    }
    else if (variableName == Character::KO)
    {
        return pImpl->ko;
    }
    else if (variableName == Character::KA)
    {
        return pImpl->ka;
    }
    else if (variableName == Character::KT0)
    {
        return pImpl->kt0;
    }
    else if (variableName == Character::KT1)
    {   
        return pImpl->kt1;
    }
    else if (variableName == Character::KT2)
    {
        return pImpl->kt2;
    }
    else if (variableName == Character::KT3)
    {
        return pImpl->kt3;
    }
    else if (variableName == Character::KT4)
    {
        return pImpl->kt4;
    }
    else if (variableName == Character::KT5)
    {
        return pImpl->kt5;
    }
    else if (variableName == Character::KT6)
    {
        return pImpl->kt6;
    }
    else if (variableName == Character::KT7)
    {
        return pImpl->kt7;
    }
    else if (variableName == Character::KT8)
    {
        return pImpl->kt8;
    }
    else if (variableName == Character::KT9)
    {
        return pImpl->kt9;
    }
    else if (variableName == Character::KF)
    {
        return pImpl->kf;
    }
    else if (variableName == Character::KUSER0)
    {
        return pImpl->kuser0;
    }
    else if (variableName == Character::KUSER1)
    {
        return pImpl->kuser1;
    }
    else if (variableName == Character::KUSER2)
    {
        return pImpl->kuser2;
    }
    else if (variableName == Character::KCMPNM)
    {
        return pImpl->kcmpnm;
    }
    else if (variableName == Character::KNETWK)
    {
        return pImpl->knetwk;
    }
    else if (variableName == Character::KDATRD)
    {
        return pImpl->kdatrd;
    }
    else if (variableName == Character::KINST)
    {
        return pImpl->kinst;
    }
#ifdef DEBUG
    else
    {
        assert(false);
    }
#endif
    return NULL_STRING;
}

