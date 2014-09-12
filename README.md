TestEngine
==========

This represents the groundwork for a larger project I hope to start soon. It's the beginning of a simple game
engine using C++ and OpenGL. At the moment the following is working:

- 3D Collision (hard and soft)
- 3D Texture Mapping
- Basic Player Based Event Handling
- Gravity and other "physically based" movement
- Simple non-player entities with potential based AI/movement
- Geometry from SVG paths (work in progress)
- Simple Skinning (work in progress)

The next big step is to work on art and music. Ideally I could remove SDL from the picture (which at the moment
starts the OpenGL context and does Event Handling, image loading and sound) and instead use GLFW for OpenGL, OpenAL for audio, and something else for event handling. However, at the moment it's a pretty attractive package. 
