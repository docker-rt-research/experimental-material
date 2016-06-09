#!/bin/bash
# Purpose: Runscript for RealtimeService
#		   to record data for a certain
#		   amount of iterations.
# Author: Philip Masek
# 
# Flags:
# -o [Occupy]
# -i [Iterations to run the RT code]
# -f [Frequency]
# 
# ---------------------------------------



usage() { echo "Usage: $0 [-o <1..99>] [-f <int>] [-i <int>]" 1>&2; exit 1; }

while getopts ":f:i:o:" opt; do
    case "${opt}" in
        o)
            o=${OPTARG}
            ((o >= 1 || o <= 99)) || usage
            ;;
        f)
            f=${OPTARG}
            ;;
        i)
            i=${OPTARG}
            ;;
        *)
            usage
            ;;
    esac
done
shift $((OPTIND-1))

if [ -z "${f}" ] || [ -z "${i}" ] || [ -z "${o}" ]; then
    usage
fi

for ((x=1;x<=$i;x++))
do
	./timetrigger --cid=111 --freq=${f} --duration 3600 --occupy ${o} --realtime=49;
	echo "${x} iteration(s) done";
	sleep 60;
done
