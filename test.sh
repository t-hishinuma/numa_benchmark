#!/bin/zsh

if [ $# != 1 ]; then
	echo "error \$1 is config filename"
	exit 1
fi

. $1
array=()

if [ -n "$THREADS" ]; then
	array=($THREADS)
else
	MAXTH=`grep processor /proc/cpuinfo | wc -l` 
	for ((i=$MAXTH;i>=1;i=i/2))
	do
		array+=$i
	done
fi

#DDOT
if [ $DDOT_EXEC = 1 ]; then
	for th in "${array[@]}"
	do
		for (( i = $DDOT_MIN_SIZE ; i<= $DDOT_MAX_SIZE ; i=i$DDOT_INCL ))
		do
			export OMP_NUM_THREADS=$th
			./src/blas1/dot.o $i $DDOT_TRIAL double
		done
	done
fi

#SDOT
if [ $SDOT_EXEC = 1 ]; then
	for th in "${array[@]}"
	do
		for (( i = $SDOT_MIN_SIZE ; i<= $SDOT_MAX_SIZE ; i=i$SDOT_INCL ))
		do
			export OMP_NUM_THREADS=$th
			./src/blas1/dot.o $i $SDOT_TRIAL float
		done
	done
fi
