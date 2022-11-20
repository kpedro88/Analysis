#!/bin/bash

#
# variables from arguments string in jdl
#

echo "Starting job on " `date` #Only to display the starting of production date
echo "Running on " `uname -a` #Only to display the machine where the job is running
echo "System release " `cat /etc/redhat-release` #And the system release
echo "CMSSW on Condor"

CMSSWVER=$1
OUTDIR="$2"
STORE="$3"
JOBNAME="$4"
PROCESS="$5"
PMSSM="$6"

echo ""
echo "parameter set:"
echo "CMSSWVER:   $CMSSWVER"
echo "OUTDIR:     $OUTDIR"
echo "STORE:      $STORE"
echo "JOBNAME:    $JOBNAME"
echo "PROCESS:    $PROCESS"
echo "PMSSM:      $PMSSM"

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

# hadd in common file
if [ $PMSSM -eq 1 ]; then
  for TREEDIR in ${OUTDIR}*/; do
    for FILE in ${TREEDIR}/*.root; do
      BASEFILE=$(basename $FILE | sed 's/_block[0-9]*//')
      mv $FILE ${FILE}.bak
      hadd $FILE ${FILE}.bak ${STORE}/${OUTDIR}_common/${BASEFILE}
    done
  done
fi

# list output
ls ${OUTDIR}*/*.root

# copy output to eos
echo "xrdcp output for condor"
for TREEDIR in ${OUTDIR}*/; do
  for FILE in ${TREEDIR}/*.root; do
    # FILE contains TREEDIR in path
    stageOut -x "-f" -i ${FILE} -o ${STORE}/${FILE}
    XRDEXIT=$?
    if [[ $XRDEXIT -ne 0 ]]; then
      rm -r ${OUTDIR}*
      echo "exit code $XRDEXIT, failure in xrdcp"
      exit $XRDEXIT
    fi
  done
  rm -r ${TREEDIR}
done



