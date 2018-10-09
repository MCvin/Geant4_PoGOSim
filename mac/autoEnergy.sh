#!/bin/sh

for energy in $(seq  $1 1 $2); do 

    cat > autoEnergy.mac <<EOF
/random/setSeeds 122 223
/gps/particle gamma
/gps/pos/type Plane
/gps/pos/shape Circle
/gps/pos/centre 0. 0. 250. cm
/gps/pos/radius 15 cm
/gps/direction 0 0 -1
/gps/polarization 1. 0. 0. # 90 deg

/gps/ene/mono $energy keV

/control/loop ../scripts/rotation.mac step 0. 360. 0.1
EOF

    echo "Running Geant4 for energy = ${energy} "

    #./PoGOSim autoEnergy.mac G4dataP${energy}
    cd ../analysis/
    result=$(./G4dataAnalysis ../build/G4dataP${energy}.root | grep 'MF')
    echo ${energy} ${result} >> ${1}to${2}keV.txt
    cd ../build/

done
exit 0
