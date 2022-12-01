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
STORE1="$3"
STORE2="$4"
JOBNAME1="$5"
JOBNAME2="$6"
PROCESS="$7"

echo ""
echo "parameter set:"
echo "CMSSWVER:   $CMSSWVER"
echo "OUTDIR:     $OUTDIR"
echo "STORE1:     $STORE1"
echo "STORE2:     $STORE2"
echo "JOBNAME1:   $JOBNAME1"
echo "JOBNAME2:   $JOBNAME2"
echo "PROCESS:    $PROCESS"

source stageOut.sh
tar -xzf ${CMSSWVER}.tar.gz
cd ${CMSSWVER}
source /cvmfs/cms.cern.ch/cmsset_default.sh
scram b ProjectRename
# cmsenv
eval `scramv1 runtime -sh`
cd src/Analysis

# run skim step
MACRO1=$(cat $_CONDOR_SCRATCH_DIR/macro_${JOBNAME1}_part${PROCESS}.txt)
echo "run: root -b -q -l '$MACRO1'"
root -b -q -l "${MACRO1}" 2>&1

ROOTEXIT=$?

if [[ $ROOTEXIT -ne 0 ]]; then
	echo "exit code $ROOTEXIT from $JOBNAME1, skipping xrdcp"
	exit $ROOTEXIT
fi

# hadd in common file
for TREEDIR in ${OUTDIR}*/; do
	for FILE in ${TREEDIR}/*.root; do
		BASEFILE=$(basename $FILE | sed 's/_block[0-9]*//')
		mv $FILE ${FILE}.bak
		hadd $FILE ${FILE}.bak ${STORE1}/${OUTDIR}_common/${BASEFILE}
	done
done

# run DC step
MACRO2=$(cat $_CONDOR_SCRATCH_DIR/macro_${JOBNAME2}_part${PROCESS}.txt)
echo "run: root -b -q -l '$MACRO2'"
root -b -q -l "${MACRO2}" 2>&1

ROOTEXIT=$?

if [[ $ROOTEXIT -ne 0 ]]; then
	rm *.root
	echo "exit code $ROOTEXIT from $JOBNAME2, skipping xrdcp"
	exit $ROOTEXIT
fi

# copy output to eos
echo "xrdcp output for condor"
for FILE in *.root; do
	echo "xrdcp -f ${FILE} ${STORE2}/${FILE}"
	stageOut -x "-f" -i ${FILE} -o ${STORE2}/${FILE}
	XRDEXIT=$?
	if [[ $XRDEXIT -ne 0 ]]; then
		rm *.root
		echo "exit code $XRDEXIT, failure in xrdcp"
		exit $XRDEXIT
	fi
	rm ${FILE}
done
