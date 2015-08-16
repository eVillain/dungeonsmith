# dungeonsmith

Game engine experiments in C++

Using SDL 2.0.4 (x64 on Windows)
glm 0.9.7
FreeType 2.6
GLEW 1.13.0
LibPNG 1.6.18
Bullet Physics 2.83.4
zlib 1.2.8
pugiXML 1.6

Considering adding liblz4f for voxel compression

Renderer and GUI are currently in progress.
3D rendering is done into a G-buffer and lit in a deferred buffer.
2D elements are forward-rendered after the 3D post-processing pass.

Voxel meshes and instanced cubes are currently up and running along with a debug console.
TODO:
- Add console vars and more debug commands
- More 3D primitives for debugging
- Entity System
