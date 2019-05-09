# Installation Instructions

## Prerequisites

Prior to building the software the following dependencies must be met

  1. [CMake](https://cmake.org/) 3.6 or greater.
  2. A C++17 compliant C++ compiler.
  3. [Qt](https://www.qt.io/download) v5.13.  Other 5-series Qt frameworks may work.  This will require that you register with Qt.
  4. [GoogleTest](https://github.com/google/googletest) >1.8.  This is for testing many of the classes.  While I am aware of of Qt's unit testing framework the idea here is that, should I cut bait, then I still have something useful. 
 
## Configure CMake

After satisfying the dependencies, it is possible to configure CMake.  I find the most straightforward approach is to configure CMake with a script.  For example, I might run the following in the root source directory to create a clang\_build directory

    #!/bin/bash
    export CXX=/usr/bin/clang++
    export BUILD_DIR=clang_build
    # Possibly simplest way to specify cmake location
    #export Qt5_DIR=/Users/bbaker/Qt/5.12.3/clang_64/lib/cmake
    # You may want to export CMAKE_PREFIX_PATH instead of specifying the 
    # requisite libraries below.  Or, even better, do this in your bashrc.
    #export GTEST_DIR=/local/lib/cmake/GTest
    #export Qt_CMAKE_DIR=/Users/bbaker/Qt/5.12.3/clang_64/lib/cmake
    #CMAKE_PREFIX_PATH=${CMAKE_PREFIX_PATH}:${Qt_CMAKE_DIR}:${GTEST_CMAKE_DIR}
    # CMake has a lot of leftovers - so just nuke the old directory
    if [ -d ${BUILD_DIR} ]; then
       rm -r ${BUILD_DIR}
    fi
    mkdir ${BUILD_DIR}
    cd ${BUILD_DIR}

    cmake ../ \
    -DCMAKE_BUILD_TYPE=Release \
    -DCMAKE_CXX_COMPILER=${CXX} \
    -DCMAKE_CXX_FLAGS="-g -Wall" \
    -DGTEST_INCLUDE_DIRS=/local/include \
    -DGTEST_LIBRARY=/local/lib/libgtest.a \
    -DGTEST_MAIN_LIBRARY=/local/lib/libgtest_main.a
    cd ..

## Build the Software

After CMake has been successfully configured for this project, descend into the build directory, say clang\_build and make, i.e.,

    cd clang_build
    make

## Test the Software

With a successful build it is possible to test aspects of the software.  To do this, either do

    make test

or 

    ctest

## Install the Software

Not done

