#include <cstdio>
#include <cstdlib>
#include <climits>
#include "temblor/library/dataReaders/segy/segy2.hpp"
#include <gtest/gtest.h>
#if __has_include(<filesystem>)
 #include <filesystem>
 #define TEMBLOR_USE_FILESYSTEM 1
 namespace fs = std::filesystem;
#elif __has_include(<experimental/filesystem>)
 #include <experimental/filesystem>
 #define TEMBLOR_USE_FILESYSTEM 1
 namespace fs = std::experimental::filesystem;
#elif __has_include(<boost/filesystem.hpp>)
 #include <boost/filesystem.hpp>
 #define TEMBLOR_USE_FILESYSTEM 1
 namespace fs = boost::filesystem;
#endif


namespace
{

using namespace Temblor::Library::DataReaders;

TEST(LibraryDataReadersSEGY, segy2)
{
    SEGY::Segy2 segy2;
    segy2.read("data/small.sgy");
}

}
