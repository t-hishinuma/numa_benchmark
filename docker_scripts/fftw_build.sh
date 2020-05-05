#!/bin/bash
. $1

if [[ $FFTW_BUILD = 1 ]]; then

	printf "-"
   	printf " {"
	printf "\"type\" : \"info\", "
	printf "\"openblas build\" : \"source\", "
	printf "\"openblas option\" : \"$OPENBLAS_CMAKE_OPTION\""
	printf "}\n"

	# float
	./configure --enable-avx2 --enable-single --enable-threads --enable-openmp --enable-shared
	make -j
	make install 

	# double
	./configure --enable-avx2 --enable-threads --enable-openmp --enable-shared
	make -j
	make install 

else

	printf "-"
   	printf " {"
	printf "\"type\" : \"info\", "
	printf "\"fftw build\" : \"apt\""
	printf "}\n"

	apt update -y &> /work/result/cblas_build.log
	apt install -y libfftw3-dev &>> /work/result/fft_build.log 

fi
