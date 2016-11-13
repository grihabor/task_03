# task_03

## How to use
```
sudo apt install freeglut3-dev libglew-dev libglu1-mesa-dev  
git clone https://github.com/grihabor/task_03
cd task_03
```
To build the project:
```
sudo make all
```
To run the project:
```
make run
```
To clean the project:
```
make clean
```
## Using CLion
To use with CLion you just need to open the project, choose __program__ as target and __task_03__ as executable. That's it!  
shaders/ and Texture/ folders are automatically copied to the target directory.

## Key hacks
[#define GL_CONTEXT_LOST 0x0507](https://github.com/grihabor/task_03/commit/7be0889be71614f861087246417cb86e90c3f4b2)  
[MESA_GL_VERSION_OVERRIDE=3.3COMPAT](https://github.com/grihabor/task_03/commit/7649a49026f82039be95898c165f5635f840b81e)  
