echo "install cuda sdk first"
sudo apt update
sudo apt upgrade

sudo apt install build-essential cmake pkg-config unzip yasm git checkinstall
sudo apt install libjpeg-dev libpng-dev libtiff-dev

sudo apt install libavcodec-dev libavformat-dev libswscale-dev libavresample-dev
sudo apt install libgstreamer1.0-dev libgstreamer-plugins-base1.0-dev
sudo apt install libxvidcore-dev x264 libx264-dev libfaac-dev libmp3lame-dev libtheora-dev 
sudo apt install libfaac-dev libmp3lame-dev libvorbis-dev

sudo apt install libopencore-amrnb-dev libopencore-amrwb-dev

sudo apt-get install libdc1394-22 libdc1394-22-dev libxine2-dev libv4l-dev v4l-utils
cd /usr/include/linux
sudo ln -s -f ../libv4l1-videodev.h videodev.h
cd ~

sudo apt-get install libgtk-3-dev

sudo apt-get install python3-dev python3-pip
sudo -H pip3 install -U pip numpy
sudo apt install python3-testresources

sudo apt-get install libtbb-dev

sudo apt-get install libatlas-base-dev gfortran

sudo apt-get install libprotobuf-dev protobuf-compiler
sudo apt-get install libgoogle-glog-dev libgflags-dev
sudo apt-get install libgphoto2-dev libeigen3-dev libhdf5-dev doxygen


cd ~
wget -O opencv.zip https://github.com/opencv/opencv/archive/4.2.0.zip
wget -O opencv_contrib.zip https://github.com/opencv/opencv_contrib/archive/4.2.0.zip
unzip opencv.zip
unzip opencv_contrib.zip

echo "Create a virtual environtment for the python binding module"
sudo pip install virtualenv virtualenvwrapper
sudo rm -rf ~/.cache/pip
echo "Edit ~/.bashrc"
export WORKON_HOME=$HOME/.virtualenvs
export VIRTUALENVWRAPPER_PYTHON=/usr/bin/python3
source /usr/local/bin/virtualenvwrapper.sh
mkvirtualenv cv -p python3
pip install numpy

echo "Procced with the installation"
cd opencv-4.2.0
mkdir build
cd build

cmake -D CMAKE_BUILD_TYPE=RELEASE\
-D WITH_OPENGL=ON \
-D BUILD_EXAMPLES=ON \
-D WITH_TBB=ON \
-D OPENCV_DNN_CUDA=ON \
-D BUILD_CUDA_STUBS=ON \
-D WITH_CUDA=ON \
-D OPENCV_EXTRA_MODULES_PATH=~/opencv_contrib-4.2.0/modules \
-D BUILD_OPENCV_WORLD=ON \
-D ENABLE_FAST_MATH=ON \
-D OPENCV_ENABLE_NONFREE=ON ..

#Cache file:
#CMAKE_BUILD_TYPE:STRING=Release
#WITH_OPENGL:BOOL=1
#BUILD_EXAMPLES:BOOL=1
#WITH_TBB:BOOL=1
#OPENCV_DNN_CUDA:BOOL=1
#BUILD_CUDA_STUBS:BOOL=0
#WITH_CUDA:BOOL=1
#OPENCV_EXTRA_MODULES_PATH:PATH= ~/opencv_contrib-4.2.0/modules
#BUILD_opencv_world:BOOL=1
#ENABLE_FAST_MATH:BOOL=1
#OPENCV_ENABLE_NONFREE:BOOL=1


make -j4
sudo make install -j4

sudo /bin/bash -c 'echo "/usr/local/lib" >> /etc/ld.so.conf.d/opencv.conf'
sudo ldconfig
#source: https://gist.github.com/raulqf/f42c718a658cddc16f9df07ecc627be7