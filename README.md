# 3D Linear Algebra Visualizer
![](https://img.shields.io/badge/c++-17-blue)
![](https://img.shields.io/badge/SFML-2.5.1-red)

## Building on Windows

### Requirements
- CMake 3.16 or newer
- A MinGW-w64 toolchain
- Git

Make sure CMake, MinGW, Git, and g++ are all on your path

### Build
- git clone https://github.com/liamdpearson/3DLinAlgVisualizer.git
- cd 3DLinAlgVisualizer
- cmake -S . -B build -G "MinGW Makefiles"
- cmake --build build
- ./visualizer
