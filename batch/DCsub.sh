#!/bin/bash

source exportProd.sh

KEEPTAR=$1
JOBDIR=jobs
INDIR=root://cmseos.fnal.gov//store/user/pedrok/SUSY2015/Analysis/Skims/${RUN2PRODV}/scan
STORE=root://cmseos.fnal.gov//store/user/pedrok/SUSY2015/Analysis/Datacards/${RUN2PRODV}

./SKcheck.sh "$KEEPTAR" "$JOBDIR"

nparts=`ls -1 ../input/fast/input_sets_DC_fast_*.txt | wc -l`
nparts=$(($nparts-1))

echo "Submitting parts 0.."$nparts

for PART in $(seq 0 $nparts); do
  ./DCtemp.sh ${JOBDIR} ${INDIR} ${PART} ${STORE}
done

