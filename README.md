# pe2d
### Physics Engine written in C++

# What pe2d can do?
-   **Broad-phase collision detection**

-   **Square and circle collision resolution**

-   **Rotational physics simulation**

-   **Kinematics simulation** 

![image_2025-03-16_200228234](https://github.com/user-attachments/assets/cbe79c0d-cfda-4bb0-87a7-169bfde287b4)

# Build
Install CMake at least version 3.14
Tested only on Linux Ubuntu

* ### Building whole project
	1. Download whole project
		* `git clone https://github.com/c0wie/PhysicsEngine`
		*  `cd PhysicsEngine`
	
	2. Configure the build
		* `cmake -B build -DBUILD_TESTS=ON -DCMAKE_BUILD_TYPE=Release` 
	
	3. Build and run 
		*  `cmake --build build`
		* `./build/demo`
 * ### Building only pe2d library
	 1. Download the library
		 * download the [.zip](https://github.com/c0wie/PhysicsEngine/archive/refs/heads/master.zip)
		 * extract **pe2d** folder 
	
	2. Include it in your CMake project 
		 * add it to your project as any other static library in CMake 

# Testing
To run Unit Tests you  have to build [whole project](#building-whole-project) and set the ***BUILD_TESTS*** flag to ***ON***.

1. Go to project directory 
	
	* `cd path/to/project/repo/PhysicsEngine`

2. Run commands
	
	*  `cd build`
	*  `ctest`

# Any problems?

Please create an issue and I'll try to help you with your problem
