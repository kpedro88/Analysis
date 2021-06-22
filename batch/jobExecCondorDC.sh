#!/bin/bash

#
# variables from arguments string in jdl
#

echo "Starting job on " `date` #Only to display the starting of production date
echo "Running on " `uname -a` #Only to display the machine where the job is running
echo "System release " `cat /etc/redhat-release` #And the system release
echo "CMSSW on Condor"

CMSSWVER=$1
STORE=$2
JOBNAME=$3
SNAME=$4
PROCESS=$5
TYPE=$6
YEAR=$7

echo ""
echo "parameter set:"
echo "CMSSWVER:   $CMSSWVER"
echo "STORE:      $STORE"
echo "JOBNAME:    $JOBNAME"
echo "SNAME:      $SNAME"
echo "PROCESS:    $PROCESS"
echo "TYPE:       $TYPE"
echo "YEAR:       $YEAR"

# get sample
source export${SNAME}.sh
SAMPLE=${SAMPLES[$PROCESS]}

source stageOut.sh
tar -xzf ${CMSSWVER}.tar.gz
cd ${CMSSWVER}
source /cvmfs/cms.cern.ch/cmsset_default.sh
scram b ProjectRename
# cmsenv
eval `scramv1 runtime -sh`
cd src/Analysis

# option to run over all years in series for SVJ
NEWSAMPLES=()
POSTPROCESS=""
if [[ ( "$TYPE" = SVJ || "$TYPE" == "SVJScan"* ) && "$YEAR" = Run2 ]]; then
	NEWSAMPLES=(${SAMPLE}_MC2016 ${SAMPLE}_MC2017 ${SAMPLE}_MC2018PRE ${SAMPLE}_MC2018POST)
	if [[ ( "$TYPE" = SVJ || "$TYPE" == "SVJScan"* && "$TYPE" != SVJScanBenchmark ) ]]; then
		POSTPROCESS=true
	fi
else
	NEWSAMPLES=(${SAMPLE})
fi

for NEWSAMPLE in ${NEWSAMPLES[@]}; do
	# run macro
	MACRO=$(cat $_CONDOR_SCRATCH_DIR/macro_${JOBNAME}.txt | sed 's/NEWSAMPLE/'$NEWSAMPLE'/')
	echo "run: root -b -q -l '$MACRO'"
	root -b -q -l "${MACRO}" 2>&1

	ROOTEXIT=$?

	if [[ $ROOTEXIT -ne 0 ]]; then
		rm *.root
		echo "exit code $ROOTEXIT for $NEWSAMPLE, skipping xrdcp"
		exit $ROOTEXIT
	fi
done

# postprocessing for SVJ
if [ -n "$POSTPROCESS" ]; then
	python processDatacardsSVJ.py -o datacard_${SAMPLE}.root -f MTAK8_*.root -R 2
	PROCEXIT=$?
	if [[ $PROCEXIT -ne 0 ]]; then
		rm *.root
		echo "exit code $PROCEXIT, failure in postprocessing, skipping xrdcp"
		exit $PROCEXIT
	fi

	# make one final datacard per signal w/ bkg & data included
	hadd datacard_final_${SAMPLE}.root datacard_${SAMPLE}.root ${STORE}/datacard_bkg_data.root
	HADDEXIT=$?
	if [[ $HADDEXIT -ne 0 ]]; then
		rm *.root
		echo "exit code $HADDEXIT, failure in postprocess hadd, skipping xrdcp"
		exit $HADDEXIT
	fi
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



