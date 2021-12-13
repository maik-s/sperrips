#!/bin/bash

mypath="$0"
orig_config="config_orig1.json"

for type in cat echo hostname ping; do
    cat "${orig_config}" | jq --arg command "eval_$type" '.command=$command' > config.json
    sudo ./sperrips --recording | tee eval1/rec_${type}.log
    mv -v deb_diff/root/recording.bin eval1/recording.bin.${type}
    mv -v deb_diff/root/output.log eval1/output.1.log.${type}
    echo
done
zip -r eval1.zip eval1/ "${mypath}" "${orig_config}"
