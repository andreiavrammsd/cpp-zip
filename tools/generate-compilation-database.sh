#!/bin/bash
set -e

workspace=$1 # ..
build_path=$2 # build-dir
build_type=$3 # Debug
build_target=$4

cwd=$PWD

mkdir -p ${build_path}
cd ${build_path}

# Build with clang for compilation database used by clang-tidy
cmake ${workspace} \
    -DCMAKE_EXPORT_COMPILE_COMMANDS=ON \
    -DCMAKE_C_COMPILER=clang \
    -DCMAKE_CXX_COMPILER=clang++ \
    -DCMAKE_BUILD_TYPE=${build_type} \
    -DENABLE_TESTS=ON \
    -DENABLE_BENCHMARKS=ON

cd ${cwd}
