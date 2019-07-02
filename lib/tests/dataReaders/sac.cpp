#include <cstdio>
#include <cstdlib>
#include <climits>
#include <cmath>
#include <algorithm>
#include "temblor/library/private/filesystem.hpp"
#include "temblor/library/dataReaders/sac/waveform.hpp"
#include "temblor/library/dataReaders/sac/header.hpp"
#include "temblor/library/dataReaders/sac/enums.hpp"
#include <gtest/gtest.h>

namespace
{

using namespace Temblor::Library::DataReaders;

TEST(LibraryDataReadersSAC, header)
{
    SAC::Header header;
    // Set the doubles
    header.setHeader(SAC::Double::DELTA,     0.01);
    header.setHeader(SAC::Double::DEPMIN,    0.02);
    header.setHeader(SAC::Double::DEPMAX,    0.03);
    header.setHeader(SAC::Double::SCALE,     0.04);
    header.setHeader(SAC::Double::ODELTA,    0.05);
    header.setHeader(SAC::Double::B,         0.06);
    header.setHeader(SAC::Double::E,         0.07);
    header.setHeader(SAC::Double::O,         0.08);
    header.setHeader(SAC::Double::A,         0.09);
    header.setHeader(SAC::Double::INTERNAL1, 0.10);
    header.setHeader(SAC::Double::T0,        0.11);
    header.setHeader(SAC::Double::T1,        0.12);
    header.setHeader(SAC::Double::T2,        0.13);
    header.setHeader(SAC::Double::T3,        0.14);
    header.setHeader(SAC::Double::T4,        0.15);
    header.setHeader(SAC::Double::T5,        0.16);
    header.setHeader(SAC::Double::T6,        0.17);
    header.setHeader(SAC::Double::T7,        0.18);
    header.setHeader(SAC::Double::T8,        0.19);
    header.setHeader(SAC::Double::T9,        0.20);
    header.setHeader(SAC::Double::F,         0.21);
    header.setHeader(SAC::Double::RESP0,     0.22);
    header.setHeader(SAC::Double::RESP1,     0.23);
    header.setHeader(SAC::Double::RESP2,     0.24);
    header.setHeader(SAC::Double::RESP3,     0.25);
    header.setHeader(SAC::Double::RESP4,     0.26);
    header.setHeader(SAC::Double::RESP5,     0.27);
    header.setHeader(SAC::Double::RESP6,     0.28);
    header.setHeader(SAC::Double::RESP7,     0.29);
    header.setHeader(SAC::Double::RESP8,     0.30);
    header.setHeader(SAC::Double::RESP9,     0.31);
    header.setHeader(SAC::Double::STLA,      0.32);
    header.setHeader(SAC::Double::STLO,      0.33);
    header.setHeader(SAC::Double::STEL,      0.34);
    header.setHeader(SAC::Double::STDP,      0.35);
    header.setHeader(SAC::Double::EVLA,      0.36);
    header.setHeader(SAC::Double::EVLO,      0.37);
    header.setHeader(SAC::Double::EVEL,      0.38);
    header.setHeader(SAC::Double::EVDP,      0.39);
    header.setHeader(SAC::Double::MAG,       0.40);
    header.setHeader(SAC::Double::USER0,     0.41);
    header.setHeader(SAC::Double::USER1,     0.42);
    header.setHeader(SAC::Double::USER2,     0.43);
    header.setHeader(SAC::Double::USER3,     0.44);
    header.setHeader(SAC::Double::USER4,     0.45);
    header.setHeader(SAC::Double::USER5,     0.46);
    header.setHeader(SAC::Double::USER6,     0.47);
    header.setHeader(SAC::Double::USER7,     0.48);
    header.setHeader(SAC::Double::USER8,     0.49);
    header.setHeader(SAC::Double::USER9,     0.50);
    header.setHeader(SAC::Double::DIST,      0.51);
    header.setHeader(SAC::Double::AZ,        0.52);
    header.setHeader(SAC::Double::BAZ,       0.53);
    header.setHeader(SAC::Double::GCARC,     0.54);
    header.setHeader(SAC::Double::INTERNAL2, 0.55);
    header.setHeader(SAC::Double::INTERNAL3, 0.56);
    header.setHeader(SAC::Double::DEPMEN,    0.57);
    header.setHeader(SAC::Double::CMPAZ,     0.58);
    header.setHeader(SAC::Double::CMPINC,    0.59);
    header.setHeader(SAC::Double::XMINIMUM,  0.60);
    header.setHeader(SAC::Double::XMAXIMUM,  0.61);
    header.setHeader(SAC::Double::YMINIMUM,  0.62);
    header.setHeader(SAC::Double::YMAXIMUM,  0.63);
    header.setHeader(SAC::Double::UNUSED0,   0.64);
    header.setHeader(SAC::Double::UNUSED1,   0.65);
    header.setHeader(SAC::Double::UNUSED2,   0.66);
    header.setHeader(SAC::Double::UNUSED3,   0.67);
    header.setHeader(SAC::Double::UNUSED4,   0.68);
    header.setHeader(SAC::Double::UNUSED5,   0.69);
    header.setHeader(SAC::Double::UNUSED6,   0.70);
    // Set the integers
    header.setHeader(SAC::Integer::NZYEAR,      1);
    header.setHeader(SAC::Integer::NZJDAY,      2);
    header.setHeader(SAC::Integer::NZHOUR,      3);
    header.setHeader(SAC::Integer::NZMIN,       4);
    header.setHeader(SAC::Integer::NZSEC,       5);
    header.setHeader(SAC::Integer::NZMSEC,      6);
    header.setHeader(SAC::Integer::NVHDR,       7);
    header.setHeader(SAC::Integer::NORID,       8);
    header.setHeader(SAC::Integer::NEVID,       9);
    header.setHeader(SAC::Integer::NPTS,       10);
    header.setHeader(SAC::Integer::INTERNAL1,  11);
    header.setHeader(SAC::Integer::NWFID,      12);
    header.setHeader(SAC::Integer::NXSIZE,     13);
    header.setHeader(SAC::Integer::NYSIZE,     14);
    header.setHeader(SAC::Integer::UNUSED0,    15);
    header.setHeader(SAC::Integer::IFTYPE,     16);
    header.setHeader(SAC::Integer::IDEP,       17);
    header.setHeader(SAC::Integer::IZTYPE,     18);
    header.setHeader(SAC::Integer::UNUSED1,    19);
    header.setHeader(SAC::Integer::IINST,      20);
    header.setHeader(SAC::Integer::ISTREG,     21);
    header.setHeader(SAC::Integer::IEVREG,     22);
    header.setHeader(SAC::Integer::IEVTYP,     23);
    header.setHeader(SAC::Integer::IQUAL,      24);
    header.setHeader(SAC::Integer::ISYNTH,     25);
    header.setHeader(SAC::Integer::IMAGTYP,    26);
    header.setHeader(SAC::Integer::IMAGSRC,    27);
    header.setHeader(SAC::Integer::UNUSED2,    28);
    header.setHeader(SAC::Integer::UNUSED3,    29);
    header.setHeader(SAC::Integer::UNUSED4,    30);
    header.setHeader(SAC::Integer::UNUSED5,    31);
    header.setHeader(SAC::Integer::UNUSED6,    32);
    header.setHeader(SAC::Integer::UNUSED7,    33);
    header.setHeader(SAC::Integer::UNUSED8,    34);
    header.setHeader(SAC::Integer::UNUSED9,    35);
    // Set the logicals
    header.setHeader(SAC::Logical::LEVEN,  false);
    header.setHeader(SAC::Logical::LPSPOL, true);
    //header.setHeader(SAC::Logical::LOVROK, -12345); // undefined
    header.setHeader(SAC::Logical::LCALDA, true);
    header.setHeader(SAC::Logical::UNUSED, false);
    // Set the strings
    header.setHeader(SAC::Character::KSTNM, "kstnm");
    header.setHeader(SAC::Character::KEVNM, "length sixteen");
    header.setHeader(SAC::Character::KHOLE, "truncate me"); // truncate
    //header.setHeader(SAC::Character::KO,  "ko")); // do nothing
    header.setHeader(SAC::Character::KA,    "P");
    header.setHeader(SAC::Character::KT0,   "S");
    header.setHeader(SAC::Character::KT1,   "");
    header.setHeader(SAC::Character::KT2,   "kt2");
    header.setHeader(SAC::Character::KT3,   "kt3");
    header.setHeader(SAC::Character::KT4,   "kt4");
    header.setHeader(SAC::Character::KT5,   "kt5");
    header.setHeader(SAC::Character::KT6,   "kt6");
    header.setHeader(SAC::Character::KT7,   "kt7");
    header.setHeader(SAC::Character::KT8,   "kt8");
    header.setHeader(SAC::Character::KT9,   "kt9");
    header.setHeader(SAC::Character::KF,    "kf");
    header.setHeader(SAC::Character::KUSER0, "kuser0");
    header.setHeader(SAC::Character::KUSER1, "kuser1");
    header.setHeader(SAC::Character::KUSER2, "kuser2");
    header.setHeader(SAC::Character::KCMPNM, "kcmpnm");
    header.setHeader(SAC::Character::KNETWK, "knetwk");
    header.setHeader(SAC::Character::KDATRD, "kdatrd");
    header.setHeader(SAC::Character::KINST,  "kinst");

    // Implicitly tests the copy as well
    SAC::Header headerCheck(header);
    ASSERT_NEAR(headerCheck.getHeader(SAC::Double::DELTA),     0.01, 1.e-14);
    ASSERT_NEAR(headerCheck.getHeader(SAC::Double::DEPMIN),    0.02, 1.e-14);
    ASSERT_NEAR(headerCheck.getHeader(SAC::Double::DEPMAX),    0.03, 1.e-14);
    ASSERT_NEAR(headerCheck.getHeader(SAC::Double::SCALE),     0.04, 1.e-14);
    ASSERT_NEAR(headerCheck.getHeader(SAC::Double::ODELTA),    0.05, 1.e-14);
    ASSERT_NEAR(headerCheck.getHeader(SAC::Double::B),         0.06, 1.e-14);
    ASSERT_NEAR(headerCheck.getHeader(SAC::Double::E),         0.07, 1.e-14);
    ASSERT_NEAR(headerCheck.getHeader(SAC::Double::O),         0.08, 1.e-14);
    ASSERT_NEAR(headerCheck.getHeader(SAC::Double::A),         0.09, 1.e-14);
    ASSERT_NEAR(headerCheck.getHeader(SAC::Double::INTERNAL1), 0.10, 1.e-14);
    ASSERT_NEAR(headerCheck.getHeader(SAC::Double::T0),        0.11, 1.e-14);
    ASSERT_NEAR(headerCheck.getHeader(SAC::Double::T1),        0.12, 1.e-14);
    ASSERT_NEAR(headerCheck.getHeader(SAC::Double::T2),        0.13, 1.e-14);
    ASSERT_NEAR(headerCheck.getHeader(SAC::Double::T3),        0.14, 1.e-14);
    ASSERT_NEAR(headerCheck.getHeader(SAC::Double::T4),        0.15, 1.e-14);
    ASSERT_NEAR(headerCheck.getHeader(SAC::Double::T5),        0.16, 1.e-14);
    ASSERT_NEAR(headerCheck.getHeader(SAC::Double::T6),        0.17, 1.e-14);
    ASSERT_NEAR(headerCheck.getHeader(SAC::Double::T7),        0.18, 1.e-14);
    ASSERT_NEAR(headerCheck.getHeader(SAC::Double::T8),        0.19, 1.e-14);
    ASSERT_NEAR(headerCheck.getHeader(SAC::Double::T9),        0.20, 1.e-14);
    ASSERT_NEAR(headerCheck.getHeader(SAC::Double::F),         0.21, 1.e-14);
    ASSERT_NEAR(headerCheck.getHeader(SAC::Double::RESP0),     0.22, 1.e-14);
    ASSERT_NEAR(headerCheck.getHeader(SAC::Double::RESP1),     0.23, 1.e-14);
    ASSERT_NEAR(headerCheck.getHeader(SAC::Double::RESP2),     0.24, 1.e-14);
    ASSERT_NEAR(headerCheck.getHeader(SAC::Double::RESP3),     0.25, 1.e-14);
    ASSERT_NEAR(headerCheck.getHeader(SAC::Double::RESP4),     0.26, 1.e-14);
    ASSERT_NEAR(headerCheck.getHeader(SAC::Double::RESP5),     0.27, 1.e-14);
    ASSERT_NEAR(headerCheck.getHeader(SAC::Double::RESP6),     0.28, 1.e-14);
    ASSERT_NEAR(headerCheck.getHeader(SAC::Double::RESP7),     0.29, 1.e-14);
    ASSERT_NEAR(headerCheck.getHeader(SAC::Double::RESP8),     0.30, 1.e-14);
    ASSERT_NEAR(headerCheck.getHeader(SAC::Double::RESP9),     0.31, 1.e-14);
    ASSERT_NEAR(headerCheck.getHeader(SAC::Double::STLA),      0.32, 1.e-14);
    ASSERT_NEAR(headerCheck.getHeader(SAC::Double::STLO),      0.33, 1.e-14);
    ASSERT_NEAR(headerCheck.getHeader(SAC::Double::STEL),      0.34, 1.e-14);
    ASSERT_NEAR(headerCheck.getHeader(SAC::Double::STDP),      0.35, 1.e-14);
    ASSERT_NEAR(headerCheck.getHeader(SAC::Double::EVLA),      0.36, 1.e-14);
    ASSERT_NEAR(headerCheck.getHeader(SAC::Double::EVLO),      0.37, 1.e-14);
    ASSERT_NEAR(headerCheck.getHeader(SAC::Double::EVEL),      0.38, 1.e-14);
    ASSERT_NEAR(headerCheck.getHeader(SAC::Double::EVDP),      0.39, 1.e-14);
    ASSERT_NEAR(headerCheck.getHeader(SAC::Double::MAG),       0.40, 1.e-14);
    ASSERT_NEAR(headerCheck.getHeader(SAC::Double::USER0),     0.41, 1.e-14);
    ASSERT_NEAR(headerCheck.getHeader(SAC::Double::USER1),     0.42, 1.e-14);
    ASSERT_NEAR(headerCheck.getHeader(SAC::Double::USER2),     0.43, 1.e-14);
    ASSERT_NEAR(headerCheck.getHeader(SAC::Double::USER3),     0.44, 1.e-14);
    ASSERT_NEAR(headerCheck.getHeader(SAC::Double::USER4),     0.45, 1.e-14);
    ASSERT_NEAR(headerCheck.getHeader(SAC::Double::USER5),     0.46, 1.e-14);
    ASSERT_NEAR(headerCheck.getHeader(SAC::Double::USER6),     0.47, 1.e-14);
    ASSERT_NEAR(headerCheck.getHeader(SAC::Double::USER7),     0.48, 1.e-14);
    ASSERT_NEAR(headerCheck.getHeader(SAC::Double::USER8),     0.49, 1.e-14);
    ASSERT_NEAR(headerCheck.getHeader(SAC::Double::USER9),     0.50, 1.e-14);
    ASSERT_NEAR(headerCheck.getHeader(SAC::Double::DIST),      0.51, 1.e-14);
    ASSERT_NEAR(headerCheck.getHeader(SAC::Double::AZ),        0.52, 1.e-14);
    ASSERT_NEAR(headerCheck.getHeader(SAC::Double::BAZ),       0.53, 1.e-14);
    ASSERT_NEAR(headerCheck.getHeader(SAC::Double::GCARC),     0.54, 1.e-14);
    ASSERT_NEAR(headerCheck.getHeader(SAC::Double::INTERNAL2), 0.55, 1.e-14);
    ASSERT_NEAR(headerCheck.getHeader(SAC::Double::INTERNAL3), 0.56, 1.e-14);
    ASSERT_NEAR(headerCheck.getHeader(SAC::Double::DEPMEN),    0.57, 1.e-14);
    ASSERT_NEAR(headerCheck.getHeader(SAC::Double::CMPAZ),     0.58, 1.e-14);
    ASSERT_NEAR(headerCheck.getHeader(SAC::Double::CMPINC),    0.59, 1.e-14);
    ASSERT_NEAR(headerCheck.getHeader(SAC::Double::XMINIMUM),  0.60, 1.e-14);
    ASSERT_NEAR(headerCheck.getHeader(SAC::Double::XMAXIMUM),  0.61, 1.e-14);
    ASSERT_NEAR(headerCheck.getHeader(SAC::Double::YMINIMUM),  0.62, 1.e-14);
    ASSERT_NEAR(headerCheck.getHeader(SAC::Double::YMAXIMUM),  0.63, 1.e-14);
    ASSERT_NEAR(headerCheck.getHeader(SAC::Double::UNUSED0),   0.64, 1.e-14);
    ASSERT_NEAR(headerCheck.getHeader(SAC::Double::UNUSED1),   0.65, 1.e-14);
    ASSERT_NEAR(headerCheck.getHeader(SAC::Double::UNUSED2),   0.66, 1.e-14);
    ASSERT_NEAR(headerCheck.getHeader(SAC::Double::UNUSED3),   0.67, 1.e-14);
    ASSERT_NEAR(headerCheck.getHeader(SAC::Double::UNUSED4),   0.68, 1.e-14);
    ASSERT_NEAR(headerCheck.getHeader(SAC::Double::UNUSED5),   0.69, 1.e-14);
    ASSERT_NEAR(headerCheck.getHeader(SAC::Double::UNUSED6),   0.70, 1.e-14);
    // Test the integers
    ASSERT_EQ(headerCheck.getHeader(SAC::Integer::NZYEAR),      1);
    ASSERT_EQ(headerCheck.getHeader(SAC::Integer::NZJDAY),      2);
    ASSERT_EQ(headerCheck.getHeader(SAC::Integer::NZHOUR),      3);
    ASSERT_EQ(headerCheck.getHeader(SAC::Integer::NZMIN),       4);
    ASSERT_EQ(headerCheck.getHeader(SAC::Integer::NZSEC),       5);
    ASSERT_EQ(headerCheck.getHeader(SAC::Integer::NZMSEC),      6);
    ASSERT_EQ(headerCheck.getHeader(SAC::Integer::NVHDR),       7);
    ASSERT_EQ(headerCheck.getHeader(SAC::Integer::NORID),       8);
    ASSERT_EQ(headerCheck.getHeader(SAC::Integer::NEVID),       9);
    ASSERT_EQ(headerCheck.getHeader(SAC::Integer::NPTS),       10);
    ASSERT_EQ(headerCheck.getHeader(SAC::Integer::INTERNAL1),  11);
    ASSERT_EQ(headerCheck.getHeader(SAC::Integer::NWFID),      12);
    ASSERT_EQ(headerCheck.getHeader(SAC::Integer::NXSIZE),     13);
    ASSERT_EQ(headerCheck.getHeader(SAC::Integer::NYSIZE),     14);
    ASSERT_EQ(headerCheck.getHeader(SAC::Integer::UNUSED0),    15);
    ASSERT_EQ(headerCheck.getHeader(SAC::Integer::IFTYPE),     16);
    ASSERT_EQ(headerCheck.getHeader(SAC::Integer::IDEP),       17);
    ASSERT_EQ(headerCheck.getHeader(SAC::Integer::IZTYPE),     18);
    ASSERT_EQ(headerCheck.getHeader(SAC::Integer::UNUSED1),    19);
    ASSERT_EQ(headerCheck.getHeader(SAC::Integer::IINST),      20);
    ASSERT_EQ(headerCheck.getHeader(SAC::Integer::ISTREG),     21);
    ASSERT_EQ(headerCheck.getHeader(SAC::Integer::IEVREG),     22);
    ASSERT_EQ(headerCheck.getHeader(SAC::Integer::IEVTYP),     23);
    ASSERT_EQ(headerCheck.getHeader(SAC::Integer::IQUAL),      24);
    ASSERT_EQ(headerCheck.getHeader(SAC::Integer::ISYNTH),     25);
    ASSERT_EQ(headerCheck.getHeader(SAC::Integer::IMAGTYP),    26);
    ASSERT_EQ(headerCheck.getHeader(SAC::Integer::IMAGSRC),    27);
    ASSERT_EQ(headerCheck.getHeader(SAC::Integer::UNUSED2),    28);
    ASSERT_EQ(headerCheck.getHeader(SAC::Integer::UNUSED3),    29);
    ASSERT_EQ(headerCheck.getHeader(SAC::Integer::UNUSED4),    30);
    ASSERT_EQ(headerCheck.getHeader(SAC::Integer::UNUSED5),    31);
    ASSERT_EQ(headerCheck.getHeader(SAC::Integer::UNUSED6),    32);
    ASSERT_EQ(headerCheck.getHeader(SAC::Integer::UNUSED7),    33);
    ASSERT_EQ(headerCheck.getHeader(SAC::Integer::UNUSED8),    34);
    ASSERT_EQ(headerCheck.getHeader(SAC::Integer::UNUSED9),    35);
    // Test the logicals
    ASSERT_EQ(headerCheck.getHeader(SAC::Logical::LEVEN),       0);
    ASSERT_EQ(headerCheck.getHeader(SAC::Logical::LPSPOL),      1);
    ASSERT_EQ(headerCheck.getHeader(SAC::Logical::LOVROK), -12345);
    ASSERT_EQ(headerCheck.getHeader(SAC::Logical::LCALDA),      1);
    ASSERT_EQ(headerCheck.getHeader(SAC::Logical::UNUSED),      0);
    // Test the strings
    ASSERT_STREQ(headerCheck.getHeader(SAC::Character::KSTNM).c_str(),
                                       "kstnm");
    ASSERT_STREQ(headerCheck.getHeader(SAC::Character::KEVNM).c_str(),
                                       "length sixteen");
    ASSERT_STREQ(headerCheck.getHeader(SAC::Character::KHOLE).c_str(),
                                       "truncate");
    ASSERT_STREQ(headerCheck.getHeader(SAC::Character::KO).c_str(),
                                       "-12345");
    ASSERT_STREQ(headerCheck.getHeader(SAC::Character::KA).c_str(),
                                       "P");  
    ASSERT_STREQ(headerCheck.getHeader(SAC::Character::KT0).c_str(),
                                       "S");
    ASSERT_STREQ(headerCheck.getHeader(SAC::Character::KT1).c_str(),
                                       "");
    ASSERT_STREQ(headerCheck.getHeader(SAC::Character::KT2).c_str(),
                                       "kt2");
    ASSERT_STREQ(headerCheck.getHeader(SAC::Character::KT3).c_str(),
                                       "kt3");
    ASSERT_STREQ(headerCheck.getHeader(SAC::Character::KT4).c_str(),
                                       "kt4");
    ASSERT_STREQ(headerCheck.getHeader(SAC::Character::KT5).c_str(),
                                       "kt5");
    ASSERT_STREQ(headerCheck.getHeader(SAC::Character::KT6).c_str(),
                                       "kt6");
    ASSERT_STREQ(headerCheck.getHeader(SAC::Character::KT7).c_str(),
                                       "kt7");
    ASSERT_STREQ(headerCheck.getHeader(SAC::Character::KT8).c_str(),
                                       "kt8");
    ASSERT_STREQ(headerCheck.getHeader(SAC::Character::KT9).c_str(),
                                       "kt9");
    ASSERT_STREQ(headerCheck.getHeader(SAC::Character::KF).c_str(),
                                       "kf");
    ASSERT_STREQ(headerCheck.getHeader(SAC::Character::KUSER0).c_str(),
                                       "kuser0");
    ASSERT_STREQ(headerCheck.getHeader(SAC::Character::KUSER1).c_str(),
                                       "kuser1");
    ASSERT_STREQ(headerCheck.getHeader(SAC::Character::KUSER2).c_str(),
                                       "kuser2");
    ASSERT_STREQ(headerCheck.getHeader(SAC::Character::KCMPNM).c_str(),
                                       "kcmpnm");
    ASSERT_STREQ(headerCheck.getHeader(SAC::Character::KNETWK).c_str(),
                                       "knetwk");
    ASSERT_STREQ(headerCheck.getHeader(SAC::Character::KDATRD).c_str(),
                                       "kdatrd");
    ASSERT_STREQ(headerCheck.getHeader(SAC::Character::KINST).c_str(),
                                       "kinst");
}

TEST(LibraryDataReadersSAC, waveform)
{
    const std::string sacFile = "data/debug.sac";
    SAC::Waveform waveform; 
    ASSERT_EQ(waveform.getHeader(SAC::Integer::NVHDR), 6);
    ASSERT_EQ(waveform.getHeader(SAC::Integer::IFTYPE), 1);
    waveform.read(sacFile);
    ASSERT_EQ(waveform.getNumberOfSamples(), 100);    
    ASSERT_NEAR(waveform.getSamplingPeriod(), 0.005, 1.e-7);
    ASSERT_STREQ(waveform.getHeader(SAC::Character::KNETWK).c_str(), "FK");
    ASSERT_STREQ(waveform.getHeader(SAC::Character::KSTNM).c_str(),  "NEW");
    ASSERT_STREQ(waveform.getHeader(SAC::Character::KCMPNM).c_str(), "HHZ");
    ASSERT_STREQ(waveform.getHeader(SAC::Character::KHOLE).c_str(),  "10");
    const double *dPtr = waveform.getDataPointer();
    double resmax = 0;
    for (int i=0; i<waveform.getNumberOfSamples(); ++i)
    {
        double res = dPtr[i] - static_cast<double> (i + 1);
        resmax = std::max(std::abs(res), resmax);
    }
    ASSERT_NEAR(resmax, 0.0, 1.e-7);

    std::vector<double> dVec = waveform.getData();
    resmax = 0;
    for (int i=0; i<waveform.getNumberOfSamples(); ++i)
    {
        double res = dVec[i] - static_cast<double> (i + 1);
        resmax = std::max(std::abs(res), resmax);
    }
    ASSERT_NEAR(resmax, 0.0, 1.e-7);

    // Let's try writing and reading the waveform
#ifdef TEMBLOR_USE_FILESYSTEM
    fs::path scratchFilePath = fs::temp_directory_path();
    std::string scratchFile = std::string(scratchFilePath.c_str())
                            + "/temp.sac";
#else
    std::string scratchFile = "temp.sac";
#endif
    waveform.write(scratchFile);
    SAC::Waveform waveformRead;
    waveformRead.read(scratchFile);
    waveform = waveformRead;
    ASSERT_EQ(waveform.getNumberOfSamples(), 100);    
    ASSERT_NEAR(waveform.getSamplingPeriod(), 0.005, 1.e-7);
    ASSERT_STREQ(waveform.getHeader(SAC::Character::KNETWK).c_str(), "FK");
    ASSERT_STREQ(waveform.getHeader(SAC::Character::KSTNM).c_str(),  "NEW");
    ASSERT_STREQ(waveform.getHeader(SAC::Character::KCMPNM).c_str(), "HHZ");
    ASSERT_STREQ(waveform.getHeader(SAC::Character::KHOLE).c_str(),  "10");
    dPtr = waveform.getDataPointer();
    resmax = 0;
    for (int i=0; i<waveform.getNumberOfSamples(); ++i)
    {   
        double res = dPtr[i] - static_cast<double> (i + 1); 
        resmax = std::max(std::abs(res), resmax);
    }   
    ASSERT_NEAR(resmax, 0.0, 1.e-7);

    dVec = waveform.getData();
    resmax = 0;
    for (int i=0; i<waveform.getNumberOfSamples(); ++i)
    {
        double res = dVec[i] - static_cast<double> (i + 1); 
        resmax = std::max(std::abs(res), resmax);
    }
    ASSERT_NEAR(resmax, 0.0, 1.e-7);
}

}
