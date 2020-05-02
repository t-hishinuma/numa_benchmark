#!/bin/bash
cd /work/
cd OpenBLAS; mkdir build; cd build; cmake ..; make -j; make install
