#!/bin/bash

tt='tt[0-9]*'
nm='nm[0-9]*'
tab=$'\t'
movie='movie'
title="[a-zA-Z0-9 ,.'\]*"
z="0"
ds="[0-9]*"
roles="(actor|actress|director)"

grep "$tt$tab$movie$tab$title$tab$title$tab$z$tab" title.basics.tsv > title.basics.movie.tsv
grep -E "$tt$tab$ds$tab$nm$tab$roles$tab" title.principals.tsv > title.principals.roles.tsv