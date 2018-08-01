#!/bin/bash

SEL=dijetmthad

HISTOS_ALL=(
0 \
1 \
2 \
3 \
4 \
5 \
)

DOHADD=""

while getopts "h:s:a" opt; do
	case "$opt" in
		h) if [ "$OPTARG" = all ]; then HISTOS=(${HISTOS_ALL[@]}); else IFS="," read -a HISTOS <<< "$OPTARG"; fi
		;;
		s) SEL=$OPTARG
		;;
		a) DOHADD=true
		;;
	esac
done

FILES=(
corrections/svj_${SEL}_jetAK8 \
test/svj_${SEL}_event \
test/svj_${SEL}_event2 \
test/svj_${SEL}_thirdjet \
test/svj_${SEL}_bothjet \
test/svj_${SEL}_shape \
test/svj_${SEL}_bothjet_extra \
test/svj_${SEL}_bothjet_fractions \
test/svj_${SEL}_bothjet_mults \
)

COMMANDS=(
'KPlotDriver.C+("root://cmseos.fnal.gov//store/user/lpcsusyhad/SVJ2017/Run2ProductionV14/Skims/tree_'${SEL}'",{"input/input_svj_hist.txt","input/input_svj_rocs_jetpt.txt","input/input_svj_rocs_sets.txt"},{"OPTION","string:rootfile['${FILES[0]}']"},0)' \
'KPlotDriver.C+("root://cmseos.fnal.gov//store/user/lpcsusyhad/SVJ2017/Run2ProductionV14/Skims/tree_'${SEL}'",{"input/input_svj_hist.txt","input/input_svj_rocs_event.txt","input/input_svj_rocs_sets.txt"},{"OPTION","string:rootfile['${FILES[1]}']"},0)' \
'KPlotDriver.C+("root://cmseos.fnal.gov//store/user/lpcsusyhad/SVJ2017/Run2ProductionV14/Skims/tree_'${SEL}'",{"input/input_svj_hist.txt","input/input_svj_rocs_event2.txt","input/input_svj_rocs_sets.txt"},{"OPTION","string:rootfile['${FILES[2]}']"},0)' \
'KPlotDriver.C+("root://cmseos.fnal.gov//store/user/lpcsusyhad/SVJ2017/Run2ProductionV14/Skims/tree_'${SEL}'",{"input/input_svj_hist.txt","input/input_svj_rocs_thirdjet.txt","input/input_svj_rocs_sets.txt"},{"OPTION","string:rootfile['${FILES[3]}']"},0)' \
'KPlotDriver.C+("root://cmseos.fnal.gov//store/user/lpcsusyhad/SVJ2017/Run2ProductionV14/Skims/tree_'${SEL}'",{"input/input_svj_hist.txt","input/input_svj_rocs_bothjet.txt","input/input_svj_rocs_sets.txt","input/input_svj_flatten_bothjet.txt"},{"OPTION","string:rootfile['${FILES[4]}']"},0)' \
'KPlotDriver.C+("root://cmseos.fnal.gov//store/user/lpcsusyhad/SVJ2017/Run2ProductionV14/Skims/tree_'${SEL}'",{"input/input_svj_hist.txt","input/input_svj_eventshape.txt","input/input_svj_rocs_shapeAK4.txt","input/input_svj_rocs_fwmAK4.txt","input/input_svj_rocs_shapeAK8.txt","input/input_svj_rocs_fwmAK8.txt","input/input_svj_rocs_sets.txt"},{"OPTION","string:rootfile['${FILES[5]}']"},0)' \
'KPlotDriver.C+("root://cmseos.fnal.gov//store/user/lpcsusyhad/SVJ2017/ProductionV2/Skims/tree_'${SEL}'",{"input/input_svj_hist.txt","input/input_svj_rocs_bothjet_extra.txt","input/input_svj_rocs_sets_qcdsig.txt","input/input_svj_flatten_bothjet.txt"},{"OPTION","string:rootfile['${FILES[6]}']"},0)' \
'KPlotDriver.C+("root://cmseos.fnal.gov//store/user/lpcsusyhad/SVJ2017/ProductionV2/Skims/tree_'${SEL}'",{"input/input_svj_hist.txt","input/input_svj_rocs_bothjet_fractions.txt","input/input_svj_rocs_sets_qcdsig.txt","input/input_svj_flatten_bothjet.txt"},{"OPTION","string:rootfile['${FILES[7]}']"},0)' \
'KPlotDriver.C+("root://cmseos.fnal.gov//store/user/lpcsusyhad/SVJ2017/ProductionV2/Skims/tree_'${SEL}'",{"input/input_svj_hist.txt","input/input_svj_rocs_bothjet_mults.txt","input/input_svj_rocs_sets_qcdsig.txt","input/input_svj_flatten_bothjet.txt"},{"OPTION","string:rootfile['${FILES[8]}']"},0)' \
)

PIDS=()

for HISTO in ${HISTOS[@]}; do
	if [ -z "${COMMANDS[$HISTO]}" ]; then
		echo "unknown histo set number $HISTO"
		continue
	fi

	# run in parallel (except jetpt, needed for bothjet)
	echo "root -b -l -q '${COMMANDS[$HISTO]}'"
	if [ "$HISTO" -eq "0" ]; then
		root -b -l -q ${COMMANDS[$HISTO]} >& log_h${HISTO}.log 2>&1
	else
		root -b -l -q ${COMMANDS[$HISTO]} >& log_h${HISTO}.log 2>&1 &
		PIDS[$HISTO]=$!
	fi
done

for PID in ${PIDS[@]}; do
	wait $PID
done

ALLFILES=""
for FILE in ${FILES[@]}; do
	ALLFILES="${ALLFILES} ${FILE}.root"
done

if [ -n "$DOHADD" ]; then
	hadd -f test/allHistos_${SEL}.root ${ALLFILES}
fi
