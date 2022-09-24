#!/bin/bash

Compile the project
mkdir build
cd build
cmake ..
cmake --build .
cd ..

ln -s build/bin/app c-rust
chmod +x c-rust
