#!/bin/sh　　　　         以下的代码由/bin/sh 来解释

echo "Start to Compile AlgoSDK......"

workdir=$(cd $(dirname $0); pwd)
install=$workdir/install

cd $workdir

config="$1"
echo "config=$config"
if [ "$config" = "debug" ];
    then echo "build debug"
    rm -r build-debug
    mkdir build-debug
    cd build-debug
    cmake .. -DCMAKE_BUILD_TYPE=Debug \
             -DNOROS=1 \
             -DCMAKE_INSTALL_PREFIX=/algosdk/ 
    make -j6 
    #make install DESTDIR=$install/debug
    cd ..

     #copy 
    echo "copy dependencies"
    cp ./config/* ./build-debug/demo

else
    echo "build release"
    rm -r build-release
    mkdir build-release
    cd build-release
    cmake .. -DCMAKE_BUILD_TYPE=Release \
             -DNOROS=1 \
             -DCMAKE_INSTALL_PREFIX=/algosdk/ 
    make -j6 
    #make install DESTDIR=$install/release
    cd ..

     #copy 
    echo "copy dependencies"
    cp ./config/* ./build-release/demo

fi