#!/bin/bash

while [ "$1" != "" ]; do 
    case $1 in 
        -c | --clear )      shift
                            rm -rf ./build
                            echo "clear"
                            ;;
        -r | --release )    shift
                            export params="config=release"
                            echo "release"
                            ;;
        * )                 shift
    esac
done

cd ./buildscript/conan

if [[ "$OSTYPE" == "linux-gnu"* ]]; then
    conan install . --build=missing --profile=./profiles/profile_linux.txt
elif [[ "$OSTYPE" == "darwin"* ]]; then
    conan install . --build=missing --profile=./profiles/profile_mac.txt
fi

cd ..
source ./build.sh
