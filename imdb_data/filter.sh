#!/bin/bash

tt='tt[0-9]*'
nm='nm[0-9]*'
tab=$'\t'
movie='movie'
title="[a-zA-Z0-9 ,-_&%\$\#@!\.\(\)':\"\]*"
z="0"
ds="[0-9]*"
roles="(actor|actress|director)"
echo "Filtering data"
grep "$tt	$movie	" title.basics.tsv | grep "	0	" > title.basics.movie.tsv
grep -E "$tt$tab$ds$tab$nm$tab$roles$tab" title.principals.tsv > title.principals.roles.tsv

