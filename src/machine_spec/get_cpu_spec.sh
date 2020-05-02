#/bin/bash

CORE=`cat /proc/cpuinfo | grep processor | wc -l`
CPUS=`cat /proc/cpuinfo | grep "physical id" | sort -u | wc -l`

echo $CORE
echo $CPUS

