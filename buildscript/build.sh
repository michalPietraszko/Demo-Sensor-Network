#!/bin/bash

cd "$(dirname "$0")"
cd ./premake

if [[ "$OSTYPE" == "linux-gnu"* ]]; then
    ./premake5_linux gmake2
elif [[ "$OSTYPE" == "darwin"* ]]; then
    ./premake5_mac gmake2
fi

cd ../../build

if [ ! -z "$params" ]
then
    make "$params"
else
    make 
fi
