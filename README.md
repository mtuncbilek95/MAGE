# M.A.G.E
Modern & Advanced Graphics Engine

## BEFORE YOU START
You need to download Vulkan SDK in order to run the engine from source code. There are couple additional features we need to run the game engine while installing Vulkan.

### On Windows
![image](https://github.com/user-attachments/assets/f509e9d6-8a85-4ed6-94ba-51fbe80b6caa)

### On Linux
Requested dependencies
```shell
sudo apt update && sudo apt upgrade -y

sudo apt install build-essentials g++ gcc cmake cmake-extra-modules libxi-dev libxcursor-dev libxinerama-dev libxrandr-dev waylandpp-dev libwayland-server0 libwayland-client++1 libuuid1 uuid-dev -y
```

## GOOD TO KNOW WHILE WORKING

- The Engine is written in C++20 and uses Vulkan API for rendering.
- The Engine is still in development and not ready for production.

- While running build generator for cmake, you will see a part where the cmake shows the excluded files. When working with Linux the excluded files needs to belong Win32API and 
vice versa. This has been added to CMake with the aim of reducing macro controllers and boosting the compilation speed.
