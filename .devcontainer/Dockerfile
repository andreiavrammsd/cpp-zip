# Use the official Ubuntu base image
FROM ubuntu:26.04

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
    lcov \
    lldb \
    clang \
    llvm \
    nano \
    software-properties-common \
    doxygen \
    graphviz \
    sudo \
    && apt-get clean && rm -rf /var/lib/apt/lists/*

# Create a non-root user
ARG USERNAME=vscode
ARG USER_UID=1000
ARG USER_GID=$USER_UID

RUN usermod --login ${USERNAME} --home /home/${USERNAME} --move-home $(getent passwd ${USER_UID} | cut -d: -f1) \
    && usermod --gid ${USER_GID} ${USERNAME} \
    && passwd -d ${USERNAME} \
    && printf '%s ALL=(ALL) NOPASSWD:ALL\n' "${USERNAME}" > "/etc/sudoers.d/${USERNAME}" \
    && chmod 0440 "/etc/sudoers.d/${USERNAME}" \
    && visudo -cf "/etc/sudoers.d/${USERNAME}"

# History
RUN SNIPPET="export PROMPT_COMMAND='history -a' && export HISTFILE=/commandhistory/.bash_history" \
    && mkdir /commandhistory \
    && touch /commandhistory/.bash_history \
    && chown -R $USERNAME /commandhistory \
    && echo "$SNIPPET" >> "/home/$USERNAME/.bashrc"

# Git completion
RUN echo ". /usr/share/bash-completion/completions/git" >> "/home/$USERNAME/.bashrc"

# Set the user to vscode
USER $USERNAME

# Set the workspace directory
WORKDIR /workspace
