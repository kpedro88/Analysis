#!/bin/bash

JOBDIR="$1"
STORE1="$2"
STORE2="$3"
JOBNAME1="$4"
JOBNAME2="$5"
OUTDIR="$6"
NUMSAMP="$7"

JDLNAME=jobExecCondor_${JOBNAME2}.jdl
echo ""
echo ">> `/bin/date` Submitting condor job(s) in $JDLNAME with params : $1 $2 $3 $4 $5 $6 $7"

mkdir -p ${JOBDIR}

cat ./jobExecCondorPM.jdl \
| sed -e s/CMSSWVER/"${CMSSW_VERSION}"/ \
| sed -e s/NUMSAMP/"${NUMSAMP}"/ \
| sed -e s~OUTDIR~"${OUTDIR}"~ \
| sed -e s~STORE1~"${STORE1}"~ \
| sed -e s~STORE2~"${STORE2}"~ \
| sed -e s/JOBNAME1/"${JOBNAME1}"/ \
| sed -e s/JOBNAME2/"${JOBNAME2}"/ \
> ${JOBDIR}/${JDLNAME}

cd ${JOBDIR}
condor_submit ${JDLNAME}
cd -
