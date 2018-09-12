#!/bin/bash

JOBDIR="$1"
INDIR="$2"
STORE="$3"
INPUTS="$4"
OUTPUT="$5"
EXTRA="$6"
JOBNAME="$7"
echo ""
echo ">> `/bin/date` Submitting condor job(s) : $1 $2 $3 $4 $5 $6"

mkdir -p ${JOBDIR}

# repeat quotes to escape (condor "new syntax")
INPUTS=$(echo "$INPUTS" | sed 's/"/""/g')
OUTPUT=$(echo "$OUTPUT" | sed 's/"/""/g')
EXTRA=$(echo "$EXTRA" | sed 's/"/""/g')

cat ./jobExecCondorHist.jdl \
| sed -e s/CMSSWVER/${CMSSW_VERSION}/ \
| sed -e s~INDIR~${INDIR}~ \
| sed -e s~STORE~${STORE}~ \
| sed -e s~INPUTS~${INPUTS}~ \
| sed -e s~OUTPUT~${OUTPUT}~ \
| sed -e s~EXTRA~${EXTRA}~ \
| sed -e s/JOBNAME/${JOBNAME}/ \
> ${JOBDIR}/jobExecCondorHist_${JOBNAME}.jdl

cd ${JOBDIR}
condor_submit jobExecCondorHist_${JOBNAME}.jdl
cd -
