#!/bin/zsh
. $1
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

#DDOT
for th in "${array[@]}"
do
	for (( i = $DDOT_MIN_SIZE ; i<= $DDOT_MAX_SIZE ; i=i + $DDOT_INCL_SIZE ))
	do
		export OMP_NUM_THREADS=$th
		./src/blas1_stream/dot.o $i $DDOT_TRIAL double
	done
done

#SDOT
for th in "${array[@]}"
do
	for (( i = $SDOT_MIN_SIZE ; i<= $SDOT_MAX_SIZE ; i=i + $SDOT_INCL_SIZE ))
	do
		export OMP_NUM_THREADS=$th
		./src/blas1_stream/dot.o $i $SDOT_TRIAL float
	done
done
