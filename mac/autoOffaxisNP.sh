#!/bin/sh

for ang in $(seq  $1 1 $2); do 
dx=$(echo "s($ang*3.1415/180)" | bc -l)
dz=$(echo "-c($ang*3.1415/180)" | bc -l)
px=$(echo "-($dx*85.0)" | bc -l)
pz=$(echo "-($dz*85.0)" | bc -l)
rx=$(echo "-($dz)" | bc -l)
rz=$(echo "$dx" | bc -l)

    cat > autoOffaxis.mac <<EOF
/random/setSeeds 122 223
/gps/particle gamma
/gps/pos/type Plane
/gps/pos/shape Circle
/gps/ang/type iso
/gps/pos/centre 0. 0. 85. cm
/gps/pos/radius 3.75 mm
/gps/ang/maxtheta 0.68 deg
#/gps/direction $dx 0. $dz
/gps/pos/rot2 0. $rx $rz
#/gps/polarization 0. 1. 0.

/gps/ene/mono 59.5 keV

/run/beamOn 1000000
EOF

    echo "Running Geant4 for Offaxis = $ang degrees"

    ./PoGOSim autoOffaxis.mac G4dataNP$ang
    mv G4dataNP$ang.root ../analysis/
    #cd ../analysis/
    #root -b -q G4dataAnalysis.C | grep 'MF' >> 0to90deg.txt
    #cd ../build/

done
exit 0
