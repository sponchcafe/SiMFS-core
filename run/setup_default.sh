#! /bin/bash

PARAMFILE=$1

DIF=../build/src/components/dif/sim_dif
EXI=../build/src/components/exi/sim_exi
PH2=../build/src/components/ph2/sim_ph2
DET=../build/src/components/det/sim_det

$DIF --config $1 --scope /dif
$EXI --config $1 --scope /exi
$PH2 --config $1 --scope /ph2
$DET --config $1 --scope /det



