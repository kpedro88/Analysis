#!/bin/bash

SETS=(
SMStt850 \
SMStt500 \
SMStt425 \
SMSqqqq1000 \
SMSqqqq1400 \
SMStttt1200 \
SMStttt1500 \
SMSbbbb1000 \
SMSbbbb1500 \
)

for SET in ${SETS[@]}; do
  root -b -l -q 'KPlotDriverDCsyst.C+(".","input/input_RA2bin_btag_ext.txt","input/input_btag_'${SET}'.txt","","",0,1)'
done
