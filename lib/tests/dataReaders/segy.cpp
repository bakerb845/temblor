#include <cstdio>
#include <cstdlib>
#include <climits>
#include "temblor/library/private/filesystem.hpp"
#include "temblor/library/dataReaders/segy/segy2.hpp"
#include <gtest/gtest.h>

namespace
{

using namespace Temblor::Library::DataReaders;

TEST(LibraryDataReadersSEGY, segy2)
{
    SEGY::Segy2 segy2;
    segy2.read("data/small.sgy");
}

}
