# dungeonsmith

Game engine experiments in C++

Using SDL2
FreeType
GLEW
LibPNG
Bullet Physics 2.83.4
Zlib

Considering adding liblz4f for voxel compression

Renderer and GUI are currently in progress.
3D rendering is done into a G-buffer and lit in a deferred buffer.
2D elements are forward-rendered after the 3D post-processing pass.

Voxel meshes and instanced cubes are currently up and running along with a debug console.
TODO: 
- Add console vars and more debug commands
- More 3D primitives for debugging
- Entity System