#!/bin/bash

#
# variables from arguments string in jdl
#

echo "Starting job on " `date` #Only to display the starting of production date
echo "Running on " `uname -a` #Only to display the machine where the job is running
echo "System release " `cat /etc/redhat-release` #And the system release
echo "CMSSW on Condor"

CMSSWVER=$1
INDIR=$2
SYSTS=$3
CONTAMS=$4
PART=$5
STORE=$6
SUFF=$7

echo ""
echo "parameter set:"
echo "CMSSWVER:   $CMSSWVER"
echo "INDIR:      $INDIR"
echo "SYSTS:      $SYSTS"
echo "CONTAMS:    $CONTAMS"
echo "PART:       $PART"
echo "STORE:      $STORE"
echo "SUFF:       $SUFF"

tar -xzf ${CMSSWVER}.tar.gz
cd ${CMSSWVER}
scram b ProjectRename
source /cvmfs/cms.cern.ch/cmsset_default.sh
# cmsenv
eval `scramv1 runtime -sh`
cd src/Analysis

#run macro
echo "run: root -b -q -l 'MakeAllDCsyst.C+(1,"'"'$INDIR'","'$SYSTS'","'$CONTAMS'",'$PART',"'$SUFF'"'")' 2>&1"
root -b -q -l 'MakeAllDCsyst.C+(1,"'$INDIR'","'$SYSTS'","'$CONTAMS'",'$PART',"'$SUFF'")' 2>&1

ROOTEXIT=$?

if [[ $ROOTEXIT -ne 0 ]]; then
  rm *.root
  echo "exit code $ROOTEXIT, skipping xrdcp"
  exit $ROOTEXIT
fi

# copy output to eos
echo "xrdcp output for condor"
for FILE in *.root
do
  echo "xrdcp -f ${FILE} ${STORE}/${FILE}"
  xrdcp -f ${FILE} ${STORE}/${FILE}
  XRDEXIT=$?
  if [[ $XRDEXIT -ne 0 ]]; then
    rm *.root
    echo "exit code $XRDEXIT, failure in xrdcp"
    exit $XRDEXIT
  fi
  rm ${FILE}
done



