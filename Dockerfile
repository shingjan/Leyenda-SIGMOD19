FROM ubuntu:17.10

# Fix missing apt-utils.
RUN apt-get update && apt-get install -y --no-install-recommends apt-utils

# Update packages.
RUN apt-get update && apt-get dist-upgrade -y

# Install allowed packages.
RUN apt-get install -y apt-utils \
                       curl \
                       autoconf=2.69-11 \
                       automake=1:1.15-6ubuntu1 \
                       cmake=3.9.1-1 \
                       gcc=4:7.2.0-1ubuntu1 \
                       gccgo=4:7.2.0-1ubuntu1 \
                       libjemalloc-dev=3.6.0-10 \
                       libboost-dev=1.62.0.1 \
                       clang-5.0=1:5.0-3 \
                       libtbb-dev=2017~U7-6 \
		       build-essential=12.4ubuntu1 

# Copy files to container
ADD src /src
ADD gensort /src/gensort

# Set work directory
WORKDIR /src

# Compile code
RUN ./compile.sh

