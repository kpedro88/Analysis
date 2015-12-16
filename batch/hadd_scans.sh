#!/bin/bash

source exportProd.sh

STDIR=/eos/uscms/store/user/lpcsusyhad/SusyRA2Analysis2015/${RUN2PRODV}/scan
XRDIR=`echo $STDIR | sed 's~/eos/uscms~root://cmseos.fnal.gov/~'`

./SKcheck.sh keep

#initialize parameters
RUN=0

#check arguments
while getopts "r" opt; do
  case "$opt" in
  r) RUN=1
    ;;
  esac
done

#get list of samples by first part of first block
SAMPLES=`ls $STDIR/*_block0_part0_*.root 2>&1`

LSTEST=$?
if [[ $LSTEST -ne 0 ]]; then
  echo "nothing to hadd in $STDIR"
  exit 0
fi

for SAMPLE in ${SAMPLES}; do
  BASE=$(echo $(basename ${SAMPLE}) | rev | cut -d'_' -f1-3 --complement | rev)
  echo ${BASE}
  
  #check to see if anything needs to be hadded
  STDFILES=`ls ${STDIR}/${BASE}_block*.root 2>&1`
  LSTEST=$?
  if [[ $LSTEST -ne 0 ]]; then
    echo "nothing to hadd for $BASE"
    continue
  fi
  
  ALLFILES=""
  for FILE in ${STDFILES}; do
    ALLFILES="${ALLFILES} ${XRDIR}/`basename ${FILE}`"
  done
  
  #todo: add simple case, if nfiles==1, just rename file
  
  #dryrun (list nfiles) is default
  if [[ $RUN -eq 1 ]]; then
    #hadd to tmp file
    hadd ${BASE}.root ${ALLFILES}
    
    #remove original files
    for FILE in ${STDIR}/${BASE}_block*.root; do
      rm ${FILE}
    done
    
    #copy hadded file to eos
    xrdcp -f ${BASE}.root ${XRDIR}/
    
    #remove tmp file
    rm ${BASE}.root
  else
    echo ${ALLFILES} | wc -w
  fi
done

