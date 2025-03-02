#!/bin/bash

# Create a build directory
mkdir -p build
cd build

# Build the project
cmake ..
make

# Install the binary system-wide
sudo make install

echo "pbf installed successfully. You can now use it as a command."
