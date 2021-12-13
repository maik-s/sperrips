#!/bin/bash

CORRECT="Correct content"
WRONG="This is wrong content"
CONFIG_PATH="$(dirname $(realpath $0))/../../config.json"
SPERRIPS_PATH="$(dirname $(realpath $0))/../../sperrips"
workdir=$(jq -r '.workdir' "$CONFIG_PATH")
stdout_file=$(jq -r '.stdout_file' "$CONFIG_PATH")
outputfile=$workdir"/"$stdout_file
echo -n > "$outputfile"

echo "$CORRECT" > /tmp/catfile
jq '.command="cat3"' "$CONFIG_PATH" | sponge config.json

valgrind --error-exitcode=1 "$SPERRIPS_PATH" --recording 2>&1 > "valgrind-recording-cat-${CI_COMMIT_SHORT_SHA}-${CI_JOB_ID}.log" 2>&1
if [ $? -ne 0 ] ; then
    exit 1
fi

echo "$WRONG" > /tmp/catfile

valgrind  "$SPERRIPS_PATH" --replaying 2>&1 > "valgrind-replaying-cat-${CI_COMMIT_SHORT_SHA}-${CI_JOB_ID}.log" 2>&1
r=$?
echo $r
if ( [ $r -ne 2080 ] && [ $r -ne 32 ] ); then
    exit 1
fi

mv /home/recording.bin "${CI_PROJECT_DIR}/code/recording_cat.bin"
mv /home/differences.bin "${CI_PROJECT_DIR}/code/differences_cat.bin"
mv "$outputfile" "${CI_PROJECT_DIR}/code/output_cat.log"

grep -q "$CORRECT" "${CI_PROJECT_DIR}/code/output_cat.log"

exit $?
