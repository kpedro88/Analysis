#!/bin/bash

JOBDIR="$1"
INDIR="$2"
SYSTS="$3"
VARS="$4"
STORE="$5"
SNAME="$6"
NUMSAMP="$7"

JDLNAME=jobExecCondor_MakeAllDCsyst_${SNAME}.jdl

echo ""
echo ">> `/bin/date` Submitting condor job(s) in $JDLNAME with params : $1 $2 $3 $4 $5 $6"

mkdir -p ${JOBDIR}

cat ./jobExecCondorDC.jdl \
| sed -e s/CMSSWVER/"${CMSSW_VERSION}"/ \
| sed -e s~INDIR~"${INDIR}"~ \
| sed -e s/SYSTS/"${SYSTS}"/ \
| sed -e s/VARS/"${VARS}"/ \
| sed -e s~STORE~"${STORE}"~ \
| sed -e s/SNAME/"${SNAME}"/ \
| sed -e s/NUMSAMP/"${NUMSAMP}"/ \
> ${JOBDIR}/${JDLNAME}

cd ${JOBDIR}
condor_submit ${JDLNAME}
cd -
