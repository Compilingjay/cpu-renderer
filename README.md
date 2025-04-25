# CPU Renderer
An old simple CPU-based renderer built using pure C++ and SDL3. Only renders shape faces, edges, and vertices.

### Required Dependencies
- A C++20 compiler
- CMake

### Building
If you do not have SDL3 on your system: run the following at project root:
```
    git submodule update --init --recursive
```

Else, just run the following:
cmake . -B build
cmake --build build

To use the program:
cd build
./renderer_3d <window title/> <path to .obj file/>

You can rotate the models using WASDQE.

### Copyright
Code is (c) 2024 Compilingjay, All rights reserved.