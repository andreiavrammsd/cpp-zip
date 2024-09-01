#!/bin/bash

set -e

default_branch=$1 # master
event=$2 # pull_request|push
workspace=$3 # ..
build_path=$4 # build-dir
build_type=$5 # Debug
build_target=$6 # tests

echo $default_branch
echo $event
echo $workspace
echo $build_path=$4
echo $build_type=$5
echo $build_target=$6

if [ -z ${default_branch} ]; then
    echo "Branch is missing"
    exit 1
fi

cwd=$PWD

# Build with clang for compilation database used by clang-tidy
mkdir -p ${build_path}
cd ${build_path}

cmake ${workspace} \
    -DCMAKE_EXPORT_COMPILE_COMMANDS=ON \
    -DCMAKE_C_COMPILER=clang \
    -DCMAKE_CXX_COMPILER=clang++ \
    -DCMAKE_BUILD_TYPE=${build_type} \
    -DENABLE_TESTS=ON
cmake --build . --config ${build_type} --target ${build_target}

cd ${cwd}

if [ $event == "pull_request" ]; then
    echo git diff --name-only origin/$default_branch...HEAD
else
    echo git ls-files
fi
exit

# Find files
files=$(if [ $event == "pull_request" ]; then
    git diff --name-only origin/$default_branch...HEAD
else
    git ls-files
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
