#!/bin/bash
nc localhost 12345
read message
echo "$message" | nc localhost 12345
echo "message sent"
