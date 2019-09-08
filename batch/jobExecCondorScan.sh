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
SNAME=$3
PROCESS=$4
INDIR=$5
STORE=$6

echo ""
echo "parameter set:"
echo "CMSSWVER:   $CMSSWVER"
echo "INPUT:      $INPUT"
echo "SNAME:      $SNAME"
echo "PROCESS:    $PROCESS"
echo "INDIR:      $INDIR"
echo "STORE:      $STORE"

#get sample
source export${SNAME}.sh
SAMPLE=${SAMPLES[$PROCESS]}

source stageOut.sh
tar -xzf ${CMSSWVER}.tar.gz
cd ${CMSSWVER}
scram b ProjectRename
source /cvmfs/cms.cern.ch/cmsset_default.sh
# cmsenv
eval `scramv1 runtime -sh`
cd src/Analysis

#run macro
EXTRAOPT=""
if [[ $SAMPLE == "SVJ"* ]]; then
	EXTRAOPT='"OPTION","b:splitSVJ[1]"'
fi
echo "run: root -b -q -l 'KScanDriver.C+(\"$SAMPLE\",\"$INDIR\",{\"$INPUT\"},{$EXTRAOPT})' 2>&1"
root -b -q -l 'KScanDriver.C+("'$SAMPLE'","'$INDIR'",{"'$INPUT'"},{'$EXTRAOPT'})' 2>&1

ROOTEXIT=$?

if [[ $ROOTEXIT -ne 0 ]]; then
	rm *.root
	echo "exit code $ROOTEXIT, skipping xrdcp"
	exit $ROOTEXIT
fi

#run followup for T1ttbb only
if [[ $SAMPLE == "T1ttbb"* ]]; then
	for FILE in *.root; do
		FILEBASE=`basename $FILE .root`
		SETDESC="base\tscan\t$FILEBASE\ts:filename[$FILE]"
		echo "run: root -b -q -l 'KScanDriver.C+(\"$FILEBASE\",\".\",{\"$INPUT\"},{\"OPTION\",\"b:splitT1ttbb[1]\",\"SET\",\"$SETDESC\"})' 2>&1"
		root -b -q -l 'KScanDriver.C+("'$FILEBASE'",".",{"'$INPUT'"},{"OPTION","b:splitT1ttbb[1]","SET","'$SETDESC'"})' 2>&1
		
		ROOTEXIT=$?

		if [[ $ROOTEXIT -ne 0 ]]; then
			rm *.root
			echo "exit code $ROOTEXIT, skipping xrdcp"
			exit $ROOTEXIT
		fi		
		
		#remove intermediate scan after splitting up
		rm $FILE
	done
fi

# copy output to eos
echo "xrdcp output for condor"
for FILE in *.root; do
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
