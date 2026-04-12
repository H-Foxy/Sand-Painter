# __Sand-Painter__

## Requirements
- CMake (3.28+)
- Git
- Visual Studio 2022 (Desktop development with C++)
  - MSVC v143 toolset
  - Windows 10/11 SDK

## Building the project
### 1. Build SFML project

This is neccessary to create the dynamic libraries used by __Sand_Painter__ project.

From inside `external` directory run:

- `cmake -S SFML -B SFML/build -DBUILD_SHARED_LIBS=TRUE`  
  Configure the CMake project to build SFML as dynamic libraries.

- `cmake --build SFML/build --config Release` (or `Debug`)  
  Build SFML in the selected configuration.

### 2. Build Sand-Painter project

Ensure SFML is built first. Then, build Sand-Painter targeting x64 platform.

