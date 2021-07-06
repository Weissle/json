mkdir google_tmp
cd google_tmp
wget https://github.com/google/googletest/archive/refs/tags/release-1.11.0.zip -O googletest.zip
unzip googletest.zip
cd googletest-release-1.11.0
mkdir build
cd build
cmake ../
make
sudo make install
cd ../../..
rm -rf google_tmp/
