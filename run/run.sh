#! /bin/bash

LOGFILE=$1.log.json
TMP=./tmp

export SIM_PARAMS=@$1.json

if [ $(uname -s | grep "CYGWIN") ];
then
    SUFFIX=".exe"
else
    SUFFIX=""
fi

DIF=../build/src/components/dif/sim_dif$SUFFIX
EXI=../build/src/components/exi/sim_exi$SUFFIX
PH2=../build/src/components/ph2/sim_ph2$SUFFIX
DET=../build/src/components/det/sim_det$SUFFIX

# Extracting the expected byte payload from diffusion
TIME=$(jq < $1.json '.dif["experiment-time"]')
INCREMENT=$(jq < $1.json '.dif["increment"]')
PAYLOAD=$(awk '{printf "%d\n", ($1/$2*8*4)}' <(echo "$TIME $INCREMENT" | sed 's/\./,/g'))
BYTES_PER_SECOND=$(awk '{printf "%d\n", ($1/$2*64)}' <(echo "1 $INCREMENT" | sed 's/\./,/g'))
BUFFER_SIZE=100M

cd $TMP
mkfifo c_exi
mkfifo c_det_pre 
mkfifo c_det
mkfifo exi 
mkfifo emi
cd ..

# SIMULATION
$DIF --iscope "/dif" --oscope "/dif/to_exi"                                                      --output $TMP/c_exi     --log $LOGFILE    & sleep 0.1 
$DIF --iscope "/dif" --oscope "/dif/to_emi"                                                      --output $TMP/c_det_pre --log $LOGFILE    & sleep 0.1
pv < $TMP/c_det_pre > $TMP/c_det -N "completion" -i 0.25 -feprbcTS -s $PAYLOAD -B $BUFFER_SIZE &
$EXI --scope "/exi"                             --input $TMP/c_exi                              --output $TMP/exi       --log $LOGFILE    & sleep 0.1
$PH2 --scope "/ph2"                                                                                                     --log $LOGFILE    & sleep 0.1
$DET --scope "/det"                             --coordinates $TMP/c_det --photons $TMP/emi     --output $1.dat         --log $LOGFILE 

rm $TMP/*
