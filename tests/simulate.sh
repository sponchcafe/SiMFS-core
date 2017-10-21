#! /bin/bash

export PARAMETERS=../params/$1.json

DIF=../../bin/dif.exe
EXI=../../bin/exiAlpha.exe
DET=../../bin/detGauss.exe
PH2=../../bin/ph2.exe
COR=../../bin/corr.exe
SUM=../../bin/sum.exe
BKG=../../bin/bkg.exe
MIX=../../bin/mix.exe

#echo "Logging parameters"
#cat <($DIF -c) <($EXI -c) <($DET -c) <($PH2 -c) | sed 's/}{/,/g' > ../logs/$1.log

echo "Creating pipes."
mkfifo ../tmp/coords1 ../tmp/coords2 ../tmp/exi ../tmp/det ../tmp/photons ../tmp/background

echo "Setting up diffusion."
$DIF | tee ../tmp/coords1 > ../tmp/coords2 &

echo "Setting up excitation."
$EXI < ../tmp/coords1 > ../tmp/exi &

echo "Setting up detection."
$DET < ../tmp/coords2 > ../tmp/det &

echo "Setting up photophysics."
$PH2 -e ../tmp/exi -d ../tmp/det | tee emission.dat > ../tmp/photons &

echo "Setting up background"
$BKG | tee background.dat > ../tmp/background &

$MIX ../tmp/photons ../tmp/background > photons.dat





#$SUM -w 1e-4  < ../tmp/photons > ttrace.dat

#echo "Setting up correlator. Starting run..."
#$COR <../tmp/photons > ../data/$1.dat

echo "Done. Cleaning up."
rm ../tmp/*

#../../inspect/view_cor.py ../data/$1.dat