# M.A.G.E
![example event parameter](https://github.com/mtuncbilek95/MAGE/actions/workflows/windows-build.yml/badge.svg)

Modern & Advanced Graphics Engine
## GOOD TO KNOW WHILE WORKING

- The Engine is written in C++20 and uses Vulkan API `v1.3.290.0` for rendering (Will support DX12 in future).
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
- [ ] Linux Platform
- [x] GAL (Graphics Abstraction Layer)
- [x] Vulkan GAL Implementation
- [x] Application Module System
- [x] Job System for multi-thread behaviour
- [x] Runtime Shader Compiler
- [x] Custom Memory Allocator
- [ ] JSON Serialization (W.I.P)
- [ ] Runtime Type Reflection (W.I.P)
- [ ] Encryption
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
- [x] Spd Log (Immediate Logger)
- [x] FMT (Format Configurator for Console)
- [ ] Tracy (Frame Profiler)
- [x] Catch2 (Unit Test Framework)

### SDK
- [ ] FMod (Audio SDK)
- [x] Vulkan (Graphics SDK)
- [ ] DX12 (Graphics SDK)
- [ ] Nvidia Physx (Physics SDK)

## THE AIM OF THE SYSTEM
- Windows should work with DX12 no matter what. But for now I don't want to burnout with DX12 so VulkanSDK continues.
- Linux has no option other than VulkanSDK.
- Main optimization workflow mostly will be on Windows since buying PS5 Dev Kit is not easy.
- Current goal is not to create showreels but creating crash-less environment
- The development process can be halted anytime due to the complexity of custom runtime type identification (In order to do R&D)
- MOST IMPORTANTLY: Dont rush just to get a solution. It can create future problems. Future you will have more responsibility so take your time to understand.

## CUSTOM SOLUTIONS

### Chunk Vector
This vector is a chunked, boosted with static array structure. Unlike std::vector, ChunkVector doesn't work fully dynamic. In the default ctor, it creates a static array 
with 32 empty elements as a chunk. Whenever you add new element, if the chunk is full, it will create another chunk with x2 size. Removing an element doesn't handles the blank 
point in the array dynamically. It means that there will be an empty index in the array. It requires extra thinking while using, otherwise the user can create a huge memory 
leak in the whole runtime.

### MemoryBuffer & MemoryOwnedBuffer
It's a basic wrapper for function pointer which can also be used as data storage. MemoryBuffer is just a reference holder. But MemoryOwnedBuffer owns the data. The user have to
use this with precaution in order to be responsible for unnecessary memory load.

### MemoryMappedFile
It  is a segment of virtual memory that has been assigned a direct byte-for-byte correlation with some portion of a file or file-like resource. This resource is typically a 
file that is physically present on disk, but can also be a device, shared memory object, or other resource that an operating system can reference through a file descriptor. 
Once present, this correlation between the file and the memory space permits applications to treat the mapped portion as if it were primary memory.

### AtomicQueue
It's a lock-free queue which is used for multi-threading. It's a basic queue structure which is used for job system. It's not a thread-safe queue. It's a lock-free queue. Highly
recommended to use with caution only on job related tasks.

## WHY PLATFORM SPECIFIC IO IMPLEMENTATION
As I learned from the unit tests I did in both release and debug mode, especially in the Win32, the performance difference is huge. The reason is that the Win32API is a bit risky 
and lower level than the std library. For example, reading a 3 Megabyte file for 1000 times with std library takes between 2.1 seconds and 2.8 seconds due to the thread priority, 
while the Win32API takes between 1.8 seconds and 2.1 seconds due to the thread priorty. We can also see that Platform-specific implementations have better thread performance. Since 
we are working on a game engine and I really care about my performance, I will continue to use platform-specific implementations for IO control units. I have not tested the Linux 
platform performance yet, but I believe if you do not use clang, the performance most probably will be better than the std library. But in order to have better control on the path, 
I will change the parameters that has been used as std::string to std::filesystem::path.

For those who are curious about my setup:
- CPU: Intel i7-11800H
- RAM: 16 GB DDR4
- GPU: Nvidia RTX 3060 Max Laptop
- SSD: 512 GB NVMe + 1 TB NVMe
- OS: Windows 11 Pro
- IDE: Visual Studio 2022
- MSVC Compiler: MSVC 19.41.34120.0