#!/bin/bash
# Dan Walkes
# 2014-01-29
# Call this script after configuring variables:
# version - the version of OpenCV to be installed
# downloadfile - the name of the OpenCV download file
# dldir - the download directory (optional, if not specified creates an OpenCV directory in the working dir)
if [[ -z "$version" ]]; then
    echo "Please define version before calling `basename $0` or use a wrapper like opencv_latest.sh"
    exit 1
fi
if [[ -z "$downloadfile" ]]; then
    echo "Please define downloadfile before calling `basename $0` or use a wrapper like opencv_latest.sh"
    exit 1
fi
if [[ -z "$dldir" ]]; then
    dldir=OpenCV
fi
if ! sudo true; then
    echo "You must have root privileges to run this script."
    exit 1
fi
set -e

echo "--- Installing OpenCV" $version

echo "--- Installing Dependencies"
source dependencies.sh

echo "--- Downloading OpenCV" $version
mkdir -p $dldir
cd $dldir
wget -O $downloadfile http://sourceforge.net/projects/opencvlibrary/files/opencv-unix/$version/$downloadfile/download

echo "--- Installing OpenCV" $version
echo $downloadfile | grep ".zip"
if [ $? -eq 0 ]; then
    unzip $downloadfile
else
    tar -xvf $downloadfile
fi
cd opencv-$version
mkdir build
cd build
cmake -D CMAKE_BUILD_TYPE=RELEASE -D CMAKE_INSTALL_PREFIX=/usr/local -D WITH_TBB=ON -D BUILD_NEW_PYTHON_SUPPORT=ON -D WITH_V4L=ON -D INSTALL_C_EXAMPLES=ON -D INSTALL_PYTHON_EXAMPLES=ON -D BUILD_EXAMPLES=ON -D WITH_QT=ON -D WITH_OPENGL=ON ..
make -j 4
sudo make install
sudo sh -c 'echo "/usr/local/lib" > /etc/ld.so.conf.d/opencv.conf'
sudo ldconfig
echo "OpenCV" $version "ready to be used"

##Non-free needs to be added
sudo add-apt-repository --yes ppa:xqms/opencv-nonfree
sudo apt-get update 
sudo apt-get install libopencv-nonfree-dev

##Ubuntu deps for video application
sudo apt-get install build-essential pkg-config qt4-qmake libqt4-dev libavformat-dev \
libavcodec-dev libavutil-dev libswscale-dev libasound2-dev libpulse-dev libjack-jackd2-dev \
libgl1-mesa-dev libglu1-mesa-dev libx11-dev libxfixes-dev libxext-dev libxi-dev g++-multilib \
libx11-6:i386 libxfixes3:i386 libglu1-mesa:i386