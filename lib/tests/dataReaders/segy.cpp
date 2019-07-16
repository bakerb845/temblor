#include <cstdio>
#include <cstdlib>
#include <climits>
#include "temblor/private/filesystem.hpp"
#include "temblor/dataReaders/segy/segy2.hpp"
#include <gtest/gtest.h>

namespace
{

using namespace Temblor::DataReaders;

TEST(LibraryDataReadersSEGY, segy2)
{
    SEGY::Segy2 segy2;
    segy2.read("data/small.sgy");
}

}
