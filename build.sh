#!/bin/bash

set -x
SOURCE_DIR=`pwd`
BUILD_DIR=${SOURCE_DIR}/build
TARGET_NAME="CAnalysis"


function Help(){
    echo "Usage:"
    echo "    $0 build (default build)"
    echo "    $0 build"
    echo "    $0 clean"
}
function Build(){
    if [ ! -d "$BUILD_DIR" ]; then
        mkdir "$BUILD_DIR"
    fi
    cd $BUILD_DIR 
    cmake $SOURCE_DIR 
    make -j`nproc`
    cp $BUILD_DIR/$TARGET_NAME $SOURCE_DIR/$TARGET_NAME
}
function Clean(){
    if [ -d "$BUILD_DIR" ]; then
        cd "$BUILD_DIR"
        make clean
    fi
    rm -rf $BUILD_DIR
    rm $SOURCE_DIR/$TARGET_NAME
}


if [ $# -eq 0 ] ; then
    Build
elif [ $# -eq 1 ] ; then
    if [ $1 = 'build' ] ; then 
        Build 
    elif [ $1 = 'clean' ] ; then
        Clean
    else 
        Help $0
    fi
else 
    Help $0
fi

cd $SOURCE_DIR


