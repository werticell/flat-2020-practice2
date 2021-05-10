mkdir cmake_build
cd cmake_build || return
cmake ..
cmake --build .
cd ..