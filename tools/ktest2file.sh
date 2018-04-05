#!/bin/bash
# $1 = input .ktest file
# $2 = output binary file
echo -n -e `ktest-tool $1 | grep 'object    0: data:' | cut -d ' '  -f 7 | sed -e "s/'//g"` > $2
