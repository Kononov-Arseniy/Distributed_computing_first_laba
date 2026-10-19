#!/bin/bash

POINTS=2000000
RUNS=10

for t in 1 2 4 8 16
do
    echo "threads = $t"
    all=""
    for i in $(seq $RUNS)
    do
        result=$(./main $t $POINTS)
        echo "  run $i: $result s"
        all="$all $result"
    done
    echo $all | tr ' ' '\n' | sort -n | awk '{a[NR]=$1; sum+=$1} END {printf "  min %.3f   median %.3f   mean %.3f\n\n", a[1], a[int((NR+1)/2)], sum/NR}'
done