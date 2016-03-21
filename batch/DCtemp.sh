#!/bin/bash

JOBDIR=$1
INDIR=$2
SYSTS=$3
CONTAMS=$4
PART=$5
STORE=$6
SUFF=$7
echo ""
echo ">> `/bin/date` Submitting condor job(s) : $1 $2 $3 $4 $5 $7"

mkdir -p ${JOBDIR}

cat ./jobExecCondorDC.jdl \
| sed -e s/CMSSWVER/${CMSSW_VERSION}/ \
| sed -e s~INDIR~${INDIR}~ \
| sed -e s/SYSTS/${SYSTS}/ \
| sed -e s/CONTAMS/${CONTAMS}/ \
| sed -e s/PART/${PART}/ \
| sed -e s~STORE~${STORE}~ \
| sed -e s/SUFF/${SUFF}/ \
> ${JOBDIR}/jobExecCondor_MakeAllDCsyst_${SYSTS}_${CONTAMS}_${SUFF}${PART}.jdl

cd ${JOBDIR}
condor_submit jobExecCondor_MakeAllDCsyst_${SYSTS}_${CONTAMS}_${SUFF}${PART}.jdl
cd -
