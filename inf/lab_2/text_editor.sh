#!/bin/bash
touch notes.txt
echo "text, plz"
while true; do
    read input
    if [ "$input" == "exit" ]; then
        break
    fi
    echo "$input" >> notes.txt
done

echo "contents:"
cat notes.txt
rm notes.txt
