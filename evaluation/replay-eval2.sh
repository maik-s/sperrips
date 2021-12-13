#!/bin/bash

eval_zip="eval2.zip"
chmod 644 "${eval_zip}"

config_file="config_repl2.json"

if [ ! -f "${eval_zip}" ] ; then 
	echo "This scripts requires ${eval_zip} in this ($(dirname "$0")) directory."
	echo "You can retrieve it with the record-eval2.sh script on another host"
	echo "Exiting"
	exit 1
fi

unzip -o "${eval_zip}"

for type in echo cat hostname ping ; do
    cat "${config_file}" | jq --arg command "eval_$type" '.command=$command' > config.json
    cp eval2/recording.bin.${type} /root/deb_diff/root/recording.bin
    sudo ./sperrips --replaying | tee eval2/repl_${type}.log
    mv -v deb_diff/root/differences.bin eval2/differences.bin.${type}
    mv -v deb_diff/root/output.log eval2/output.2.log.${type}
    echo
    echo
    echo
done
zip -r "${eval_zip}" eval2/ "${config_file}" "$0"
