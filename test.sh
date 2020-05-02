#!/bin/zsh
. ./benchmark_config
array=()

if [ -n "$THREADS" ]; then
	array=($THREADS)
else
	MAXTH=`grep cpu.cores /proc/cpuinfo | sort -u | awk '{print \$4}'` 
	for ((i=$MAXTH;i>=1;i=i/2))
	do
		array+=$i
	done
fi

for th in "${array[@]}"
do
	for (( i = $DDOT_MIN_SIZE ; i<= $DDOT_MAX_SIZE ; i=i + $DDOT_INCL_SIZE ))
	do
		export OMP_NUM_THREADS=$th
		./src/blas1_stream/dot.o $i 10
	done
done
