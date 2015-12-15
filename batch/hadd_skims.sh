#!/bin/bash

source exportProd.sh

STDIR=/eos/uscms/store/user/lpcsusyhad/SusyRA2Analysis2015/Skims/${RUN2PRODV}

./SKcheck.sh -k

SAMPLES=(
TTJets_SingleLeptFromT \
TTJets_SingleLeptFromTbar \
TTJets_DiLept \
ttHJetTobb_M125 \
TTTT
)

for SAMPLE in ${SAMPLES[@]}; do
  for TREEDIR in ${STDIR}/tree*/; do
    #check to see if anything needs to be hadded
	ls ${TREEDIR}/tree_${SAMPLE}_*.root > /dev/null 2>&1
	LSTEST=$?
	if [[ $LSTEST -ne 0 ]]; then
	  echo "nothing to hadd for $SAMPLE in $TREEDIR"
	  continue
	fi
  
    XRDIR=`echo $TREEDIR | sed 's~/eos/uscms~root://cmseos.fnal.gov/~'`
    ALLFILES=${XRDIR}/tree_${SAMPLE}.root
    for FILE in ${TREEDIR}/tree_${SAMPLE}_*.root; do
      ALLFILES="${ALLFILES} ${XRDIR}/`basename ${FILE}`"
    done

    #hadd to tmp file
    hadd tree_${SAMPLE}.root ${ALLFILES}

    #remove original files
    rm ${TREEDIR}/tree_${SAMPLE}.root
    for FILE in ${TREEDIR}/tree_${SAMPLE}_*.root; do
      rm ${FILE}
    done

    #copy hadded file to eos
    xrdcp -f tree_${SAMPLE}.root ${XRDIR}/

    #remove tmp file
    rm tree_${SAMPLE}.root

  done
done

