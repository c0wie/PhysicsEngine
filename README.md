# pe2d physics engine written in cpp 

## Engine contains:
* broad phase collision detection implemented by dividing space into grid
* narrow phase collision detection implemented by SAT algorithm 
* many solvers to deal with collision
* easy to use interface

![image](https://github.com/user-attachments/assets/a3ead6a8-1d6a-4589-a9ef-c87c507674ab)


# What this repo contains?
* In `Tests` file there are unit test to engine
* In `Sample` file there is simple use case of pe2d
* In `pe2d` file there is engine itself

# How to build it?
You need to have installed `CMake` on your machine.
Then you can copy `pe2d file` to your project and add it as `static library`
to your `CMakeLists.txt`. From now every thing should work just fine but I only tested it on gcc compiler.
