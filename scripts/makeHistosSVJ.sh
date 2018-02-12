#!/bin/bash

SEL=dijethad

HISTOS_ALL=(
0 \
1 \
2 \
)

while getopts "h:s:" opt; do
	case "$opt" in
		h) if [ "$OPTARG" = all ]; then HISTOS=(${HISTOS_ALL[@]}); else IFS="," read -a HISTOS <<< "$OPTARG"; fi
		;;
		s) SEL=$OPTARG
		;;
	esac
done

FILES=(
test/svj_${SEL}_event \
corrections/svj_${SEL}_jetAK8 \
test/svj_${SEL}_bothjet \
)

COMMANDS=(
'KPlotDriver.C+("root://cmseos.fnal.gov//store/user/lpcsusyhad/SVJ2017/Run2ProductionV14/Skims/tree_'${SEL}'",{"input/input_svj_hist.txt","input/input_svj_rocs_event.txt","input/input_svj_rocs_sets.txt"},{"OPTION","string:rootfile['${FILES[0]}']"},0)' \
'KPlotDriver.C+("root://cmseos.fnal.gov//store/user/lpcsusyhad/SVJ2017/Run2ProductionV14/Skims/tree_'${SEL}'",{"input/input_svj_hist.txt","input/input_svj_rocs_jetpt.txt","input/input_svj_rocs_sets.txt"},{"OPTION","string:rootfile['${FILES[1]}']"},0)' \
'KPlotDriver.C+("root://cmseos.fnal.gov//store/user/lpcsusyhad/SVJ2017/Run2ProductionV14/Skims/tree_'${SEL}'",{"input/input_svj_hist.txt","input/input_svj_rocs_bothjet.txt","input/input_svj_rocs_sets.txt","input/input_svj_flatten_bothjet.txt"},{"OPTION","string:rootfile['${FILES[2]}']"},0)' \
)

PIDS=()

for HISTO in ${HISTOS[@]}; do
	if [ -z "${COMMANDS[$HISTO]}" ]; then
		echo "unknown histo set number $HISTO"
		continue
	fi

	# run in parallel
	echo "root -b -l -q '${COMMANDS[$HISTO]}'"
	root -b -l -q ${COMMANDS[$HISTO]} >& log_h${HISTO}.log 2>&1 &
	PIDS[$HISTO]=$!
done

for PID in ${PIDS[@]}; do
	wait $PID
done

ALLFILES=""
for FILE in ${FILES[@]}; do
	ALLFILES="${ALLFILES} ${FILE}.root"
done

hadd -f test/allHistos_${SEL}.root ${ALLFILES}
