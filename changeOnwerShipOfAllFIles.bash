#!/bin/bash
# The docker container that we remote into has root privilegs
# Meaning every file/folder it creates is lock to only root can acess them
# This is a problem when trying the edit someting on the host machine and you are not able to
# so to fix that run this script that will recusively change the permission of all files/folders
# in this root directory

# can circumvent this by changing user to something else when remoting into container

sudo chmod -R 777 .
