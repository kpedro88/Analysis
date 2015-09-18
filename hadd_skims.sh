#!/bin/bash

STDIR=/eos/uscms/store/user/pedrok/SUSY2015/Analysis/Skims/Run2ProductionV1

./FScheck.sh keep

SAMPLES="
TTJets_SingleLeptFromT \
TTJets_SingleLeptFromTbar \
TTJets_DiLept \
ttHJetTobb_M125
"

for SAMPLE in ${SAMPLES}; do
  for TREEDIR in ${STDIR}/tree*/; do
  
    ALLFILES=""
    for FILE in ${TREEDIR}/tree_${SAMPLE}*.root; do
      ALLFILES="${ALLFILES} ${XRDIR}/`basename ${FILE}`"
    done

    #hadd to tmp file
    hadd tree_${SAMPLE}.root ${ALLFILES}

    #remove original files
    for FILE in ${TREEDIR}/tree_${SAMPLE}*.root; do
      rm ${FILE}
    done

    #copy hadded file to eos
    XRDIR=`echo $TREEDIR | sed 's~/eos/uscms~root://cmseos.fnal.gov/~'`
    xrdcp -f tree_${SAMPLE}.root ${XRDIR}/

    #remove tmp file
    rm tree_${SAMPLE}.root

  done
done

