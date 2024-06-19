ARG VARIANT="18.04"
FROM ubuntu:${VARIANT}

RUN apt-get update && export DEBIAN_FRONTEND=noninteractive \
    && apt-get -y install --no-install-recommends ca-certificates software-properties-common \
    git curl wget ninja-build build-essential python3-pip \
    pkg-config zip unzip tar iputils-ping ccache

RUN wget -O cmake.sh https://github.com/Kitware/CMake/releases/download/v3.23.3/cmake-3.23.3-linux-x86_64.sh && \
    sh cmake.sh --prefix=/usr/local/ --exclude-subdir && rm -rf cmake.sh

RUN apt-get update && apt-get -y install gpg-agent

RUN wget -O - https://apt.llvm.org/llvm-snapshot.gpg.key | apt-key add -

ARG LLVM_VERSION=16
RUN wget https://apt.llvm.org/llvm.sh && chmod +x llvm.sh && ./llvm.sh ${LLVM_VERSION} all

ENV PATH="${PATH}:/usr/lib/llvm-${LLVM_VERSION}/bin"

ENV SHELL /bin/bash

RUN pip3 install -U pip
RUN pip3 install bump2version

ARG USERNAME=vscode
ARG USER_UID=1000
ARG USER_GID=$USER_UID

# Create the user
RUN groupadd --gid $USER_GID $USERNAME \
    && useradd --uid $USER_UID --gid $USER_GID -m $USERNAME

RUN groupmod --gid $USER_GID $USERNAME \
    && usermod --uid $USER_UID --gid $USER_GID $USERNAME \
    && chown -R $USER_UID:$USER_GID /home/$USERNAME
