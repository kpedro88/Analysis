#!/bin/bash

JOBDIR="$1"
INDIR="$2"
STORE="$3"
JOBNAME="$4"
NUMSAMP="$5"

JDLNAME=jobExecCondorHist_${JOBNAME}.jdl
echo ""
echo ">> `/bin/date` Submitting condor job(s) in $JDLNAME with params : $1 $2 $3 $4 $5"

mkdir -p ${JOBDIR}

cat ./jobExecCondorHist.jdl \
| sed -e s/CMSSWVER/"${CMSSW_VERSION}"/ \
| sed -e s~INDIR~"${INDIR}"~ \
| sed -e s~STORE~"${STORE}"~ \
| sed -e s/JOBNAME/"${JOBNAME}"/ \
| sed -e s/NUMSAMP/"${NUMSAMP}"/ \
> ${JOBDIR}/${JDLNAME}

cd ${JOBDIR}
condor_submit ${JDLNAME}
cd -
