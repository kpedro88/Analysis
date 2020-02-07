#!/bin/bash

JOBDIR="$1"
STORE="$2"
JOBNAME="$3"
SNAME="$4"
NUMSAMP="$5"
TYPE="$6"
YEAR="$7"

JDLNAME=jobExecCondorDC_${JOBNAME}.jdl

echo ""
echo ">> `/bin/date` Submitting condor job(s) in $JDLNAME with params : $1 $2 $3 $4 $5 $6 $7"

mkdir -p ${JOBDIR}

cat ./jobExecCondorDC.jdl \
| sed -e s/CMSSWVER/"${CMSSW_VERSION}"/ \
| sed -e s~STORE~"${STORE}"~ \
| sed -e s/JOBNAME/"${JOBNAME}"/ \
| sed -e s/SNAME/"${SNAME}"/ \
| sed -e s/NUMSAMP/"${NUMSAMP}"/ \
| sed -e s/TYPE/"${TYPE}"/ \
| sed -e s/YEAR/"${YEAR}"/ \
> ${JOBDIR}/${JDLNAME}

cd ${JOBDIR}
condor_submit ${JDLNAME}
cd -
