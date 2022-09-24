#!/bin/bash

# Download ANTLR
echo "Downloading ANTLR..."

cd external
bash ./download_antlr.sh
cd ..

echo "ANTLR... downloaded!"
echo "Generating files..."

# Generate ANTLR files
cd ./src/grammar
bash generate_files.sh
cd ../../

echo "Files generated!"
echo "Now compile:"
echo "    mkdir build && cd build"
echo "    cmake .. && cmake --build ."




