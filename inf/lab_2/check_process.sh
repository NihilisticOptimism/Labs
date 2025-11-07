#!/bin/bash
echo "give process name"
read p_n
if pgrep "$p_n" > /dev/null; then
    echo "$p_n works!"
else
    "$p_n doesn't work"
fi


