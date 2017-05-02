#!/bin/bash

KEEPTAR=""
JOBDIR=jobs

#check arguments
while getopts "k" opt; do
  case "$opt" in
  k) KEEPTAR="keep"
    ;;
  esac
done

# grid proxy existence & expiration check
if ! voms-proxy-info -exists ; then
  voms-proxy-init -voms cms --valid 168:00
fi

# tarball of CMSSW area
if [ -z "$KEEPTAR" ]; then
  mkdir -p ${JOBDIR}
  tar --exclude-caches-all -zcf ${JOBDIR}/${CMSSW_VERSION}.tar.gz -C ${CMSSW_BASE}/.. ${CMSSW_VERSION}
fi

ls -lth ${JOBDIR}/${CMSSW_VERSION}.tar.gz
