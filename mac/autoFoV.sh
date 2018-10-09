#!/bin/sh

for ang in $(seq  $1 1 $2); do

    echo "/PoGO+/SetElevation $ang degree" > autoFoV${ang}deg.mac
    cat ../scripts/Crab.mac >> autoFoV${ang}deg.mac

    echo "Running Geant4 with polarimeter elevation = ${ang} degrees"

    #./PoGOSim autoFoV${ang}deg.mac G4data_Crab_FoV${ang}deg &
    cd ../analysis/
    result=$(./G4dataAnalysis ../build/G4data_Crab_FoV${ang}deg.root | grep 'MF')
    echo ${ang} ${result} >> results/FoV${1}to${2}deg.txt
    cd ../build/

done
exit 0
