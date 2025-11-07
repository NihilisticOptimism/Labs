#!/bin/bash
echo "give number"
read number
for (( i=1; i<=$number; i++ )); do
    t=$((number-i))
    echo "$t"
done
echo "done!"
