#!/bin/bash

JOBDIR=$1
INPUT=$2
DIR=$3
SUFF=$4
TIME=`date +%s`
echo ""
echo ">> `/bin/date` Submitting condor job(s) : $1 $2 $3 $4 $TIME"

mkdir -p ${JOBDIR}

cat ./jobExecCondorHadd.jdl \
| sed -e s/CMSSWVER/${CMSSW_VERSION}/ \
| sed -e s~INPUT~${INPUT}~ \
| sed -e s~DIR~${DIR}~ \
| sed -e s/SUFF/${SUFF}/ \
| sed -e s/TIME/${TIME}/ \
> ${JOBDIR}/jobExecCondor_hadd_${SUFF}_${TIME}.jdl

cd ${JOBDIR}
condor_submit jobExecCondor_hadd_${SUFF}_${TIME}.jdl
cd -