#!/bin/bash

set -e

default_branch=$1 # master
event=$2 # pull_request|push
workspace=$3 # ..
build_path=$4 # build-dir
build_type=$5 # Debug
build_target=$6 # tests

echo default_branch=$default_branch
echo event=$event
echo workspace=$workspace
echo build_path=$build_path
echo build_type=$build_type
echo build_target=$build_target

if [ -z ${default_branch} ]; then
    echo "Default branch is missing"
    exit 1
fi

./generate-compilation-database.sh ${workspace} ${build_path} ${build_type} ${build_target}

# Find files
files=$(if [ $event == "pull_request" ]; then
    git diff --name-only origin/"${default_branch}"...HEAD include
else
    git ls-files --directory include
fi | grep '\.*pp$' || true)

# Run tools
echo

if [ ! -z "${files}" ]; then
    file_count=$(echo "${files}" | wc -w)
    
    echo "Format ${file_count} file(s)"
    clang-format -i $files
    
    git diff --exit-code
    echo
    
    echo "Tidy ${file_count} file(s)"
    clang-tidy -p ${build_path} $files
else
    echo "No files for code quality"
fi
