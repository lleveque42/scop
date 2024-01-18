The goal of this project was to create a small program that show a 3D object conceived with a modelization program like Blender. The 3D object is stored in a .obj file.

In a window, the 3D object is displayed in perspective, rotate on itself and can be moved on three axis, in both directions. 
A texture is applicable by pressing a key.

The project is in C++ and uses OpenGl. Shaders are in GLSL.
No librairies are allowed to load the 3D object, nor to make the matrixes or to load the shaders.

As there can be a lot of parameters in a .obj file, my project only fits with the simplest ones (vertices, textures, normals and faces coordinates).
