#ifndef TEMBLOR_LIBRARY_PRIVATE_HPP
#define TEMBLOR_LIBRARY_PRIVATE_HPP 1

#if __has_include(<filesystem>)
 #include <filesystem>
 namespace fs = std::filesystem;
 #define TEMBLOR_USE_FILESYSTEM 1
#elif __has_include(<experimental/filesystem>)
 #include <experimental/filesystem>
 namespace fs = std::experimental::filesystem;
 #define TEMBLOR_USE_FILESYSTEM 1
#elif __has_include(<boost/filesystem.hpp>)
 #include <boost/filesystem.hpp>
 #include <boost/filesystem/path.hpp>
 namespace fs = boost::filesystem;
 #define TEMBLOR_USE_FILESYSTEM 1
#endif

#endif
