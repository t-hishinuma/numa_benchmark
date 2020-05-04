#!/bin/bash
IP=34.67.228.30

if [ $# != 1 ]; then
	echo "error! \$1: filename (result.yml)"
	exit
fi

ret=`curl --upload-file $1 $IP:443`

ret=`echo $ret | rev | cut -c 1-8 | rev`

echo upload done, your file tag is $ret
echo access http://$IP:80
