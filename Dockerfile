FROM alpine:latest

RUN apk update && apk add \
    cmake \
    clang \
    clang-dev \
    g++ \
    ninja

COPY . /usr/src/SiMFS-Tk/SiMFS-core
WORKDIR /usr/src/SiMFS-Tk/SiMFS-core/build

RUN cmake .. -G Ninja 
RUN ninja
RUN ctest

LABEL Name=simfs-core Version=0.0.1
