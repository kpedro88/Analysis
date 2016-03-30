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
XRDIR=root://cmseos.fnal.gov/$DIR
#convert input into array
IFS=',' read -r -a SAMPLES <<< "$INPUT"; unset IFS
for BASE in ${SAMPLES[@]}; do
  echo $BASE
  
  #check to see if anything needs to be hadded
  IFS=$'\n' LGFILES=($(xrdfs root://cmseos.fnal.gov ls ${DIR} | grep "${BASE}_block\|${BASE}_ext")); unset IFS
  LSTEST=$?
  if [[ $LSTEST -ne 0 ]]; then
    echo "nothing to hadd for $BASE"
    continue
  fi
  
  ALLFILES=""
  for FILE in ${LGFILES[@]}; do
    ALLFILES="${ALLFILES} ${XRDIR}/`basename ${FILE}`"
  done
  
  #check to see if base file should be included
  LSBASEFILE=$(xrdfs root://cmseos.fnal.gov ls ${DIR} | grep "${BASE}.root")
  LSTEST=$?
  if [[ $LSTEST -eq 0 ]]; then
    ALLFILES="${XRDIR}/${BASE}.root ${ALLFILES}"
  fi
  
  #hadd to tmp file
  TMPFILE=${BASE}.root
  if [[ -n "$SUFF" ]]; then
    TMPFILE=${BASE}_${SUFF}.root
  fi
  hadd ${TMPFILE} ${ALLFILES}
  
  #check exit code
  HADDEXIT=$?
  if [[ $HADDEXIT -ne 0 ]]; then
    rm ${TMPFILE}
    echo "exit code $HADDEXIT, skipping xrdcp"
    exit $HADDEXIT
  fi
  
  #copy hadded file to eos
  xrdcp -f ${TMPFILE} ${XRDIR}/

  #check exit code
  XRDEXIT=$?
  if [[ $XRDEXIT -ne 0 ]]; then
    rm ${TMPFILE}
    echo "exit code $XRDEXIT, failure in xrdcp"
    exit $XRDEXIT
  fi  
  
  #remove original files (only if hadd and xrdcp succeeded)
  for FILE in ${LGFILES[@]}; do
    xrdfs root://cmseos.fnal.gov/ rm ${FILE}
  done
  
  #remove tmp file
  rm ${TMPFILE}
done
