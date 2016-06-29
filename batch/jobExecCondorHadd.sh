#!/bin/bash

#
# variables from arguments string in jdl
#

echo "Starting job on " `date` #Only to display the starting of production date
echo "Running on " `uname -a` #Only to display the machine where the job is running
echo "System release " `cat /etc/redhat-release` #And the system release
echo "CMSSW on Condor"

CMSSWVER=$1
INPUT=$2
DIR=$3
SUFF=$4

echo ""
echo "parameter set:"
echo "CMSSWVER:   $CMSSWVER"
echo "INPUT:      $INPUT"
echo "DIR:        $DIR"
echo "SUFF:       $SUFF"

tar -xzf ${CMSSWVER}.tar.gz
cd ${CMSSWVER}
scram b ProjectRename
source /cvmfs/cms.cern.ch/cmsset_default.sh
# cmsenv
eval `scramv1 runtime -sh`
cd src/Analysis/batch

#run code
./haddEOS.sh -d ${DIR} -i ${INPUT} -x "${SUFF}" -g "_block" -r
