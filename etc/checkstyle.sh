#!/bin/sh

astylerc=`dirname $0`/astylerc

( astyle --options=$astylerc < $1 ) | head -n -1 | diff - $1

