#!/bin/bash

JOBDIR="$1"
INDIR="$2"
STORE="$3"
JOBNAME="$4"
echo ""
echo ">> `/bin/date` Submitting condor job(s) : $1 $2 $3 $4"

mkdir -p ${JOBDIR}

cat ./jobExecCondorHist.jdl \
| sed -e s/CMSSWVER/"${CMSSW_VERSION}"/ \
| sed -e s~INDIR~"${INDIR}"~ \
| sed -e s~STORE~"${STORE}"~ \
| sed -e s/JOBNAME/"${JOBNAME}"/ \
> ${JOBDIR}/jobExecCondorHist_${JOBNAME}.jdl

cd ${JOBDIR}
condor_submit jobExecCondorHist_${JOBNAME}.jdl
cd -
