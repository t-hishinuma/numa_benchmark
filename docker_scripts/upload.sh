#!/bin/bash
IP=34.67.228.30

$FILE
if [ $# == 1 ]; then
	FILE=$1
else
	FILE=/work/result/result.yml
fi

echo $FILE

ret=`curl --upload-file $FILE $IP:443`

ret=`echo $ret | rev | cut -c 1-8 | rev`

echo your hash is $ret
echo access http://$IP:80
