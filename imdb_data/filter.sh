#!/bin/bash

num='tt[0-9]*'
tab=$'\t'
movie='movie'
title="[a-zA-Z0-9 ,.'\]*"
z="0"

# grep "$num" title.basics.tsv
grep "$num$tab$movie$tab$title$tab$title$tab$z$tab" title.basics.tsv > title.basics.movie.tsv