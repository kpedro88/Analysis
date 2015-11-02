#!/bin/bash

STDIR=/eos/uscms/store/user/pedrok/SUSY2015/Analysis/Datacards/Run2ProductionV4
XRDIR=`echo $STDIR | sed 's~/eos/uscms~root://cmseos.fnal.gov/~'`

./SKcheck.sh keep

SAMPLES=`ls $STDIR/*_part0.root`

for SAMPLE in ${SAMPLES}; do
  BASE=$(echo $(basename ${SAMPLE}) | rev | cut -d'_' -f1-1 --complement | rev)
  echo ${BASE}
  ALLFILES=""
  for FILE in ${STDIR}/${BASE}_part*.root; do
    ALLFILES="${ALLFILES} ${XRDIR}/`basename ${FILE}`"
  done
  
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
done

