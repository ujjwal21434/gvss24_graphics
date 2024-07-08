# Ray Tracing Engine in C++

## Description

This project is a ray tracing engine written in C++ that simulates realistic lighting and shadows in 3D scenes. It features:
- Ray-sphere and ray-plane intersection algorithms.
- Support for reflective, refractive, and diffusive objects.
- Ambient, diffuse, and specular lighting.
- Transparency and refraction using Snell's law and Schlick's approximation.
- Complex light and material interactions.

## Features

- **Ray Tracing**: Implements the core ray tracing algorithm for realistic rendering.
- **Reflection and Refraction**: Supports both reflective and refractive materials.
- **Lighting Models**: Includes ambient, diffuse, and specular lighting models.
- **Multiple Objects**: Handles scenes with multiple spheres and planes.
- **Shadows and Highlights**: Generates shadows and highlights based on object interactions.

## Dependencies

- C++11 or higher
- CMake 3.10 or higher

## Project Structure

```
.
├── src
│   ├── color.h
│   ├── ray.h
│   ├── vec3.h
│   ├── utility_functions.hpp
│   ├── sphere.hpp
│   ├── plane.hpp
│   └── sphere_raytracing.cpp
├── CMakeLists.txt
└── README.md
```

## Building the Project

### Step 1: Create a Build Directory

```sh
mkdir build
cd build
```

### Step 2: Run CMake

```sh
cmake ..
```

### Step 3: Build the Project

```sh
make
```

### Step 4: Run the Program

```sh
./sphere_raytracing
```

## Usage

The program generates a PPM image file representing the rendered scene. The image is output to the console in P3 format, which can be redirected to a file.

```sh
./sphere_raytracing > output.ppm
```

Open the `output.ppm` file with an image viewer that supports PPM format.

## Example Output

The generated image will include multiple spheres and planes with different materials and lighting effects, demonstrating reflections, refractions, and shadows.

![33](https://github.com/ujjwal21434/gvss24_graphics/assets/108088836/4849c7c1-41c5-41f6-9c18-c50a98049edf)
