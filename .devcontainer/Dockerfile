# Use the official Ubuntu base image
FROM ubuntu:22.04

# Avoid interactive package installation
ENV DEBIAN_FRONTEND=noninteractive

# Install required packages
RUN apt update && apt install -y \
    build-essential \
    cmake \
    clang-tidy \
    clang-format \
    lcov \
    git \
    curl \
    wget \
    unzip \
    python3 \
    python3-pip \
    gdb \
    lldb \
    clang \
    llvm \
    nano \
    software-properties-common \
    doxygen \
    graphviz \
    && rm -rf /var/lib/apt/lists/*

# Create a non-root user
ARG USERNAME=vscode
ARG USER_UID=1000
ARG USER_GID=$USER_UID

RUN groupadd -g $USER_GID $USERNAME || true \
    && useradd --uid $USER_UID --gid $USER_GID -m $USERNAME -s /bin/bash \
    && apt update \
    && apt install -y sudo \
    && echo $USERNAME ALL=\(ALL\) NOPASSWD:ALL > /etc/sudoers.d/$USERNAME \
    && chmod 0440 /etc/sudoers.d/$USERNAME

# History
RUN SNIPPET="export PROMPT_COMMAND='history -a' && export HISTFILE=/commandhistory/.bash_history" \
    && mkdir /commandhistory \
    && touch /commandhistory/.bash_history \
    && chown -R $USERNAME /commandhistory \
    && echo "$SNIPPET" >> "/home/$USERNAME/.bashrc"

    # Set the user to vscode
USER $USERNAME

# Set the workspace directory
WORKDIR /workspace
