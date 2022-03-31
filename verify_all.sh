#!/bin/bash

NUM_PROCS="1"

while getopts "j:" opt; do
    case $opt in
        j)  NUM_PROCS="$OPTARG"
            ;;
    esac
done
shift $(($OPTIND - 1))

function verify_single()
{
  local file="$1"
  local result_file="$file.verify.txt"
  if [ ! -f "$result_file" ]; then
    local file="$1"
    local base="${file%%.result.txt}"
    local pres=`parallel-cgal/build/snap "$base".parallel-cgal.xml`
    local mres=`main/build/snap "$base".main.xml`
    local res="match"
    if [ "$pres" != "$mres" ]; then 
      res="disagree"
    fi
    echo "$res → $base"
    echo "$res $pres $mres" | tr ' ' ','  | tr '\n' ',' >"$result_file"
  fi
}

function verify_all()
{
  local list_file="$1"
  local COUNTER=0
  local count=`wc -l <"$list_file" | sed 's/^ *//g'`
  sleep 0.1
  while read id; do
    let COUNTER++
    # remove trailing (newline) character
    echo "$worker_id: $COUNTER/$count"
    verify_single "$id"
  done <"$list_file"
}

if [[ -d "$1" ]]
then
  tmp="./verify_all.txt"
  find $1 -name "*.result.txt" | sort -R >"$tmp"
  echo "NUM_PROCS: $NUM_PROCS"
  split -a 4 -l$((`wc -l < $tmp`/$NUM_PROCS)) "$tmp" "$tmp.split.txt-"

  pids=()
  worker_id=0
  for sp in $tmp.split.txt-*; do
    let worker_id++
    verify_all $sp $worker_id &
    echo "kill $!"
    pids+=("$!")
  done
  echo "--------------------------------------------"
  for pid in "${pids[@]}"; do
    wait "$pid"
  done;
  echo "all done."
else
  verify_single "$1"
fi


