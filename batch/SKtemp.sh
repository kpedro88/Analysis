#!/bin/bash

JOBDIR="$1"
INPUT="$2"
SNAME="$3"
NUMSAMP="$4"
SELTYPE="$5"
INDIR="$6"
OUTDIR="$7"
STORE="$8"
JOBTYPE="$9"

JDLNAME=jobExecCondor_${SNAME}_${JOBTYPE}.jdl
echo ""
echo ">> `/bin/date` Submitting condor job(s) in $JDLNAME with params : $1 $2 $3 $4 $5 $6 $7 $8 $9"

mkdir -p ${JOBDIR}

cat ./jobExecCondor.jdl \
| sed -e s/CMSSWVER/"${CMSSW_VERSION}"/ \
| sed -e s~INPUT~"${INPUT}"~ \
| sed -e s/SNAME/"${SNAME}"/ \
| sed -e s/NUMSAMP/"${NUMSAMP}"/ \
| sed -e s/SELTYPE/"${SELTYPE}"/ \
| sed -e s~INDIR~"${INDIR}"~ \
| sed -e s~OUTDIR~"${OUTDIR}"~ \
| sed -e s~STORE~"${STORE}"~ \
| sed -e s/JOBTYPE/"${JOBTYPE}"/ \
> ${JOBDIR}/${JDLNAME}

cd ${JOBDIR}
condor_submit ${JDLNAME}
cd -
