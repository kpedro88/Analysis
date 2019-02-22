#!/bin/bash

SNAME=$1
PROCESS=$2

source export${SNAME}.sh
echo ${SAMPLES[$PROCESS]}
