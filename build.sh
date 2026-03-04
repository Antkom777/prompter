#!/bin/bash

# Build script for Prompter application

echo "Building Prompter..."

# Create build directory if it doesn't exist
if [ ! -d "build" ]; then
    mkdir build
fi

cd build

# Configure with CMake
echo "Configuring with CMake..."
cmake ..

if [ $? -ne 0 ]; then
    echo "CMake configuration failed!"
    exit 1
fi

# Build
echo "Compiling..."
if [ -f "build.ninja" ]; then
    ninja
else
    make
fi

if [ $? -ne 0 ]; then
    echo "Build failed!"
    exit 1
fi

echo "Build successful!"
echo "Run the application with: ./build/prompter"
