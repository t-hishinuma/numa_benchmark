#/bin/bash


## CPU Spec
CORE=`cat /proc/cpuinfo | grep processor | wc -l`
CPUS=`cat /proc/cpuinfo | grep "physical id" | sort -u | wc -l`
MODEL=`cat /proc/cpuinfo | sed -nr '/model name/ s/.*:\s*(.*)/\1/p' | sort -u`
MEM=`lsmem | grep "online memory" | awk '{print $4}'`

if type "nvidia-smi" > /dev/null 2>&1; then
	GPU=`nvidia-smi -L`
else
	GPU="none"
fi

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
printf ", "
printf "\"GPU\" : \"$GPU\""
printf "}\n"
