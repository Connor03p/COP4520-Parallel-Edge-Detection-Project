FROM ubuntu:22.04

# update
RUN apt update -y

# install gcc/g++
RUN apt install build-essential -y

# TODO: need to install opencv
# TODO install other things needed as well

# make source dir
RUN mkdir /src
WORKDIR /src

# create non root user
RUN useradd dev

# copy the entrypoint script to src in container
COPY ./entrypoint.bash .

# run the commands inside of the entrypoint script
ENTRYPOINT ["bash","entrypoint.bash" ]