# 3D Raycaster


A simple non DDA based 3D Raycaster built in C using the SDL3 Windowing Library.  Uses the [parametric line intersection formula](https://en.wikipedia.org/wiki/Line%E2%80%93line_intersection) instead of DDA to cast rays


## Video Demo and screenshots
<video src="https://github.com/user-attachments/assets/001436e2-67ac-47b5-b4c6-b7131c8574e3" controls="controls" style="max-width: 600px;">
</video>

<br>

| Retro Texture | Pixel Art Texture |
| :---: | :---: |
| <img width="1284" height="713" alt="Image" src="https://github.com/user-attachments/assets/0f330759-8275-40e0-acec-9740e2f320ba" /> | <img width="1281" height="721" alt="Image" src="https://github.com/user-attachments/assets/5962169d-d644-4989-83c1-4e3c8a6cf95b" /> |

<br>

## Info
* WASD or arrow keys movement 
* Themes can be changed by changing the value of the ```THEME``` macro in ```include/constants.h``` to either 0 1 or 2.


## Prerequisites
1. CMake (v3.15 or higher)

2. A C Compiler:
* Linux: gcc (via build-essential)

* Windows: Visual Studio (with C++ workflow) or MinGW

* macOS: Xcode Command Line Tools

3. SDL3 Development Libraries - [Install Instructions](https://github.com/libsdl-org/SDL/blob/main/INSTALL.md)

> [!NOTE]
> On Ubuntu, all dependencies can be installed by running:
> ```sudo apt update && sudo apt install -y build-essential cmake libsdl3-dev```




## Building the Game
1. Clone the repository and cd into it:
   ```sh
    git clone https://github.com/logorhythmic/3D-Raycaster.git && cd 3D-Raycaster
   ```

2. Make the build directory:
   ```sh  
	cmake -B build  
   ```

3. Compile and run the Raycaster:
   ```sh
   cmake --build build --config Release  
   ```

## Assets
1. [Light Texture](https://opengameart.org/content/tiny-texture-pack-1)
2. [Retro Texture](https://aquilarius.itch.io/aquilariusrt)
3. [Pixel Art Texture](https://normalmap-games.itch.io/pixelated-textures-asset-pack?download)


## Resources
1. Permadi Raycasting Tutorial - https://permadi.com/1996/05/ray-casting-tutorial-table-of-contents/
2. The Coding Train -  2D Raycasting - https://youtu.be/TOEi6T2mtHo

