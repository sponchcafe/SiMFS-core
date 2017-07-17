#! /bin/bash

export PARAMETERS=$1
PATH=./bin:$PATH

mkfifo ./tmp/coords1 ./tmp/coords2 ./tmp/exi ./tmp/det ./tmp/phot

dif | tee ./tmp/coords1 > ./tmp/coords2 &
exiAlpha < ./tmp/coords1 > ./tmp/exi &
detGauss < ./tmp/coords2 > ./tmp/det &
./bin/ph2 -p default.json -e ./tmp/exi -d ./tmp/det > photons.dat #"./photons_$(date +%F)_$(date +%T | sed 's/:/-/g').dat"
rm ./tmp/*