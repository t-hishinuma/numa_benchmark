#!/bin/zsh

function blas_exec(){
	EXEC=$1; MIN=$2; MAX=$3; INCL=$4; TRY=$5; BIN=$6; PREC=$7

	#SDOT
	if [ $EXEC = 1 ]; then
		for th in "${array[@]}"
		do
			for (( i = $MIN ; i<= $MAX ; i=i$INCL ))
			do
				export OMP_NUM_THREADS=$th
				${BIN}_cpu.o $i $TRY $PREC
			done
		done
		if type "nvidia-smi" > /dev/null 2>&1; then
			for (( i = $MIN ; i<= $MAX ; i=i$INCL ))
			do
				export OMP_NUM_THREADS=$th
				${BIN}_gpu.o $i $TRY $PREC
			done
		fi
	fi
}

# main

if [ $# != 1 ]; then
	ENVFILE="./benchmark_config"
else 
	ENVFILE=$1
fi

. $ENVFILE

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

# blas1
blas_exec $SDOT_EXEC $SDOT_MIN_SIZE $SDOT_MAX_SIZE $SDOT_INCL $SDOT_TRIAL ./src/blas1/dot float
blas_exec $DDOT_EXEC $DDOT_MIN_SIZE $DDOT_MAX_SIZE $DDOT_INCL $DDOT_TRIAL ./src/blas1/dot double

# blas3
blas_exec $SGEMM_EXEC $SGEMM_MIN_SIZE $SGEMM_MAX_SIZE $SGEMM_INCL $SGEMM_TRIAL ./src/blas3/gemm float
blas_exec $DGEMM_EXEC $DGEMM_MIN_SIZE $DGEMM_MAX_SIZE $DGEMM_INCL $DGEMM_TRIAL ./src/blas3/gemm double
