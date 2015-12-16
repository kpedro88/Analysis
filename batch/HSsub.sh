#!/bin/bash

source exportProd.sh

JOBDIR=jobs
DIR=/store/user/lpcsusyhad/SusyRA2Analysis2015/${RUN2PRODV}/scan
CHECKARGS=""
nhadds=10

#check arguments
while getopts "kn:" opt; do
  case "$opt" in
  k) CHECKARGS="${CHECKARGS} -k"
    ;;
  n) nhadds=$OPTARG
    ;;
  esac
done

./SKcheck.sh ${CHECKARGS}

#list samples
IFS=$'\n' ALLSAMPLES=($(xrdfs root://cmseos.fnal.gov ls ${DIR} | grep "block")); unset IFS

#remove duplicates
SAMPLELIST=""
SAMPLES=()
for SAMPLE in ${ALLSAMPLES[@]}; do
  BASE=$(echo $(basename ${SAMPLE}) | rev | cut -d'_' -f1-3 --complement | rev)
  #skip duplicates
  if (echo "$SAMPLELIST" | fgrep -qw ${BASE}); then
    continue
  fi
  #append
  SAMPLES+=(${BASE})
  SAMPLELIST="${BASE} ${SAMPLELIST}"
done

#loop vars
counter=0
INPUT=""

#loop over sample array
for ((i=0; i < ${#SAMPLES[@]}; i++)); do
  BASE=${SAMPLES[$i]}
  
  #add sample to input list
  if [[ -z "$INPUT" ]]; then
    INPUT=${BASE}
  else
    INPUT="${INPUT},${BASE}"
  fi
  
  #increment counter
  counter=$((counter+1))
  
  #check counter (or end of loop)
  if [[ $counter -ge $nhadds ]] || [[ $i -eq $((${#SAMPLES[@]} - 1)) ]]; then
    #submit job with this input list
	./HStemp.sh ${JOBDIR} ${INPUT} ${DIR} fast
	
	#reset vars
	INPUT=""
	counter=0
  fi
done
