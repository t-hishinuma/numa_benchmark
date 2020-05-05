#/bin/bash


## CPU Spec
CORE=`cat /proc/cpuinfo | grep processor | wc -l`
CPUS=`cat /proc/cpuinfo | grep "physical id" | sort -u | wc -l`
MODEL=`cat /proc/cpuinfo | sed -nr '/model name/ s/.*:\s*(.*)/\1/p' | sort -u`
MEM=`lsmem | grep "online memory" | awk '{print $4}'`

# SIMD
SIMD=""
if [[ `grep avx512 /proc/cpuinfo` ]]; then
	SIMD="AVX512"
elif [[ `grep avx2 /proc/cpuinfo` ]]; then
	SIMD="AVX2"
elif [[ `grep avx /proc/cpuinfo` ]]; then
	SIMD="AVX"
elif [[ `grep sse2 /proc/cpuinfo` ]]; then
	SIMD="SSE2"
else
	SIMD=""
fi

# GPU
if type "nvidia-smi" > /dev/null 2>&1; then
	GPU=`nvidia-smi -L`
else
	GPU="none"
fi

printf "-"
printf " {"
printf "\"type\" : \"info\""
printf ", "
printf "\"physical_cpu\" : $CPUS"
printf ", "
printf "\"cpu\" : \"$MODEL\""
printf ", "
printf "\"cores\" : $CORE"
printf ", "
printf "\"simd\" : $SIMD"
printf ", "
printf "\"memory\" : \"$MEM\""
printf ", "
printf "\"gpu\" : \"$GPU\""
printf "}\n"
