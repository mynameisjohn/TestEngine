TestEngine
==========

This represents the groundwork for a larger project I hope to start soon. It's the beginning of a simple game
engine using C++ and OpenGL. At the moment the following is working:

- 3D Collision (hard and soft)
- 3D Texture Mapping
- Basic Player Based Event Handling
- Gravity and other "physically based" movement
- Simple non-player entities

The next step is to work on art, music, and AI. I would also like to remove SDL from the picture (which at the moment
starts the OpenGL context and does Event Handling and sound) and instead use GLFW for OpenGL, OpenAL for audio, and something
else for event handling. 

I'd also like to us libRSVG to load in SVG files, rasterize them to textures, and generate OpenGL
geometry from the SVG files. From there I'd like to implement a basic skinning algorithm. 
