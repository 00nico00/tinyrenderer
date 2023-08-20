# TinyRenderer: A Learning Project for Rasterization
This is a README.md file for TinyRenderer, a learning project for rasterization.

# Feature
This project is based on the TinyRenderer tutorial by Dmitry V. Sokolov.

+ Drawing lines and triangles
+ Filling triangles with colors and textures
+ Computing lighting and shading
+ Loading and rendering 3D models
+ Implementing a depth buffer
+ Applying perspective projection and camera transformations
+ Adding shadows and anti-aliasing
+ Using a simple library called TGAImage to create and save TGA images. The project does not use any external libraries or frameworks for graphics.

# Getting started
```bash
git clone https://github.com/00nico00/tinyrenderer.git
cd tinyrenderer
mkdir build
cd build
cmake ..
cmake --build .
tinyrenderer.exe
```

On linux or macOS
```bash
cd tinyrenderer
mkdir build
cd build
cmake ..
make
./tinyrenderer
```