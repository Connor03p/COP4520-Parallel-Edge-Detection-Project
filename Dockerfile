FROM ubuntu:22.04

# switch to bash
SHELL ["/bin/bash", "-c"]

# update
RUN apt update && apt install -y cmake g++ wget unzip

# install gcc/g++
RUN apt install build-essential gdb -y
RUN apt install sudo -y

# install opencv
# Reason why We can't just apt install libopencv-dev --> https://stackoverflow.com/questions/44331836/apt-get-install-tzdata-noninteractive
RUN echo 'libopencv-dev libopencv-dev/Areas select Europe' | debconf-set-selections
RUN echo 'libopencv-dev libopencv-dev/Zones/Europe select Paris' | debconf-set-selections
RUN DEBIAN_FRONTEND="noninteractive" apt install -y libopencv-dev

# make source dir
RUN mkdir /src
WORKDIR /src/

# copy the entrypoint script to src in container
WORKDIR /src
COPY ./entrypoint.bash .

# run the commands inside of the entrypoint script
ENTRYPOINT ["bash","entrypoint.bash" ]