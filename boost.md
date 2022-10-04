Download 
https://www.boost.org/users/history/version_1_80_0.html

Decompress and compile
```
tar -xvzf boost_1_80_0.tar.gz
cd boost_1_80_0/
./bootstrap.sh --prefix=. 
./b2 -std=C++11
./b2  headers
./b2 install



./bootstrap.sh --prefix=. --with-libraries=filesystem thread
./b2 --with-filesystem --with-thread
./b2 install
```

At this point the subdirectory `boost/boost_1_80_0/stage/lib` contains the files `libboost_thread.a` and `libboost_filesystem.a`.

## Example

The script `example_boost.cpp` is compiled with
```
c++ -std=c++11 example_boost.cpp -o example_boost  -I ./boost/boost_1_80_0/  ./boost/boost_1_80_0/stage/lib/libboost_filesystem.a
```
