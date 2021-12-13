#!/bin/bash

CORRECT="Correct content"
WRONG="This is wrong content"
CONFIG_PATH="$(dirname $(realpath $0))/../../config.json"
SPERRIPS_PATH="$(dirname $(realpath $0))/../../sperrips"
workdir=$(jq -r '.workdir' "$CONFIG_PATH")
stdout_file=$(jq -r '.stdout_file' "$CONFIG_PATH")
outputfile=$workdir"/"$stdout_file
testjobname=echo
echo -n > "$outputfile"

jq '.command="echo"' "$CONFIG_PATH" | sponge "$CONFIG_PATH"

jq ".commands.echo.args[1]=\"$CORRECT\"" "$CONFIG_PATH" | sponge "$CONFIG_PATH"

valgrind --error-exitcode=1 "$SPERRIPS_PATH" --recording 2>&1 > "valgrind-recording-${testjobname}-${CI_COMMIT_SHORT_SHA}-${CI_JOB_ID}.log" 2>&1
if [ $? -ne 0 ] ; then
    exit 1
fi

jq ".commands.echo.args[1]=\"$WRONG\"" "$CONFIG_PATH" | sponge "$CONFIG_PATH"

valgrind "$SPERRIPS_PATH" --replaying 2>&1 > "valgrind-replaying-${testjobname}-${CI_COMMIT_SHORT_SHA}-${CI_JOB_ID}.log" 2>&1
if [ $? -ne 0 ] ; then
    exit 1
fi

mv /home/recording.bin "${CI_PROJECT_DIR}/code/recording_${testjobname}.bin"
mv /home/differences.bin "${CI_PROJECT_DIR}/code/differences_${testjobname}.bin"
mv "$outputfile" "${CI_PROJECT_DIR}/code/output_${testjobname}.log"

grep -q "$CORRECT" "${CI_PROJECT_DIR}/code/output_${testjobname}.log"

exit $?