# scop for 42cursus.

The goal of this project was to create a small program that shows a 3D object conceived with a modelization program like Blender. The 3D object is stored in a .obj file.

In a window, the 3D object is displayed in perspective, rotate on itself and can be moved on three axis, in both directions. 
A texture is applicable by pressing a key.

The project is in C++ and uses OpenGl. Shaders are in GLSL.
No librairies are allowed to load the 3D object, nor to make the matrixes or to load the shaders.

As there can be a lot of parameters in a .obj file, my project only fits with the simplest ones (vertices, textures, normals and faces coordinates).

---

### Prerequisites :wrench:
    
    sudo apt install freeglut3-dev

---

### Usage :desktop_computer:

First:

    git clone https://github.com/lleveque42/scop
    cd scop
    
Then:
    
    make

<p align="center">
    <img src="https://github.com/lleveque42/scop/blob/main/readme/42scopMake.gif">
</p>

And finally:

    ./scop <path_to_obj_file.obj>

:warning: If no file.obj is provided, 42.obj will be used as default. :warning:

<p align="center">
    <img src="https://github.com/lleveque42/scop/blob/main/readme/42scopLaunch.gif">
</p>

You can now interact with the object with theses keys :
- W: move object forward
- S: move object backward
- A: move object to the left
- D: move object to the right
- Left Shift: move object down
- Space: move object up
- T: change colors (colorful/gray/textured)

---

### Examples of use :nerd_face:

#### Robot :robot: :

<p align="center">
    <img src="https://github.com/lleveque42/scop/blob/main/readme/42scopRobotFinal.gif">
</p>

#### MrCatPC (i don't know why a hand is missing) :cat2: :

<p align="center">
    <img src="https://github.com/lleveque42/scop/blob/main/readme/42scopMrCatPC.gif">
</p>

#### Traffic Cone :warning: :

<p align="center">
    <img src="https://github.com/lleveque42/scop/blob/main/readme/42scopTrafficCone.gif">
</p>
