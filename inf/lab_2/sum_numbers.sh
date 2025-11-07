#!/bin/bash
echo "give 2 numbers"
if [ $# -ne 2 ]; then
    echo "liar!"
    exit 1
fi

sum=$(($1 + $2))
echo $sum
