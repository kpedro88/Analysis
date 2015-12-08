#!/bin/bash

source exportProd.sh

STDIR=/eos/uscms/store/user/pedrok/SUSY2015/Analysis/Datacards/${RUN2PRODV}
XRDIR=`echo $STDIR | sed 's~/eos/uscms~root://cmseos.fnal.gov/~'`

./SKcheck.sh keep

#initialize parameters
RUN=0
UPDATE=0

#check arguments
while getopts "ru" opt; do
  case "$opt" in
  r) RUN=1
    ;;
  u) UPDATE=1
    ;;
  esac
done

#if updating, part number may not start with 0
if [[ $UPDATE -eq 1 ]]; then
  SAMPLES=`ls $STDIR/*_part*.root 2>&1`
else
  SAMPLES=`ls $STDIR/*_part0.root 2>&1`
fi

LSTEST=$?
if [[ $LSTEST -ne 0 ]]; then
  echo "nothing to hadd in $STDIR"
  exit 0
fi

SAMPLELIST=""

for SAMPLE in ${SAMPLES}; do
  BASE=$(echo $(basename ${SAMPLE}) | rev | cut -d'_' -f1-1 --complement | rev)
  #skip duplicates
  if (echo "$SAMPLELIST" | fgrep -qw ${BASE}); then
    continue
  fi
  echo ${BASE}
  
  #check to see if anything needs to be hadded
  ls ${STDIR}/${BASE}_part*.root > /dev/null 2>&1
  LSTEST=$?
  if [[ $LSTEST -ne 0 ]]; then
    echo "nothing to hadd for $BASE"
    continue
  fi
  
  ALLFILES=""
  for FILE in ${STDIR}/${BASE}_part*.root; do
    ALLFILES="${ALLFILES} ${XRDIR}/`basename ${FILE}`"
  done
  #include base file when updating
  if [[ $UPDATE -eq 1 ]]; then
    ALLFILES="${XRDIR}/${BASE}.root ${ALLFILES}"
  fi
  
  #dryrun (list nfiles) is default
  if [[ $RUN -eq 1 ]]; then
    #hadd to tmp file
    hadd ${BASE}.root ${ALLFILES}
    
    #remove original files
    for FILE in ${STDIR}/${BASE}_part*.root; do
      rm ${FILE}
    done
    
    #copy hadded file to eos
    xrdcp -f ${BASE}.root ${XRDIR}/
    
    #remove tmp file
    rm ${BASE}.root
  else
    echo ${ALLFILES} | wc -w
  fi
  
  #append sample to list
  SAMPLELIST="${BASE} ${SAMPLELIST}"
done

