#!/bin/sh
Geant4_DIR=$(echo /usr/share/geant4/geant4.*-install/lib/Geant4*)
CMAKE_MODULE_PATH=$(echo /usr/share/geant4/geant4.*/cmake/Modules/)

cmake .. -DGeant4_DIR=$Geant4_DIR -DCMAKE_MODULE_PATH=$CMAKE_MODULE_PATH
