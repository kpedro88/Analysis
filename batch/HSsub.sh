#!/bin/bash

source exportProd.sh

JOBDIR=jobs
DIR=/store/user/lpcsusyhad/SusyRA2Analysis2015/${RUN2PRODV}/scan
CHECKARGS=""
TESTARGS=""
nhadds=10
RUN=0
SUFF=fast
SEARCH="_block"
UPDATE=0
SKIPTREE=0
VERBOSE=0

#check arguments
while getopts "kn:rsS:d:g:utv" opt; do
  case "$opt" in
  k) CHECKARGS="${CHECKARGS} -k"
    ;;
  n) nhadds=$OPTARG
    ;;
  r) RUN=1
    ;;
  s) SUFF=""
    ;;
  S) SUFF="$OPTARG"
    ;;
  d) DIR=$OPTARG;
    ;;
  g) SEARCH="$OPTARG";
    ;;
  u) UPDATE=1
     TESTARGS="${TESTARGS} -u"
    ;;
  t) SKIPTREE=1
    ;;
  v) VERBOSE=1
    ;;
  esac
done

TESTARGS="${TESTARGS} -d ${DIR} -g ${SEARCH}"
if [[ -n "$SUFF" ]]; then
  TESTARGS="${TESTARGS} -s ${SUFF}"
fi
if [ "$VERBOSE" -eq 1 ]; then
  TESTARGS="${TESTARGS} -v"
fi

./SKcheck.sh ${CHECKARGS}

#list samples
IFS=$'\n' SAMPLES=($(python findHadds.py -d ${DIR} -g ${SEARCH})); unset IFS

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
      ./HStemp.sh ${JOBDIR} ${INPUT} ${DIR} ${SEARCH} ${UPDATE} ${SKIPTREE} ${SUFF}
    else
      #echo ${INPUT}
      ./haddEOS.sh ${TESTARGS} -i ${INPUT}
    fi
    
    #reset vars
    INPUT=""
    counter=0
  fi
done
