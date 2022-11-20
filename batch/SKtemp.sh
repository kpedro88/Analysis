#!/bin/bash

JOBDIR="$1"
STORE="$2"
JOBNAME="$3"
OUTDIR="$4"
NUMSAMP="$5"
PMSSM="$6"

JDLNAME=jobExecCondor_${JOBNAME}.jdl
echo ""
echo ">> `/bin/date` Submitting condor job(s) in $JDLNAME with params : $1 $2 $3 $4 $5 $6"

mkdir -p ${JOBDIR}

cat ./jobExecCondor.jdl \
| sed -e s/CMSSWVER/"${CMSSW_VERSION}"/ \
| sed -e s/NUMSAMP/"${NUMSAMP}"/ \
| sed -e s~OUTDIR~"${OUTDIR}"~ \
| sed -e s~STORE~"${STORE}"~ \
| sed -e s/JOBNAME/"${JOBNAME}"/ \
| sed -e s/PMSSM/"${PMSSM}"/ \
> ${JOBDIR}/${JDLNAME}

cd ${JOBDIR}
condor_submit ${JDLNAME}
cd -
