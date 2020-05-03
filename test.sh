#!/bin/zsh
#
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

#SDOT
if [ $SDOT_EXEC = 1 ]; then
	for th in "${array[@]}"
	do
		for (( i = $SDOT_MIN_SIZE ; i<= $SDOT_MAX_SIZE ; i=i$SDOT_INCL ))
		do
			export OMP_NUM_THREADS=$th
			./src/blas1/dot_cpu.o $i $SDOT_TRIAL float
		done
	done
	if type "nvidia-smi" > /dev/null 2>&1; then
		for (( i = $SDOT_MIN_SIZE ; i<= $SDOT_MAX_SIZE ; i=i$SDOT_INCL ))
		do
			export OMP_NUM_THREADS=$th
			./src/blas1/dot_gpu.o $i $SDOT_TRIAL float
		done
	fi
fi

#DDOT
if [ $DDOT_EXEC = 1 ]; then
	for th in "${array[@]}"
	do
		for (( i = $DDOT_MIN_SIZE ; i<= $DDOT_MAX_SIZE ; i=i$DDOT_INCL ))
		do
			export OMP_NUM_THREADS=$th
			./src/blas1/dot_cpu.o $i $DDOT_TRIAL double
		done
	done
	if type "nvidia-smi" > /dev/null 2>&1; then
		for (( i = $DDOT_MIN_SIZE ; i<= $DDOT_MAX_SIZE ; i=i$DDOT_INCL ))
		do
			export OMP_NUM_THREADS=$th
			./src/blas1/dot_gpu.o $i $DDOT_TRIAL double
		done
	fi
fi

#SGEMM
if [ $SGEMM_EXEC = 1 ]; then
	for th in "${array[@]}"
	do
		for (( i = $SGEMM_MIN_SIZE ; i<= $SGEMM_MAX_SIZE ; i=i$SGEMM_INCL ))
		do
			export OMP_NUM_THREADS=$th
			./src/blas3/gemm_cpu.o $i $SGEMM_TRIAL float
		done
	done
fi

#DGEMM
if [ $DGEMM_EXEC = 1 ]; then
	for th in "${array[@]}"
	do
		for (( i = $DGEMM_MIN_SIZE ; i<= $DGEMM_MAX_SIZE ; i=i$DGEMM_INCL ))
		do
			export OMP_NUM_THREADS=$th
			./src/blas3/gemm_cpu.o $i $DGEMM_TRIAL double
		done
	done
fi
