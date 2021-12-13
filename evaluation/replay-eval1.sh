#!/bin/bash

scp kernel-playground-numero-dos:eval1.zip .
 
eval_zip="eval1.zip"
config_file="config_orig2.json"

if [ ! -f "${eval_zip}" ] ; then 
	echo "This scripts requires ${eval_zip} in this ($(dirname "$0")) directory."
	echo "You can retrieve it with the record-eval1.sh script on another host"
	echo "Exiting"
	exit 1
fi

unzip -o "${eval_zip}"

for type in cat echo hostname ping; do
    cat "${config_file}" | jq --arg command "eval_$type" '.command=$command' > config.json
    cp eval1/recording.bin.${type} /root/deb_diff/root/recording.bin
    sudo ./sperrips --replaying | tee eval1/repl_${type}.log
    mv -v deb_diff/root/differences.bin eval1/differences.bin.${type}
    mv -v deb_diff/root/output.log eval1/output.2.log.${type}
    echo
    echo
    echo
done
zip -r "${eval_zip}" eval1/ "${config_file}" "$0"
