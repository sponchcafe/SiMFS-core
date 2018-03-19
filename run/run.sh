#! /bin/bash

LOGFILE=$1.log
TMP=./tmp

export SIM_PARAMS=@$1.json

DIF=../build/src/components/dif/sim_dif
EXI=../build/src/components/exi/sim_exi
PH2=../build/src/components/ph2/sim_ph2
DET=../build/src/components/det/sim_det

cd $TMP; mkfifo c_exi c_det exi emi; cd ..

# DRY RUN
$DIF --iscope "/dif" --oscope "/dif/to_exi"                                                     --output $TMP/c_exi     --config $LOGFILE
$DIF --iscope "/dif" --oscope "/dif/to_det"                                                     --output $TMP/c_det     --config $LOGFILE 
$EXI --scope "/exi"                             --input $TMP/c_exi                              --output $TMP/exi       --config $LOGFILE 
$PH2 --scope "/ph2"                                                                                                     --config $LOGFILE 
$DET --scope "/det"                             --coordinates $TMP/c_det --photons $TMP/emi     --output $1.dat         --config $LOGFILE

# SIMULATION
$DIF --scope "/dif"                                                                             --output $TMP/c_exi     &
$DIF --scope "/dif"                                                                             --output $TMP/c_det     &
$EXI --scope "/exi"                             --input $TMP/c_exi                              --output $TMP/exi       &
$PH2 --scope "/ph2"                                                                                                     &
$DET --scope "/det"                             --coordinates $TMP/c_det --photons $TMP/emi     --output $1.dat

rm $TMP/*
