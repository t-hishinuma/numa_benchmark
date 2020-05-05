#!/bin/bash
. $1

SIMD=""
if [[ `grep avx512 /proc/cpuinfo` ]]; then
	SIMD="--enable-avx512"
elif [[ `grep avx2 /proc/cpuinfo` ]]; then
	SIMD="--enable-avx2"
elif [[ `grep avx /proc/cpuinfo` ]]; then
	SIMD="--enable-avx"
else
	SIMD=""
fi

if [[ $FFTW_BUILD = 1 ]]; then

	printf "-"
   	printf " {"
	printf "\"type\" : \"info\", "
	printf "\"fftw build\" : \"source\", "
	printf "\"fftw option\" : \"$OPENBLAS_CMAKE_OPTION $SIMD\""
	printf "}\n"

	# float
	cd /numa_bench/fftw-3.3.8/
	./configure $SIMD --enable-single --enable-threads --enable-openmp --enable-shared &> /work/result/fft_build.log
	make -j &> /work/result/fft_build.log
	make install &> /work/result/fft_build.log 

	# double
	./configure $SIMD --enable-threads --enable-openmp --enable-shared &> /work/result/fft_build.log
	make -j &> /work/result/fft_build.log
	make install &> /work/result/fft_build.log 
	cd -

else

	printf "-"
   	printf " {"
	printf "\"type\" : \"info\", "
	printf "\"fftw build\" : \"apt\""
	printf "}\n"

	apt update -y &> /work/result/cblas_build.log
	apt install -y libfftw3-dev &>> /work/result/fft_build.log 
fi
