echo "Start to Compile AlgoSDK......"

tool=/opt/rv1126_rv1109_sdk/buildroot/output/rockchip_face_board/host/share/buildroot/toolchainfile.cmake
export PATH=/opt/rv1126_rv1109_sdk/buildroot/output/rockchip_face_board/host/bin:$PATH

export ARCH=arm
export CROSS_COMPILE=arm-linux-gnueabihf-
set -e

workdir=$(cd $(dirname $0); pwd)
install=$workdir/install

rm -rf build-rv1126
mkdir build-rv1126
cd build-rv1126
cmake .. -DCMAKE_BUILD_TYPE=Release \
         -DPLATFORM_RV1126=1 \
         -DCMAKE_INSTALL_PREFIX=/algosdk \
         -DCMAKE_TOOLCHAIN_FILE=$tool
make -j4
make install DESTDIR=$install/rv1126
