#!/bin/bash

JOBDIR=$1
INDIR=$2
SYSTS=$3
VARS=$4
STORE=$5
SAMPLE=$6
echo ""
echo ">> `/bin/date` Submitting condor job(s) : $1 $2 $3 $4 $5 $6"

mkdir -p ${JOBDIR}

cat ./jobExecCondorDC.jdl \
| sed -e s/CMSSWVER/${CMSSW_VERSION}/ \
| sed -e s~INDIR~${INDIR}~ \
| sed -e s/SYSTS/${SYSTS}/ \
| sed -e s/VARS/${VARS}/ \
| sed -e s~STORE~${STORE}~ \
| sed -e s/SAMPLE/${SAMPLE}/ \
> ${JOBDIR}/jobExecCondor_MakeAllDCsyst_${SAMPLE}.jdl

cd ${JOBDIR}
condor_submit jobExecCondor_MakeAllDCsyst_${SAMPLE}.jdl
cd -
