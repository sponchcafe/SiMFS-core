#! /bin/bash

export PARAMETERS=$1

EXEC=../bin
TMP=./tmp

DIF=$EXEC/dif.exe
EXI=$EXEC/exiAlpha.exe
DET=$EXEC/detGauss.exe
PH2=$EXEC/ph2.exe
SUM=$EXEC/sum.exe
BKG=$EXEC/bkg.exe
MIX=$EXEC/mix.exe

for i in `seq 1 3`; do
(
    echo "Creating pipes."
    mkfifo $TMP/coords1_$i $TMP/coords2_$i $TMP/exi_$i $TMP/det_$i $TMP/photons_$i
    touch $TMP/PIDs
    
    echo "Setting up diffusion."
    $DIF -s $i | tee $TMP/coords1_$i > $TMP/coords2_$i &
    echo $! >> $TMP/PIDS

    echo "Setting up excitation."
    $EXI < $TMP/coords1_$i > $TMP/exi_$i &
    echo $! >> $TMP/PIDS

    echo "Setting up detection."
    $DET < $TMP/coords2_$i > $TMP/det_$i &
    echo $! >> $TMP/PIDS

    echo "Setting up photophysics."
    $PH2 -s $i -e $TMP/exi_$i -d $TMP/det_$i > $TMP/photons_$i &
    echo $! >> $TMP/PIDS

)
done

mkfifo $TMP/background $TMP/trace $TMP/binned

echo "Setting up background."
$BKG > $TMP/background &
echo $! >> $TMP/PIDS

echo "Mixing."
$MIX $TMP/photons_* $TMP/background | tee $2 | dd > $TMP/trace &
echo $! >> $TMP/PIDS

echo "Binning."
$SUM < $TMP/trace | tee $3 > $TMP/binned &
echo $! >> $TMP/PIDS

python trace_view.py $TMP/binned

echo "Done. Cleaning up."
rm $TMP/*