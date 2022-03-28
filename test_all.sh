#!/bin/bash

NUM_PROCS=""

while getopts "j:" opt; do
    case $opt in
        j)  NUM_PROCS="$OPTARG"
            ;;
    esac
done
shift $(($OPTIND - 1))


function test_single()
{
  local file="$1"
  local result_file="$file.result.txt"

  if [ ! -f "$result_file" ]; then
    # run main
    local main_xml="$file.main.xml"
    #if [ ! -f "$main_xml" ]; then
      main/build/main $file
    #fi
    local main_t=`grep -o -a -m 1 -h -r "t val=.*" "$main_xml" | head -1 | sed -e 's/^.*="\([^"]*\)".*$/\1/'`
    #echo "  main_t: $main_t secs"

    local parallel_cgal_xml="$file.parallel-cgal.xml"
    #if [ ! -f "$parallel_cgal_xml" ]; then
      parallel-cgal/build/parallel-cgal $file
    #fi
    local parallel_cgal_t=`grep -o -a -m 1 -h -r "t val=.*" "$parallel_cgal_xml" | head -1 | sed -e 's/^.*="\([^"]*\)".*$/\1/'`
    #echo "  para_t: $parallel_cgal_t secs"

    if main/build/compare "$file" "$main_xml" "$parallel_cgal_xml" 1>/dev/null; then
      #echo "  success!"
      local result="success"
    else
      #echo "  failure! ☠☠☠☠☠☠☠☠☠☠☠☠☠☠☠☠☠☠☠☠☠☠☠"
      local result="failure"
    fi
    echo "$result,$main_t,$parallel_cgal_t">"$result_file"
  fi
}

function test_all()
{
  local list_file="$1"
  local worker_id="$2"
  local COUNTER=0
  local count=`wc -l <"$list_file" | sed 's/^ *//g'`
  sleep 0.1
  while read id; do
    let COUNTER++
    # remove trailing (newline) character
    local id=${id%?}
    # find file (could end in .stl, .obj, etc.)
    local pattern="$raw_meshes/$id.*"
    local files=( $pattern )
    local mesh_file=${files[0]}
    echo "$worker_id: $COUNTER/$count"
    test_single "$mesh_file" 
  done <"$list_file"
}


if [[ -d "$1" ]]
then
  pwn_file="random-pwn.txt"
  raw_meshes="$1"
  rm -f "$pwn_file.split.txt-"*
  split -a 4 -l$((`wc -l < $pwn_file`/$NUM_PROCS)) $pwn_file "$pwn_file.split.txt-"

  pids=()
  local worker_id=0
  for sp in random-pwn.txt.split.txt-*; do
    test_all $sp $worker_id &
    echo "kill $!"
    pids+=("$!")
    let worker_id++
  done
  echo "--------------------------------------------"
  for pid in "${pids[@]}"; do
    wait "$pid"
  done;
  echo "all done."
else
  test_single "$1"
fi

