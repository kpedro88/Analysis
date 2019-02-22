#!/bin/bash

JOBDIR="$1"
INPUT="$2"
SNAME="$3"
NUMSAMP="$4"
INDIR="$5"
STORE="$6"
JOBTYPE=scan

JDLNAME=jobExecCondor_${SNAME}_${JOBTYPE}.jdl
echo ""
echo ">> `/bin/date` Submitting condor job(s) in $JDLNAME with params : $1 $2 $3 $4 $5 $6"

mkdir -p ${JOBDIR}

cat ./jobExecCondorScan.jdl \
| sed -e s/CMSSWVER/"${CMSSW_VERSION}"/ \
| sed -e s~INPUT~"${INPUT}"~ \
| sed -e s/SNAME/"${SNAME}"/ \
| sed -e s/NUMSAMP/"${NUMSAMP}"/ \
| sed -e s/JOBTYPE/"${JOBTYPE}"/ \
| sed -e s~INDIR~"${INDIR}"~ \
| sed -e s~STORE~"${STORE}"~ \
> ${JOBDIR}/${JDLNAME}

cd ${JOBDIR}
condor_submit ${JDLNAME}
cd -
