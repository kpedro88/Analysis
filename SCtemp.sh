#!/bin/bash

JOBDIR=$1
INPUT=$2
SAMPLE=$3
INDIR=$4
STORE=$5
SELTYPE=scan
echo ""
echo ">> `/bin/date` Submitting condor job(s) : $1 $2 $3 $4 $5 $6 $7"

mkdir -p ${JOBDIR}

cat ./jobExecCondorScan.jdl \
| sed -e s/CMSSWVER/${CMSSW_VERSION}/ \
| sed -e s~INPUT~${INPUT}~ \
| sed -e s/SAMPLE/${SAMPLE}/ \
| sed -e s/SELTYPE/${SELTYPE}/ \
| sed -e s~INDIR~${INDIR}~ \
| sed -e s~STORE~${STORE}~ \
> ${JOBDIR}/jobExecCondor_${SAMPLE}_${SELTYPE}.jdl

cd ${JOBDIR}
condor_submit jobExecCondor_${SAMPLE}_${SELTYPE}.jdl
cd -