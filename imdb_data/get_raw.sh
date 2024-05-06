#!/bin/bash

function download_unzip {
	url=$1
	name=$(echo $url | awk 'BEGIN { FS="/" } { print $NF }')
	echo $name
}

echo Backup old files
mkdir bak/
mv *.tsv bak/
rm -f *.gz

echo Downloading Titles
download_unzip https://datasets.imdbws.com/title.basics.tsv.gz
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

