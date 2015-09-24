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
SAMPLE=$3
INDIR=$4
STORE=$5

echo ""
echo "parameter set:"
echo "CMSSWVER:   $CMSSWVER"
echo "INPUT:      $INPUT"
echo "SAMPLE:     $SAMPLE"
echo "INDIR:      $INDIR"
echo "STORE:      $STORE"

tar -xzf ${CMSSWVER}.tar.gz
cd ${CMSSWVER}
scram b ProjectRename
source /cvmfs/cms.cern.ch/cmsset_default.sh
# cmsenv
eval `scramv1 runtime -sh`
cd src/Analysis

#run macro
echo "run: root -b -q -l 'KScanDriver.C+("$INPUT","$SAMPLE","$INDIR",)' 2>&1"
root -b -q -l 'KScanDriver.C+("'$INPUT'","'$SAMPLE'","'$INDIR'")' 2>&1

ROOTEXIT=$?

if [[ $ROOTEXIT -ne 0 ]]; then
  rm *.root
  echo "exit code $ROOTEXIT, skipping xrdcp"
  exit $ROOTEXIT
else
  # copy output to eos
  echo "xrdcp output for condor"
  for FILE in *.root
    do
      echo "xrdcp -f ${FILE} ${STORE}/${FILE}"
      xrdcp -f ${FILE} ${STORE}/${FILE}
      rm ${FILE}
    done
fi


