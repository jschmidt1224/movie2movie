#!/bin/bash

function wait_and_zip {
  pid=$1
  name=$2
  wait $pid
  echo "Unzipping $name"
  gunzip $name
}

echo Backup old files
mkdir bak/
mv *.tsv bak/
rm -f *.gz

echo Downloading Titles
curl -s -O https://datasets.imdbws.com/title.basics.tsv.gz &
tpid=$!
echo "tpid $tpid"


echo Downloading Names
curl -s -O https://datasets.imdbws.com/name.basics.tsv.gz &
npid=$!
echo "npid $npid"


echo Downloading Casts
curl -s -O https://datasets.imdbws.com/title.principals.tsv.gz &
ppid=$!
echo "ppid $ppid"

wait $tpid
echo Unzipping Titles
gunzip title.basics.tsv.gz &

wait $npid
echo Unzipping Names
gunzip name.basics.tsv.gz &

wait $tpid
echo Unzipping Casts
gunzip title.principals.tsv.gz &

wait

echo Removing old files
rm -r bak/