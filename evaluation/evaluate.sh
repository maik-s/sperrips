#!/bin/bash


if [ -z "$1" ] || ( [ "$1" != "eval1" ] && [ "$1" != "eval2" ] ); then 
	echo "Usage ./"$0" eval1|eval2"
	echo "Please specify which evaluation you want to evaluate"
	exit 1 
fi

eval_zip="${1}.zip"

if [ ! -f "${eval_zip}" ] ; then
        echo "This scripts requires ${eval_zip} in this ($(dirname "$0")) directory."
        echo "You can retrieve it with the record-$1.sh script on another host"
        echo "Exiting"
        exit 1
fi

unzip -o "${eval_zip}"

for i in cat hostname echo ping ; do
python3 ../code/utils/protoprint.py --replaying -f "$1"/differences.bin.$i > "$1"/diff_$i.log
done
