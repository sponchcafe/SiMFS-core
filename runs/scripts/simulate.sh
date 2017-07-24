#! /bin/bash

export PARAMETERS=../params/$1.json

DIF=../../bin/dif
EXI=../../bin/exiAlpha
DET=../../bin/detGauss
PH2=../../bin/ph2
COR=../../bin/corr

echo "Logging parameters"
cat <($DIF -c) <($EXI -c) <($DET -c) <($PH2 -c) | sed 's/}{/,/g' > ../logs/$1.log

echo "Creating pipes."
mkfifo ../tmp/coords1 ../tmp/coords2 ../tmp/exi ../tmp/det

echo "Setting up diffusion."
$DIF | tee ../tmp/coords1 > ../tmp/coords2 &

echo "Setting up excitation."
$EXI < ../tmp/coords1 > ../tmp/exi &

echo "Setting up detection."
$DET < ../tmp/coords2 > ../tmp/det &

echo "Setting up photophysics and correlator. Starting run..."
$PH2 -e ../tmp/exi -d ../tmp/det | $COR > ../data/$1.dat

echo "Done. Cleaning up."

rm ../tmp/*