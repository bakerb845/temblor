cmake_minimum_required(VERSION 3.2)
project(temblor VERSION 0.1.1 LANGUAGES CXX)
enable_testing()

##########################################################################################
#                    Define the compiler standard and requisites                         #
##########################################################################################
include(CheckCXXCompilerFlag)

include(FindOpenMP)
if (OPENMP_FOUND)
   message("OpenMP found")
   string(APPEND CMAKE_CXX_FLAGS " ${OpenMP_CXX_FLAGS}")
endif()

#set(CMAKE_CXX_STANDARD 17) 
#set(CMAKE_CXX_STANDARD_REQUIRED ON) 
#set(CMAKE_CXX_EXTENSIONS OFF)

find_package(GeographicLib)

#find_package(Qt5 COMPONENTS Core Widgets REQUIRED)

# TODO - move to ui and scope there.  This generates useless stuff for the core library.
# Run MOC when required
#set(CMAKE_AUTOMOC ON)
#set(CMAKE_AUTOUIC ON)
#set(CMAKE_AUTORCC ON)


find_package(GTest REQUIRED)

#include(CheckIncludeFileCXX)
#check_include_file_cxx(filesystem TEMBLOR_USE_FS)
#if (TEMBLOR_USE_FS)
#   message("Using standard filesystem")
#   add_definitions(-DTEMBLOR_USE_FILESYSTEM)
#else()
#   check_include_file_cxx(experimental/filesystem TEMBLOR_USE_EXPFS)
#   if (TEMBLOR_USE_EXPFS)
#      message("Using experimental filesystem")
#   else()
#      message(STATUS "Compiler does not support filesystem") 
#   endif()
#endif()

# In case i switch to gtkmm
find_package(PkgConfig REQUIRED)
pkg_check_modules(GTKMM REQUIRED gtkmm-3.0)
find_package(OpenGL REQUIRED)

include_directories(
  ${CMAKE_CURRENT_SOURCE_DIR}/include
  ${GTKMM_INCLUDE_DIRS}
)

include(CheckIncludeFileCXX)
check_include_file_cxx(filesystem TEMBLOR_HAVE_STD_FS)
#link_libraries(stdc++fs ${GeographicLib_LIBRARIES})
if (TEMBLOR_HAVE_STD_FS)
   MESSAGE("Using filesystem")
   link_libraries(stdc++fs)
else()
   check_include_file_cxx(experimental/filesystem TEMBLOR_HAVE_EXP_FS)
   if (TEMBLOR_HAVE_EXP_FS)
      MESSAGE("Using experimental/filesystem")
      link_libraries(stdc++fs)
   else()
       check_include_file_cxx(boost/filesystem.hpp TEMBLOR_HAVE_BOOST_FS)
       set(Boost_USE_STATIC_LIBS ON)
       find_package(Boost COMPONENTS filesystem REQUIRED)
       if (TEMBLOR_HAVE_BOOST_FS)
          MESSAGE("Using boost filesystem")
          link_libraries(${Boost_FILESYSTEM_LIBRARY})
       endif()
   endif()
endif()
link_directories(${GTKMM_LIBRARY_DIRS})
#link_libraries(stdc++fs stdc++) #${GeographicLib_LIBRARIES})
#########################################################################################
#                      Define the source, libraries, and executables                    #
#########################################################################################

set(LIBSRC
    lib/dataReaders/sac/waveform.cpp
    lib/dataReaders/sac/header.cpp
    lib/dataReaders/segy/binaryFileHeader.cpp
    lib/dataReaders/segy/segy2.cpp
    lib/models/timeSeriesData/singleChannelWaveform.cpp
    lib/utilities/geodetic/globalPosition.cpp
    lib/utilities/geodetic/globalPositionPair.cpp
    lib/utilities/time.cpp)

#add_executable(helloworld main.cpp dialog.cpp) # mainwindow.cpp mainwindow.ui resources.qrc)
add_executable(gltest
               ui/applications/gltest.cpp
               ui/applications/glarea.cpp
               ui/widgets/firDesigner.cpp
               )
set_property(TARGET gltest PROPERTY CXX_STANDARD 17)

add_library(temblor SHARED ${LIBSRC})
target_include_directories(temblor PRIVATE ${GeographicLib_INCLUDE_DIRS})
target_link_libraries(temblor PRIVATE ${GeographicLib_LIBRARIES})
set_property(TARGET temblor PROPERTY CXX_STANDARD 17)

# Add the Qt5 Widgets for linking
#target_link_libraries(helloworld Qt5::Widgets)
target_include_directories(gltest PUBLIC ${GTKMM_INCLUDE_DIRS} ${GL_INCLUDE_DIR})
target_link_libraries(gltest temblor ${GTKMM_LIBRARIES} ${GL_LIBRARY} ${EPOXY_LIBRARY})

##########################################################################################
#                                      Unit Tests                                        #
##########################################################################################
add_executable(testLibraryUtilities
               lib/tests/utilities/main.cpp 
               lib/tests/utilities/time.cpp
               lib/tests/utilities/location.cpp)
#set_property(TARGET testLibraryUtilities PROPERTY CXX_STANDARD 17)
target_link_libraries(testLibraryUtilities PRIVATE temblor ${GTEST_BOTH_LIBRARIES})
target_include_directories(testLibraryUtilities PRIVATE ${GTEST_INCLUDE_DIRS})
add_test(NAME testLibraryUtilities
         COMMAND testLibraryUtilities)

add_executable(testLibraryDataReaders
               lib/tests/dataReaders/main.cpp
               lib/tests/dataReaders/sac.cpp
               lib/tests/dataReaders/segy.cpp
               )
set_property(TARGET testLibraryDataReaders PROPERTY CXX_STANDARD 17)
target_link_libraries(testLibraryDataReaders PRIVATE temblor ${GTEST_BOTH_LIBRARIES})
target_include_directories(testLibraryDataReaders PRIVATE ${GTEST_INCLUDE_DIRS})
add_test(NAME testLibraryDataReaders
         COMMAND testLibraryDataReaders)

# Also need to copy some test data
file(COPY ${CMAKE_SOURCE_DIR}/lib/tests/data DESTINATION .)
          

##########################################################################################
#                                     Installation                                       #
##########################################################################################

include(GNUInstallDirs)
install(TARGETS temblor
        RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
        LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
        ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR})
