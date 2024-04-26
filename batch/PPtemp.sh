#!/bin/bash

JOBDIR="$1"
STORE="$2"
JOBNAME="$3"
REQMEM=6000

JDLNAME=jobExecCondor_${JOBNAME}.jdl
echo ""
echo ">> `/bin/date` Submitting condor job(s) in $JDLNAME with params : $1 $2 $3 $REQMEM"

mkdir -p ${JOBDIR}

cat ./jobExecCondorPP.jdl \
| sed -e s/CMSSWVER/"${CMSSW_VERSION}"/ \
| sed -e s~STORE~"${STORE}"~ \
| sed -e s/JOBNAME/"${JOBNAME}"/ \
| sed -e s/REQMEM/"${REQMEM}"/ \
> ${JOBDIR}/${JDLNAME}

cd ${JOBDIR}
condor_submit ${JDLNAME}
cd -
