#!/bin/bash


echo "Container Up and Running"

# TODO: will have to switch users manually
# switch user to non root user created in dockerfile
# su dev

# keeps the container open and running with no server
tail -f /dev/null

# the how ^
# We can use the tail -f  command to read the /dev/null file. The command keeps looking for new changes in the file to 
# display, so it never ends as long as the file exists: