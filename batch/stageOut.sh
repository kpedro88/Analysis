#!/bin/bash

stageOut() {
	WAIT=5
	NUMREP=5
	INPUT=""
	OUTPUT=""
	XRDARGS=""
	QUIET=0

	stageOut_usage() {
		case `uname` in
			Linux) ECHO="echo -e" ;;
			*) ECHO="echo" ;;
		esac

		$ECHO "stageOut [options]"
		$ECHO ""
		$ECHO "Options:"
		$ECHO "-i input      \tinput file name (required)"
		$ECHO "-o output     \toutput file name (required)"
		$ECHO "-w wait       \twait time in seconds (default = $WAIT)"
		$ECHO "-n num        \tnumber of repetitions (default = $NUMREP)"
		$ECHO "-x args       \tany arguments to pass to xrdcp (should be quoted)"
		$ECHO "-q            \tquiet (don't print any messages)"
	}

	# set vars used by getopts to local
	local OPTIND OPTARG
	while getopts "i:o:w:n:x:q" opt; do
		case "$opt" in
		i) INPUT="$OPTARG"
		;;
		o) OUTPUT="$OPTARG"
		;;
		w) WAIT="$OPTARG"
		;;
		n) NUMREP="$OPTARG"
		;;
		x) XRDARGS="$OPTARG"
		;;
		q) QUIET=1
		;;
		esac
	done

	if [[ -z "$INPUT" ]] || [[ -z "$OUTPUT" ]]; then
		stageOut_usage
		return 1
	fi

	# try to copy n times, increasing wait each time
	TMPWAIT=0
	for ((i=0; i < $NUMREP; i++)); do
		xrdcp $XRDARGS $INPUT $OUTPUT
		XRDEXIT=$?
		if [ $XRDEXIT -eq 0 ]; then
			return 0
		fi
		# in case of bad exit, wait and try again
		TMPWAIT=$(($TMPWAIT + $WAIT))
		if [ $QUIET -eq 0 ]; then echo "Exit code $XRDEXIT, failure in xrdcp. Retry after $TMPWAIT seconds..."; fi
		sleep $TMPWAIT
	done

	# if we get here, it really didn't work
	if [ $QUIET -eq 0 ]; then echo "xrdcp failed $NUMREP times. It might be an actual problem."; fi
	return 60000
}
