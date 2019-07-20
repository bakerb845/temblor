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
find_package(Freetype REQUIRED)

include_directories(
  ${CMAKE_CURRENT_SOURCE_DIR}/include
  ${MSEED_INCLUDE_DIR}
  ${GTKMM_INCLUDE_DIRS}
)
if (${USE_SQLITE3})
   #include_directories(${SQLite3_INCLUDE_DIRS})
   link_libraries(${SQLite3_LIBRARIES})
endif()

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
    lib/dataReaders/miniseed/sncl.cpp
    lib/dataReaders/miniseed/trace.cpp
    lib/models/event/origin.cpp
    lib/models/timeSeriesData/singleChannelWaveform.cpp
    lib/models/timeSeriesData/waveformIdentifier.cpp
    lib/solvers/rayTrace1D/isotropicLayer.cpp
    lib/solvers/rayTrace1D/layerCakeModel.cpp
    lib/utilities/geodetic/globalPosition.cpp
    lib/utilities/geodetic/globalPositionPair.cpp
    lib/utilities/time.cpp)

#set(DBSRC
#    database/tables/event.cpp
#    database/tables/eventType.cpp
#    database/tables/eventTypeTuple.cpp
#    )

#add_executable(helloworld main.cpp dialog.cpp) # mainwindow.cpp mainwindow.ui resources.qrc)
set(UILIB_SRC
    ui/models/rgba.cpp
    ui/models/waveformGather.cpp
    ui/models/plotTransformations.cpp)
add_executable(gltest
               ui/applications/glWiggle.cpp
               ui/applications/glslShader.cpp
               ui/applications/gltest.cpp
               ui/applications/glarea.cpp
               ui/widgets/firDesignerWindow.cpp
               ui/widgets/firDesignerModel.cpp
               ui/widgets/waveformPlotOptionsModel.cpp
               )
set_property(TARGET gltest PROPERTY CXX_STANDARD 17)

add_library(temblor SHARED ${LIBSRC}) # ${DBSRC})
target_include_directories(temblor PRIVATE ${GeographicLib_INCLUDE_DIRS})
target_link_libraries(temblor PRIVATE ${GeographicLib_LIBRARIES} ${MSEED_LIBRARY})
if (${USE_SQLITE3})
   target_include_directories(temblor PRIVATE ${SQLITE_ORM_INCLUDE_DIR})
endif()
set_property(TARGET temblor PROPERTY CXX_STANDARD 17)

add_library(temblorUI SHARED ${UILIB_SRC})
target_link_libraries(temblorUI PRIVATE ${temblor} ${GTKMM_LIBRARIES} ${GL_LIBRAY} ${EPOXY_LIBRARY} ${FREETYPE_LIBRARIES})

target_include_directories(gltest PUBLIC ${GTKMM_INCLUDE_DIRS} ${GL_INCLUDE_DIR})
target_link_libraries(gltest temblorUI temblor ${GTKMM_LIBRARIES} ${GL_LIBRARY} ${EPOXY_LIBRARY} ${FREETYPE_LIBRARIES})

##########################################################################################
#                                 Copy Shader Files                                      #
##########################################################################################
#add_custom_target(copy_shader_files
                  #${CMAKE_COMMAND}
                  #-D SOURCE_DIR=${CMAKE_CURRENT_SOURCE_DIR}/
                  #-D DESTINATION_DIR=${CMAKE_CURRENT_BINARY_DIR}
                  #-P CopyFile.cmake
                  #COMMENT "Copying files for target ")
#add_dependencies(gltest copy_shader_files)
file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/ui/shaders DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/ui/fonts   DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
##########################################################################################
#                                      Unit Tests                                        #
##########################################################################################
#add_executable(testDatabase
#               database/testing/main.cpp
#               database/testing/eventType.cpp)
#target_link_libraries(testDatabase PRIVATE temblor ${GTEST_BOTH_LIBRARIES})

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
               lib/tests/dataReaders/miniseed.cpp
               lib/tests/dataReaders/segy.cpp
               )
set_property(TARGET testLibraryDataReaders PROPERTY CXX_STANDARD 17)
target_link_libraries(testLibraryDataReaders PRIVATE temblor ${GTEST_BOTH_LIBRARIES})
target_include_directories(testLibraryDataReaders PRIVATE ${GTEST_INCLUDE_DIRS})
add_test(NAME testLibraryDataReaders
         COMMAND testLibraryDataReaders)

add_executable(testLibrarySolvers
               lib/tests/solvers/main.cpp
               lib/tests/solvers/rayTrace1D.cpp)
set_property(TARGET testLibrarySolvers PROPERTY CXX_STANDARD 17)
target_link_libraries(testLibrarySolvers PRIVATE temblor ${GTEST_BOTH_LIBRARIES})
target_include_directories(testLibrarySolvers PRIVATE ${GTEST_INCLUDE_DIRS})
add_test(NAME testLibrarySolvers
         COMMAND testLibrarySolvers)

add_executable(testUserInterfaceModels
               ui/tests/main.cpp
               ui/tests/rgba.cpp
              )
target_link_libraries(testUserInterfaceModels PRIVATE temblor temblorUI ${GTEST_BOTH_LIBRARIES})
target_include_directories(testUserInterfaceModels PRIVATE ${GTEST_INCLUDE_DIRS})
add_test(NAME testUserInterfaceModels
         COMMAND testUserInterfaceModels)

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
