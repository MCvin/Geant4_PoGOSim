#----------------------------------------------------------------------------
# User settings
#
set(name PoGOSim)
set(useROOT true)

#----------------------------------------------------------------------------
# Setup the project
#
cmake_minimum_required(VERSION 2.6 FATAL_ERROR)
project(${name})

#----------------------------------------------------------------------------
# Find Geant4 package, activating all available UI and Vis drivers by default
# You can set WITH_GEANT4_UIVIS to OFF via the command line or ccmake/cmake-gui
# to build a batch mode only executable
#
option(WITH_GEANT4_UIVIS "Build example with Geant4 UI and Vis drivers" ON)
if(WITH_GEANT4_UIVIS)
  find_package(Geant4 REQUIRED ui_all vis_all)
else()
  find_package(Geant4 REQUIRED)
endif()

#----------------------------------------------------------------------------
# Find ROOT package
#
if(useROOT)
  # You need to tell CMake where to find the ROOT installation. This can be done in a number of ways:
  #   - ROOT built with classic configure/make use the provided $ROOTSYS/etc/cmake/FindROOT.cmake
  #   - ROOT built with CMake. Add in CMAKE_PREFIX_PATH the installation prefix for ROOT
  list(APPEND CMAKE_PREFIX_PATH $ENV{ROOTSYS})
  #---Locate the ROOT package and defines a number of variables (e.g. ROOT_INCLUDE_DIRS)
  find_package(ROOT REQUIRED)
  #include_directories(${ROOT_INCLUDE_DIR})
  include_directories(${ROOT_INCLUDE_DIRS})

  IF(ROOT_INCLUDE_DIRS)
    MESSAGE("Your current ROOT version is ${ROOT_VERSION}")
  ELSE()
    MESSAGE("ROOT not found! Please install Root and make it available in PATH")
  ENDIF()
endif(useROOT)

#----------------------------------------------------------------------------
# Setup Geant4 include directories and compile definitions
# Setup include directory for this project
#
include(${Geant4_USE_FILE})
include_directories(${PROJECT_SOURCE_DIR}/include)

#----------------------------------------------------------------------------
# Locate sources and headers for this project
# NB: headers are included so they will show up in IDEs
#
file(GLOB sources ${PROJECT_SOURCE_DIR}/src/*.cc)
file(GLOB headers ${PROJECT_SOURCE_DIR}/include/*.hh)

#----------------------------------------------------------------------------
# Add the executable, and link it to the Geant4 libraries
#
add_executable(${name} ${name}.cc ${sources} ${headers})
target_link_libraries(${name} ${Geant4_LIBRARIES} ${ROOT_LIBRARIES})

#message(STATUS "${Geant4_LIBRARIES}")
#message(STATUS "${Geant4_USE_FILE}")
