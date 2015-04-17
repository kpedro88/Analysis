#!/bin/bash

KEEPTAR=$1
JOBDIR=batch
STORE=root://cmseos.fnal.gov//store/user/pedrok/LQ2012

./SKcheck.sh "$KEEPTAR" "$JOBDIR"

./SKtemp.sh ${JOBDIR} input/input_selection.txt LQ500_tb main,zjets,anti-iso,main_TauESUp ${STORE}/ntuple tree_test ${STORE}

