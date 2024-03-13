#!/bin/bash


# TODO: will have to switch users manually
# switch user to non root user created in dockerfile
# su dev


echo "Container Up and Running"

# Do this to check and verify installation
dpkg -l libopencv-dev

# keeps the container open and running with no server
# We can use the tail -f  command to read the /dev/null file. The command keeps looking for new changes in the file to 
# display, so it never ends as long as the file exists:
tail -f /dev/null