#! /bin/bash
export PARAMETERS=../params/$1.json

DIF=../../bin/dif
EXI=../../bin/exiAlpha
DET=../../bin/detGauss
PH2=../../bin/ph2
MIX=../../bin/mix
COR=../../bin/corr

cat <($DIF -c) <($EXI -c) <($DET -c) <($PH2 -c) | sed 's/}{/,/g' > ../logs/$1.log

for i in `seq 1 20`; do

(

    echo "Initializing molecule $i"

    echo "Creating pipes."
    mkfifo ../tmp/coords1_$i ../tmp/coords2_$i ../tmp/exi_$i ../tmp/det_$i ../tmp/photons_$i

    echo "Setting up diffusion."
    $DIF -s $1 | tee ../tmp/coords1_$i > ../tmp/coords2_$i &

    echo "Setting up excitation."
    $EXI < ../tmp/coords1_$i > ../tmp/exi_$i &

    echo "Setting up detection."
    $DET < ../tmp/coords2_$i > ../tmp/det_$i &

    echo "Setting up photophysics."
    $PH2 -s $1 -e ../tmp/exi_$i -d ../tmp/det_$i > ../tmp/photons_$i &
)
done

echo "Starting run..."

$MIX ../tmp/photons_* | $COR > ../data/$1.cor

echo "Done. Cleaning up."

rm ../tmp/*

../../inspect/view_cor.py ../data/$1.cor
