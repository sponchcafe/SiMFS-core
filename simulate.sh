#! /bin/bash

export PARAMETERS=$1

DIF=./bin/dif
EXI=./bin/exiAlpha
DET=./bin/detGauss
PH2=./bin/ph2

echo "Logging parameters:"
cat <($DIF -c) <($EXI -c) <($DET -c) <($PH2 -c)

mkfifo ./tmp/coords1 ./tmp/coords2 ./tmp/exi ./tmp/det

./bin/dif | tee ./tmp/coords1 > ./tmp/coords2 &
$EXI < ./tmp/coords1 > ./tmp/exi &
$DET < ./tmp/coords2 > ./tmp/det &
$PH2 -e ./tmp/exi -d ./tmp/det | tee photons.dat |./bin/corr > $2

rm ./tmp/*