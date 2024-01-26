arch=$(uname -m)

mkdir build-$arch

cd build-$arch
rm * -rf
cmake ..

make clean
make

# unit test
test/test_xmedia


cd ..