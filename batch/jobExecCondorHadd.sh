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
SEARCH=$4
UPDATE=$5
SKIPTREE=$6
SUFF=$7

echo ""
echo "parameter set:"
echo "CMSSWVER:   $CMSSWVER"
echo "INPUT:      $INPUT"
echo "DIR:        $DIR"
echo "SEARCH:     $SEARCH"
echo "UPDATE:     $UPDATE"
echo "SKIPTREE:   $SKIPTREE"
echo "SUFF:       $SUFF"

tar -xzf ${CMSSWVER}.tar.gz
cd ${CMSSWVER}
source /cvmfs/cms.cern.ch/cmsset_default.sh
scram b ProjectRename
# cmsenv
eval `scramv1 runtime -sh`
cd src/Analysis/batch

#run code
ARGS=""
if [[ $UPDATE -eq 1 ]]; then
	ARGS="-u"
fi
if [[ $SKIPTREE -eq 1 ]]; then
	ARGS="$ARGS -t"
fi
./haddEOS.sh -d ${DIR} -i ${INPUT} -s "${SUFF}" -g "${SEARCH}" -r ${ARGS}

#check exit code
HADDEXIT=$?
if [[ $HADDEXIT -ne 0 ]]; then
	echo "exit code $HADDEXIT, job failed"
	exit $HADDEXIT
fi
