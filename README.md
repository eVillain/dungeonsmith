# dungeonsmith

Game engine experiments in C++

Using SDL2, FreeType, GLEW, LibPNG and Zlib
Considering adding liblz4f for voxel compression, Bullet/Cu3e for physics

OSX Project included, VS Solution coming in the near-ish future (or feel free to roll your own).

Renderer and GUI are currently in progress.
The current implementation supports forward-rendering for 2D primitives and 2D text.
TODO: forward rendering of 3D primitives for debugging

Deferred rendering is 80% done, currently supporting voxel meshes and 3D text.
Still to be added on deferred rendering side:
Instanced objects, imposter spheres, sky dome with atmospheric absorption

Post-processing effects to add:
Deferred lighting
Lens simulation and depth-of-field
Light bloom
Screen-space ambient occlusion
Screen-space light-shafts (aka. god-rays)
