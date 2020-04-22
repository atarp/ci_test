#!/bin/bash
#########################
# md5 test
#########################

if [ "$(../build/bin/md5)"  == "$(echo -n  'abcdefghijklmnopqrstuvwxyz'|md5sum|cut -d ' ' -f 1)" ] 
then
	exit 0; 
else
	exit 1;
fi

