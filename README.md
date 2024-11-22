<div style="text-align: justify;">

![example event parameter](https://github.com/mtuncbilek95/MAGE/actions/workflows/windows-build.yml/badge.svg)
# M.A.G.E

Modern & Advanced Graphics Engine
## GOOD TO KNOW WHILE WORKING

- The Engine is written in C++20 and uses Vulkan API `v1.3.290.0` for rendering (Will support DX12 in future).
- hpp is way too heavy. I'm going back to C version of Vulkan.
- I have my own memory allocator design. It has issues but mostly its working fine with a little bit over allocation.
- The Engine is still in development and not ready for production.

## IMPLEMENTED FEATURES
- [x] Unit Test Environment (Catch2)
- [x] Win32 Platform
- [ ] Linux Platform
- [x] Vulkan Implementation
- [x] Job System for multi-thread behaviour
- [x] Runtime Shader Compiler
- [x] Custom Memory Allocator
- [ ] JSON Serialization
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


## ACTIVE VULKAN FEATURES
- [x] Dynamic Rendering
- [x] RenderPass & FrameBuffer
- [x] Descriptor Buffer
- [x] Descriptor Set
- [x] Bindless Shaders

## THE AIM OF THE SYSTEM
- I believe the only reason for DX12 would be Xbox support. I will continue my journey with Vulkan as much as possible.
- Linux has no option other than VulkanSDK.
- I already made cross platform support for both Linux and Windows a time ago. It's not that hard to make it. The hard part is to make them work together while doing lots of feature
implementations. So till I get the satisfaction on Win32, I won't touch the Linux part. The system already has the capability to run on Linux.
- Main optimization workflow mostly will be on Windows since buying PS5 Dev Kit is not easy.
- Current goal is not to create showreels but creating crash-less environment
- MOST IMPORTANTLY: Dont rush just to get a solution. It can create future problems. Future you will have more responsibility so take your time to understand.

## WHY PLATFORM SPECIFIC IO IMPLEMENTATION
As I learned from the unit tests I did in both release and debug mode, especially in the Win32, the performance difference is huge. The reason is that the Win32API is a bit risky 
and lower level than the std library. For example, reading a 3 Megabyte file for 1000 times with std library takes between 2.1 seconds and 2.8 seconds due to the thread priority, 
while the Win32API takes between 1.8 seconds and 2.1 seconds due to the thread priorty. We can also see that Platform-specific implementations have better thread performance. Since 
we are working on a game engine and I really care about my performance, I will continue to use platform-specific implementations for IO control units. I have not tested the Linux 
platform performance yet, but I believe if you do not use clang, the performance most probably will be better than the std library. But in order to have better control on the path, 
I will change the parameters that has been used as std::string to std::filesystem::path. The tests above has been done with chrono library.

For those who are curious about my setup:
- CPU: Intel i7-11800H
- RAM: 16 GB DDR4
- GPU: Nvidia RTX 3060 Max Laptop
- SSD: 512 GB NVMe + 1 TB NVMe
- OS: Windows 11 Pro
- IDE: Visual Studio 2022
- MSVC Compiler: MSVC 19.41.34120.0

### Performance Test Results
The tests below have been done with 3 different implementations. The first one is the std::fopen/fread, the second one is the std::stream and the last one is the win32::readFile. All
tests have been done by catch2 library. The tests have been done with 100 samples and 1 iteration. The results are below:

| Benchmark Names        |  samples<br>mean<br>std dev  |  iterations<br>low mean<br>low std dev  |  est run time<br>high mean<br>high std dev  |
| :--------------------- |  -------------------------:  |  ------------------------------------:  |  ----------------------------------------:  |
| std::fopen/fread       | 100<br>3.3963ms<br>322.951us |  1<br>3.33906ms<br>262.83us             |  328.766ms<br>3.4672ms<br>401.061us         |
| std::stream            | 100<br>6.5817ms<br>337.886us |  1<br>6.52007ms<br>289.23us             |  642.397ms<br>6.6532ms<br>425.863us         |
| Win32::ReadFile        | 100<br>3.3310ms<br>218.811us |  1<br>3.29175ms<br>182.57us             |  336.502ms<br>3.3782ms<br>269.380us         |
