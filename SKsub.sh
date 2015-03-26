#!/bin/bash

KEEPTAR=$1
JOBDIR=batch
STORE=root://cmseos.fnal.gov//store/user/pedrok/LQ2012

# grid proxy check
voms-proxy-info --all > /dev/null
if [ $? -ne 0 ]; then
  voms-proxy-init -voms cms --valid 168:00
fi

# tarball of CMSSW area
if [ -z "$KEEPTAR" ]; then
  mkdir -p ${JOBDIR}
  tar --exclude-caches -zcf ${JOBDIR}/${CMSSW_VERSION}.tar.gz -C ${CMSSW_BASE}/.. ${CMSSW_VERSION}
fi

./SKtemp.sh ${JOBDIR} input/input_selection.txt LQ500_tb main,zjets,anti-iso,main_TauESUp ${STORE}/ntuple tree_test ${STORE}

