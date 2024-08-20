# M.A.G.E
Modern & Advanced Graphics Engine

## REQUESTED DEPENDENCIES ON LINUX (W.I.P by [Mehmet Samet Hakut](https://github.com/hakut))
```shell
sudo apt update && sudo apt upgrade -y

sudo apt install build-essentials g++ gcc cmake cmake-extra-modules libxi-dev libxcursor-dev libxinerama-dev libxrandr-dev waylandpp-dev libwayland-server0 libwayland-client++1 libuuid1 uuid-dev -y
```

## GOOD TO KNOW WHILE WORKING

- The Engine is written in C++20 and uses Vulkan API for rendering (Will support DX12 in future).
- The Engine is still in development and not ready for production.
- Development has been suspended till the runtime reflection is available to use on this game engine.

- While running build generator for cmake, you will see a part where the cmake shows the excluded files. When working with Linux the excluded files needs to belong Win32API and 
vice versa. This has been added to CMake with the aim of reducing macro controllers and boosting the compilation speed.

```terminal
1> [CMake] -- ----------------- Excluded files: -----------------
1> [CMake] --   D:/Projects/MAGE/Source/Engine/Platform/Linux/LinuxDirectory.cpp
1> [CMake] --   D:/Projects/MAGE/Source/Engine/Platform/Linux/LinuxDirectory.h
1> [CMake] --   D:/Projects/MAGE/Source/Engine/Platform/Linux/LinuxGuid.cpp
1> [CMake] --   D:/Projects/MAGE/Source/Engine/Platform/Linux/LinuxGuid.h
1> [CMake] --   D:/Projects/MAGE/Source/Engine/Platform/Linux/LinuxPath.cpp
1> [CMake] --   D:/Projects/MAGE/Source/Engine/Platform/Linux/LinuxPath.h
1> [CMake] -- ----------------- End of excluded files -----------------
```

## IMPLEMENTED FEATURES
- [x] Unit Test Environment (Catch2)
- [x] Win32 Platform
- [ ] Linux Platform (W.I.P @hakut)
- [x] GAL (Graphics Abstraction Layer)
- [x] Vulkan GAL Implementation
- [x] Application Module System
- [x] Job System for multi-thread behaviour
- [x] Runtime Shader Compiler
- [x] Custom Memory Allocator
- [ ] JSON Serialization (W.I.P)
- [ ] Runtime Type Reflection (W.I.P)
- [ ] Resource Control Mechanism
- [ ] Entity Component System
- [ ] Editor Integration with ImGui
- [ ] Scripting
- [ ] Physics Engine Implementation
- [ ] Audio Engine Implementation

## DEPENDENCIES
### Platform
- [x] GLFW (Window Control Unit)
- [x] ImGui (Immediate GUI Library)

### Resource
- [x] Assimp (Mesh Loader)
- [x] EnTT (Entity Component System)
- [x] Stb (Image Loader)

### Utils
- [x] Glm (Mathematic Library)
- [x] Nlohmann (JSON Serialization)
- [x] Magic Enum (Compile Time Enum Reflection)
- [ ] Tracy (Frame Profiler)

### SDK
- [ ] FMod (Audio SDK)
- [x] Vulkan (Graphics SDK)
- [ ] DX12 (Graphics SDK)
- [ ] Nvidia Physx (Physics SDK)
