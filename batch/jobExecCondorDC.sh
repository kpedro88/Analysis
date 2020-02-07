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
VARS=$4
STORE=$5
SNAME=$6
DCCONFIG=$7
PROCESS=$8
TYPE=$9
YEAR=${10}
REGION=${11}

echo ""
echo "parameter set:"
echo "CMSSWVER:   $CMSSWVER"
echo "INDIR:      $INDIR"
echo "SYSTS:      $SYSTS"
echo "VARS:       $VARS"
echo "STORE:      $STORE"
echo "SNAME:      $SNAME"
echo "DCCONFIG:   $DCCONFIG"
echo "PROCESS:    $PROCESS"
echo "TYPE:       $TYPE"
echo "YEAR:       $YEAR"
echo "REGION:     $REGION"

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

# option to run over all years in series for SVJ
NEWSAMPLES=()
POSTPROCESS=""
if [[ ( "$TYPE" = SVJ || "$TYPE" = SVJScan ) && "$YEAR" = Run2 ]]; then
	NEWSAMPLES=(${SAMPLE}_MC2016 ${SAMPLE}_MC2017 ${SAMPLE}_MC2018PRE ${SAMPLE}_MC2018POST)
	POSTPROCESS=true
else
	NEWSAMPLES=(${SAMPLE})
fi

for NEWSAMPLE in ${NEWSAMPLES[@]}; do
	# run macro
	echo "run: root -b -q -l 'MakeAllDCsyst.C+("'"'$DCCONFIG'","'$NEWSAMPLE'","'$REGION'","'$INDIR'","'$SYSTS'","'$VARS'"'")' 2>&1"
	root -b -q -l 'MakeAllDCsyst.C+("'$DCCONFIG'","'$NEWSAMPLE'","'$REGION'","'$INDIR'","'$SYSTS'","'$VARS'")' 2>&1

	ROOTEXIT=$?

	if [[ $ROOTEXIT -ne 0 ]]; then
		rm *.root
		echo "exit code $ROOTEXIT for $NEWSAMPLE, skipping xrdcp"
		exit $ROOTEXIT
	fi
done

# postprocessing for SVJ
if [ -n "$POSTPROCESS" ]; then
	python processDatacardsSVJ.py -o datacard_${SAMPLE}.root -f MTAK8_${REGION}_${SAMPLE}_*.root
fi

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



