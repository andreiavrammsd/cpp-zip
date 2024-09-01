#!/bin/bash

set -e

default_branch=$1 # master
workspace=$2 # ..
build_path=$3 # build
build_type=$4 # Debug
build_target=$5 # tests

if [ -z ${default_branch} ]; then
    echo "Branch is missing"
    exit 1
fi

cwd=$PWD

# Build with clang for compilation database used by clang-tidy
mkdir ${build_path}
cd ${build_path}

cmake ${workspace} \
    -DCMAKE_EXPORT_COMPILE_COMMANDS=ON \
    -DCMAKE_C_COMPILER=clang \
    -DCMAKE_CXX_COMPILER=clang++ \
    -DCMAKE_BUILD_TYPE=${build_type} \
    -DENABLE_TESTS=ON
cmake --build . --config ${build_type} --target ${build_target}

cd ${cwd}

# Find files
files=$(if [ "$(git rev-parse --abbrev-ref HEAD)" = ${default_branch} ]; then
    git ls-files
else
    git diff --name-only ${default_branch}
fi | grep '\.*pp$')

# Run tools
if [ ! -z "${files}" ]; then
    echo clang-format -i $files
    clang-format -i $files

    git diff --exit-code

    echo clang-tidy -p ${build_path} $files
    clang-tidy -p ${build_path} $files
else
    echo "No files changed"
fi

