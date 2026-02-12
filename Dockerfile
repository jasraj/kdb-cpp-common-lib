FROM ubuntu:latest

RUN apt update
RUN apt upgrade -y
RUN apt install -y cmake libsystemd-dev g++ rpm

ENTRYPOINT /bin/bash
