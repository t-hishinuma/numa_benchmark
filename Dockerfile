FROM nvidia/cuda:10.1-base
LABEL maintainer "Toshiaki Hishinuma <hishinuma.toshiaki@gmail.com>"

RUN apt update \
   &&  apt install -y make cmake git zsh\
   &&  apt install -y gcc gfortran \
   &&  apt install -y cuda-cublas-dev-10-0 cuda-cudart-dev-10-0 cuda-compiler-10.0 cuda-nvprof-10-1 cuda-cusolver-dev-10-0 cuda-cusparse-dev-10-0 

RUN apt-get clean \
	&&  rm -rf /var/lib/apt/lists/* 

# Library download
RUN mkdir /numa_bench \
	&& git clone https://github.com/xianyi/OpenBLAS.git /numa_bench/OpenBLAS
ENV LD_LIBRARY_PATH $LD_LIBRARY_PATH:/usr/local/lib/
ENV CPATH $LD_LIBRARY_PATH:/usr/local/include/openblas/

# source copy
COPY ./docker_scripts/run.sh /usr/local/bin/run
COPY ./docker_scripts/* ./test.sh ./benchmark_config /numa_bench/
COPY ./src/ /numa_bench/src/
