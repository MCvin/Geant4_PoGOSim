#!/bin/sh

for length in $(seq  $1 30 $2); do

    echo "/PoGOLite/SetFastLength $length mm" > autoLength${length}mm.mac
    cat ../scripts/Crab.mac >> autoLength${length}mm.mac

    echo "Running Geant4 with fast length = ${length} "

    ./PoGOSim autoLength${length}mm.mac G4dataCrabFast${length}mm &
    #cd ../analysis/
    #result=$(./G4dataAnalysis ../build/G4dataCrabFast${length}mm.root | grep 'MF')
    #echo ${length} ${result} >> ${1}to${2}mm.txt
    #cd ../build/

done
exit 0
