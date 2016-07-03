#!/bin/bash

source exportProd.sh

JOBDIR=jobs
DIR=/store/user/lpcsusyhad/SusyRA2Analysis2015/${RUN2PRODV}/scan
CHECKARGS=""
nhadds=10
RUN=0
SUFF=fast

#check arguments
while getopts "kn:rsd:" opt; do
  case "$opt" in
  k) CHECKARGS="${CHECKARGS} -k"
    ;;
  n) nhadds=$OPTARG
    ;;
  r) RUN=1
    ;;
  s) SUFF=""
    ;;
  d) DIR=$OPTARG;
    ;;
  esac
done

./SKcheck.sh ${CHECKARGS}

#list samples
IFS=$'\n' SAMPLES=($(python findHadds.py -d ${DIR} -g "_block")); unset IFS

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
    #dryrun (list input) is default
    if [[ $RUN -eq 1 ]]; then
      #submit job with this input list
      ./HStemp.sh ${JOBDIR} ${INPUT} ${DIR} ${SUFF}
    else
      echo ${INPUT}
    fi
	
    #reset vars
    INPUT=""
    counter=0
  fi
done
