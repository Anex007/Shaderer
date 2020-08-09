# Shaderer
A program to create and compile and shader programs. Often I needed a simple tool
to test and run shaders. Yes there is [Shadetoy](https://www.shadertoy.com/) but i needed something offline. This does support most of the inputs Shadertoy does.

### Future Plans
* Builtin Camera System
* Easy customisability for syntax highlighting and other functionalities in the Text Editor


![Mandlebrot](/Shaderer/assets/screenshots/mandlebrotshader.png)

### Textures
You can use upto 3 textures in your shader

![Texture support](/Shaderer/assets/screenshots/textures.gif)

## building
You need premake5 installed to build and compile this program.
First clone the repo including the submodules with 
```
git clone --recursive https://github.com/Anex007/Shaderer
```
then generate the build files
#### Windows
```
premake5 vs2019
```
Use visual studio to compile and run
#### Linux
```
premake5 gmake2
make
```