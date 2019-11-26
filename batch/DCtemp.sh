#!/bin/bash

JOBDIR="$1"
INDIR="$2"
SYSTS="$3"
VARS="$4"
STORE="$5"
SNAME="$6"
DCCONFIG="$7"
NUMSAMP="$8"
TYPE="$9"
YEAR="${10}"

JDLNAME=jobExecCondor_MakeAllDCsyst_${SNAME}.jdl

echo ""
echo ">> `/bin/date` Submitting condor job(s) in $JDLNAME with params : $1 $2 $3 $4 $5 $6 $7 $8 $9 ${10}"

mkdir -p ${JOBDIR}

cat ./jobExecCondorDC.jdl \
| sed -e s/CMSSWVER/"${CMSSW_VERSION}"/ \
| sed -e s~INDIR~"${INDIR}"~ \
| sed -e s/SYSTS/"${SYSTS}"/ \
| sed -e s/VARS/"${VARS}"/ \
| sed -e s~STORE~"${STORE}"~ \
| sed -e s/SNAME/"${SNAME}"/ \
| sed -e s~DCCONFIG~"${DCCONFIG}"~ \
| sed -e s/NUMSAMP/"${NUMSAMP}"/ \
| sed -e s/TYPE/"${TYPE}"/ \
| sed -e s/YEAR/"${YEAR}"/ \
> ${JOBDIR}/${JDLNAME}

cd ${JOBDIR}
condor_submit ${JDLNAME}
cd -
