#!/bin/bash
KEYS=1000
OPS=100000
RUNS=3

check_results() {
    out_my=$(echo "$KEYS $OPS $3" | ./$1 $2 | grep -v "Elapsed time")
    out_lib=$(echo "$KEYS $OPS $3" | ./$4 $2 | grep -v "Elapsed time")

    # Сравниваем
    if [ "$out_my" != "$out_lib" ]; then
        echo "outputs between $1 and mix: $4 are different"
        echo "Output $1"
        echo "$out_my"
        echo "Output $4"
        echo "$out_lib"
        echo "----------------------"
        exit 1
    fi
}

median_time() {
    all=""
    for i in $(seq $RUNS)
    do
        x=$(echo "$KEYS $OPS $3" | ./$1 $2 | grep Elapsed | awk '{print $4}')
        all="$all $x"
    done
    echo $all | tr ' ' '\n' | sort -g | awk '{a[NR]=$1} END {printf "%.3f", a[int((NR+1)/2)]}'
}

for mix in "0.999 0.0005" "0.9 0.05" "0.5 0.25"
do
    echo "=== поиски и вставки: $mix ==="
    echo "потоки  lib_rwlock  my_rwlock  my_rwlock/lib_rwlock"
    for t in 1 2 4 8 16
    do
        check_results "my_ll" $t "$mix" "lib_ll"
        lib=$(median_time lib_ll $t "$mix")
        my=$(median_time my_ll $t "$mix")
        ratio=$(awk -v a=$my -v b=$lib 'BEGIN {printf "%.2f", a/b}')
        echo "  $t       $lib       $my      $ratio"
    done
    echo ""
done