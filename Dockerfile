FROM nvidia/cuda:10.1-base
LABEL maintainer "Toshiaki Hishinuma <hishinuma.toshiaki@gmail.com>"

RUN apt update \
   &&  apt install -y make cmake git\
   &&  apt install -y gcc gfortran \
   &&  apt install -y cuda-cublas-dev-10-0 cuda-cudart-dev-10-0 cuda-compiler-10.0 cuda-nvprof-10-1 cuda-cusolver-dev-10-0 cuda-cusparse-dev-10-0 

RUN apt-get clean \
	&&  rm -rf /var/lib/apt/lists/* 

# Library download
RUN mkdir /work
RUN git clone https://github.com/xianyi/OpenBLAS.git /work/OpenBLAS

# source copy
COPY ./docker_scripts/run.sh /usr/local/bin/run
COPY ./src /work/src/
