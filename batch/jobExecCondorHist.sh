#!/bin/bash

#
# variables from arguments string in jdl
#

echo "Starting job on " `date` #Only to display the starting of production date
echo "Running on " `uname -a` #Only to display the machine where the job is running
echo "System release " `cat /etc/redhat-release` #And the system release
echo "CMSSW on Condor"

CMSSWVER="$1"
INDIR="$2"
STORE="$3"
JOBNAME="$4"
PROCESS="$5"

echo ""
echo "parameter set:"
echo "CMSSWVER:   $CMSSWVER"
echo "INDIR:      $INDIR"
echo "STORE:      $STORE"
echo "JOBNAME:    $JOBNAME"
echo "PROCESS:    $PROCESS"

source stageOut.sh
tar -xzf ${CMSSWVER}.tar.gz
cd ${CMSSWVER}
source /cvmfs/cms.cern.ch/cmsset_default.sh
scram b ProjectRename
# cmsenv
eval `scramv1 runtime -sh`
cd src/Analysis

#run macro
MACRO=$(cat $_CONDOR_SCRATCH_DIR/macro_${JOBNAME}_part${PROCESS}.txt)
echo "run: root -b -q -l '$MACRO'"
root -b -q -l "${MACRO}" 2>&1

ROOTEXIT=$?

if [[ $ROOTEXIT -ne 0 ]]; then
  echo "exit code $ROOTEXIT, skipping xrdcp"
  exit $ROOTEXIT
fi

# list output
ls *.root

# copy output to eos
echo "xrdcp output for condor"
for FILE in *.root
do
  echo "xrdcp -f ${FILE} ${STORE}/${FILE}"
  stageOut -x "-f" -i ${FILE} -o ${STORE}/${FILE}
  XRDEXIT=$?
  if [[ $XRDEXIT -ne 0 ]]; then
    rm *.root
    echo "exit code $XRDEXIT, failure in xrdcp"
    exit $XRDEXIT
  fi
  rm ${FILE}
done
