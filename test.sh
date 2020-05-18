#!/bin/zsh

## BLAS
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

## FFT
function fft_exec(){
	EXEC=$1; MIN=$2; MAX=$3; INCL=$4; TRY=$5; BIN=$6; PREC=$7; TYPE=$8; DIM=$9

	#SDOT
	if [ $EXEC = 1 ]; then
		for th in "${array[@]}"
		do
			for (( i = $MIN ; i<= $MAX ; i=i$INCL ))
			do
				export OMP_NUM_THREADS=$th
				${BIN}_cpu_${PREC}.o $i $TRY $TYPE $DIM
			done
		done
		if type "nvidia-smi" > /dev/null 2>&1; then
			for (( i = $MIN ; i<= $MAX ; i=i$INCL ))
			do
				export OMP_NUM_THREADS=$th
				${BIN}_gpu_${PREC}.o $i $TRY $TYPE $DIM
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
	MAXTH=`grep processor /proc/cpuinfo | wc -l` 
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

# blas2
blas_exec $SGEMV_EXEC $SGEMV_MIN_SIZE $SGEMV_MAX_SIZE $SGEMV_INCL $SGEMV_TRIAL ./src/blas2/gemv float
blas_exec $DGEMV_EXEC $DGEMV_MIN_SIZE $DGEMV_MAX_SIZE $DGEMV_INCL $DGEMV_TRIAL ./src/blas2/gemv double

# blas3
blas_exec $SGEMM_EXEC $SGEMM_MIN_SIZE $SGEMM_MAX_SIZE $SGEMM_INCL $SGEMM_TRIAL ./src/blas3/gemm float
blas_exec $DGEMM_EXEC $DGEMM_MIN_SIZE $DGEMM_MAX_SIZE $DGEMM_INCL $DGEMM_TRIAL ./src/blas3/gemm double

# fft-1D
fft_exec $SFFT1D_FORWARD_EXEC $SFFT1D_MIN_SIZE $SFFT1D_MAX_SIZE $SFFT1D_INCL $SFFT1D_TRIAL ./src/fft/fft f forward 1
fft_exec $SFFT1D_FORWARD_EXEC $SFFT1D_MIN_SIZE $SFFT1D_MAX_SIZE $SFFT1D_INCL $SFFT1D_TRIAL ./src/fft/fft d forward 1
fft_exec $DFFT1D_BACKWARD_EXEC $DFFT1D_MIN_SIZE $DFFT1D_MAX_SIZE $DFFT1D_INCL $DFFT1D_TRIAL ./src/fft/fft f backward 1
fft_exec $DFFT1D_BACKWARD_EXEC $DFFT1D_MIN_SIZE $DFFT1D_MAX_SIZE $DFFT1D_INCL $DFFT1D_TRIAL ./src/fft/fft d backward 1

# fft-2D
fft_exec $SFFT1D_FORWARD_EXEC $SFFT1D_MIN_SIZE $SFFT1D_MAX_SIZE $SFFT1D_INCL $SFFT1D_TRIAL ./src/fft/fft f forward 2
fft_exec $SFFT1D_FORWARD_EXEC $SFFT1D_MIN_SIZE $SFFT1D_MAX_SIZE $SFFT1D_INCL $SFFT1D_TRIAL ./src/fft/fft d forward 2
fft_exec $DFFT1D_BACKWARD_EXEC $DFFT1D_MIN_SIZE $DFFT1D_MAX_SIZE $DFFT1D_INCL $DFFT1D_TRIAL ./src/fft/fft f backward 2
fft_exec $DFFT1D_BACKWARD_EXEC $DFFT1D_MIN_SIZE $DFFT1D_MAX_SIZE $DFFT1D_INCL $DFFT1D_TRIAL ./src/fft/fft d backward 2

# fft-3D
fft_exec $SFFT1D_FORWARD_EXEC $SFFT1D_MIN_SIZE $SFFT1D_MAX_SIZE $SFFT1D_INCL $SFFT1D_TRIAL ./src/fft/fft f forward 3
fft_exec $SFFT1D_FORWARD_EXEC $SFFT1D_MIN_SIZE $SFFT1D_MAX_SIZE $SFFT1D_INCL $SFFT1D_TRIAL ./src/fft/fft d forward 3
fft_exec $DFFT1D_BACKWARD_EXEC $DFFT1D_MIN_SIZE $DFFT1D_MAX_SIZE $DFFT1D_INCL $DFFT1D_TRIAL ./src/fft/fft f backward 3
fft_exec $DFFT1D_BACKWARD_EXEC $DFFT1D_MIN_SIZE $DFFT1D_MAX_SIZE $DFFT1D_INCL $DFFT1D_TRIAL ./src/fft/fft d backward 3

