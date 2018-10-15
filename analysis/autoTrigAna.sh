#!/bin/bash

for trigger in $(seq  $1 10 $2); do

    echo "Analysing with trigger = ${trigger} "

    resultCrab=$(./G4dataAnalysis ~/g4data/G4data_Crab.root ${trigger} | grep 'MF')
    resultGammas=$(./G4dataAnalysis ~/g4data/G4data_Gammas.root ${trigger} | grep 'MF')
    resultNeutrons=$(./G4dataAnalysis ~/g4data/G4data_Neutrons.root ${trigger} | grep 'MF')
    echo ${trigger} ${resultCrab} >> Trigger${1}to${2}_Crab.txt
    echo ${trigger} ${resultGammas} >> Trigger${1}to${2}_Gammas.txt
    echo ${trigger} ${resultNeutrons} >> Trigger${1}to${2}_Neutrons.txt

done
exit 0
