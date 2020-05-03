#!/bin/bash

cd /work/OpenBLAS; mkdir build; cd build; cmake .. ; make -j ; make install 
# apt update -y
# apt install -y libopenblas-dev

