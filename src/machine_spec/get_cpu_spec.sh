#/bin/bash


## CPU Spec
CORE=`cat /proc/cpuinfo | grep processor | wc -l`
CPUS=`cat /proc/cpuinfo | grep "physical id" | sort -u | wc -l`
MODEL=`cat /proc/cpuinfo | sed -nr '/model name/ s/.*:\s*(.*)/\1/p' | sort -u`
MEM=`lsmem | grep "online memory" | awk '{print $4}'`

printf "-"
printf " {"
printf "\"type\" : \"info\""
printf ", "
printf "\"# of physical cpu\" : $CPUS"
printf ", "
printf "\"CPU model\" : \"$MODEL\""
printf ", "
printf "\"# of cores\" : $CORE"
printf ", "
printf "\"# of memory\" : \"$MEM\""
printf "}\n"
