Procedural Village Generation on Arbitrary Terrain v0.1

Arnaud Emilien (c) 2010
contact@wildmagegames.com

# License
This software can not be copied and/or distributed without the express permission of Arnaud Emilien.

# Requirements
GLSL 130
GL_EXT_texture_array

g++-5 with -std=c++11 (for variadic templates) 
(it compiles directly on Ubuntu 16.04, I was not able yet to compile it on 18.04)

# Dependencies
lib qt4 dev
lib glew dev

# Controls
forward/backward: middle mouse button
translate view: right mouse button
rotate view: left mouse button

# How to generate a basic village
1. File: Open heightmap, select an heightmap
2. Gen: Select a generation scenario, click Play secnario (you can repeat that step)
3. Build: click Watershed
4. Geom: click Generate Geometry

# TODO
- watershed vectorization from the watershed grid needs several improvements to provide more rectangular house shapes for houses clusters with close neighbors. A better watershed implementation, used for the publication, has been lost. Also, new papers have been made on procedural building layout generation from road networks.
- geometry: some buiding part generation have bugs. Meshs could use instancing for performances.
- improve camera controls.
- multi threaded implementation.

