<div style="text-align: justify;">

# M.A.G.E
![example event parameter](https://github.com/mtuncbilek95/MAGE/actions/workflows/windows-build.yml/badge.svg)

Modern & Advanced Graphics Engine
## GOOD TO KNOW WHILE WORKING

- The Engine is written in C++20 and uses Vulkan API `v1.3.290.0` for rendering (Will support DX12 in future).
- The Engine is still in development and not ready for production.
- While running build generator for cmake, you will see a part where the cmake shows the excluded files. When working with Linux the excluded files needs to belong Win32API and 
vice versa. This has been added to CMake with the aim of reducing macro controllers and boosting the compilation speed. **Edit:** It is still there but it's not that important anymore.
Because I literally deep dived into not having layers on platform specific implementations. Couple things are still there for this excluding purpose but most of them are gone.

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
- Windows should work with DX12 no matter what. But for now I don't want the burnout with DX12, so VulkanSDK continues. If I could go the past, I would choose DX12 though.
- Linux has no option other than VulkanSDK.
- I already made cross platform support for both Linux and Windows. It's not that hard to make it. The hard part is to make them work together while doing lots of feature
implementations. So till I get the satisfaction on Win32, I won't touch the Linux part. The system already has the capability to run on Linux.
- Main optimization workflow mostly will be on Windows since buying PS5 Dev Kit is not easy.
- Current goal is not to create showreels but creating crash-less environment
- MOST IMPORTANTLY: Dont rush just to get a solution. It can create future problems. Future you will have more responsibility so take your time to understand.

## CUSTOM SOLUTIONS

<h3><u>Chunk Vector:</u></h3>

This vector is a chunked, boosted with static array structure. Unlike std::vector, ChunkVector doesn't work fully dynamic. In the default ctor, it creates a static array 
with 32 empty elements as a chunk. Whenever you add new element, if the chunk is full, it will create another chunk with x2 size. Removing an element doesn't handles the blank 
point in the array dynamically. It means that there will be an empty index in the array. It requires extra thinking while using, otherwise the user can create a huge memory 
leak in the whole runtime.

<h3><u>MemoryBuffer & MemoryOwnedBuffer</u></h3>

It's a basic wrapper for function pointer which can also be used as data storage. MemoryBuffer is just a reference holder. But MemoryOwnedBuffer owns the data. The user have to
use this with precaution in order to be responsible for unnecessary memory load.

<h3><u>MemoryMappedFile</u></h3>

It  is a segment of virtual memory that has been assigned a direct byte-for-byte correlation with some portion of a file or file-like resource. This resource is typically a 
file that is physically present on disk, but can also be a device, shared memory object, or other resource that an operating system can reference through a file descriptor. 
Once present, this correlation between the file and the memory space permits applications to treat the mapped portion as if it were primary memory.

<h3><u>AtomicQueue</u></h3>

It's a lock-free queue which is used for multi-threading. It's a basic queue structure which is used for job system. It's not a thread-safe queue. It's a lock-free queue. Highly
recommended to use with caution only on job related tasks.

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

# THE MOST IMPORTANT PART OF A GAME ENGINE

First of all I have to say something. When it comes to learning something very advanced/complex, thinking everything by yourself can halt your progress. We know that everyone tries to
teach something. Even Cherno does a lot of god's work. But when it comes to learning something that is not that popular and advanced as hell, even he doesn't take on the full responsibility. 
I'm not the smartest developer/engineer but I want to share my knowledge with you. Even if it's not correct. Because if I start showing how to do things, some whiners will come and say 
"**You're doing it wrong it has to be done like this**" and "**that**". Which will lead us to the actual solution. Because people always like to correct someone elses mistake to make them 
look stupid. So, I'm taking the responsibility here. Okay, enough with the chit-chat. Let's get to the point.

The most important part of a game engine is the runtime reflection system. It's the heart of the engine. It's the part that makes the engine alive. It's the part that makes the
engine work without huge amount of JSON/XML/YAML files. So there will be a huge explanation about it. I suffered a lot to make this work. I even cried a bit. This thing in here is the 
beginning of the end. Then the rest is just learning about tools, features and the rest.

- Don't rush to make it work. You will regret it. You will regret it so much that you will want to delete the whole project. So take your time to understand the system. 
- Also please learn how to use include the libraries in the project. Since I'm working with CMake, I won't explain how. Because devs that uses CMake mostly knows what to do about 
the regular stuffs. But you already have the whole project, go and check the CMakeLists.txt files. They are the key to understand how to include the libraries if you are a rookie when 
it comes to CMake.

## RUNTIME REFLECTION
Okay, this topic was a huge pain in my bottom lately. If anyone familiar with C++ and worked on data serialization that has been done in runtime automatically, they know my pain. So,
recently I found out that EnTT has a runtime reflection framework called EnTT Meta (Appearently Minecraft uses the same system). I have been working on it for a while and I have 
to say that it's a bit complex for peasants (We are pathetic hairless monkeys). If you're a intermediate level C++ developer, you have to know that you can't write your own reflection 
system because it needs literally the lowest-level programming knowledge that can C++ access. But, you can always use the most trustworthy reflection system which is EnTT Meta.

The task is simple. Those which will be reflected must be registered in the meta's registry. But also they need to be registered before program starts. 
Or they can be registered while in hot-reload. So what we need to do is to manifest them in the registry system. For the engine, it has to be done before runtime. But for the game, I
guess it can be done in runtime but need a bit more research. I will be noting down the every runtime reflection steps I made, so if anyone wants to use it, be my guest.

- P.S: Dont think that I will let entt::meta live in my system. When I understand more, the less I will need entt::meta. But for now, I will use it.

### HOW I IMPLEMENTED THE RUNTIME REFLECTION
First, lets make it work manually. For my taste, it has to work like Unreal Engine. Most devs don't like to see macros in their code. If you are reading this and want to use this, 
you may also check how to switch from macros to attribute specifiers `[[reflect_class]]`. It will probably cause couple problems since your compiler will force itself to read the attribute 
specifiers according to their own rules. But for now, I will be using macros.

- You have to know that the code generator is a tool that I have written in C#. It simply reads every header files in the whole engine and checks if there is a reflection macro. If 
there is, it just generate `File.generated.h` file in the same directory(I may change it to Intermediate), after that it modifies the related header file with `#include "File.generated.h"`.

```cpp
// ReflectedClass.h

#pragma once

#include "Engine/Core/Core.h"
#include "Engine/Serialization/GenerationMacros.h"
/* You can put more in here*/
#include "ReflectedClass.generated.h" // This include has to be the last include in the file in order to work properly

REFLECT_CLASS();
class ENGINE_API ReflectedClass
{
	GENERATE_MANIFEST;
public:
	ReflectedClass();
	~ReflectedClass();

	void NonReflectedFunction();

	REFLECT_METHOD(); // or you can see it as REFLECT_FUNCTION();
	int ReflectedFunction(); // Return value can be whatever you want

private:
	float mNonReflectedVariable = 3.14f;

	REFLECT_MEMBER();
	int mReflectedMember = 32;

	REFLECT_MEMBER();
	String mReflectedString = "Hello World!"; // String is just using String = std::string; for my engine
};
```

Every REFLECT_X() you see above is for code generator to generate the reflection code. They basically will a messenger for code generator to mark the data which will be exposed. 
Unreal Engine adds lots weird and unnecessary stuff in there in order to control their garbage collector better, but we are not focusing on something that big and their system is 
a bit over-engineered. On the other hand `GENERATE_MANIFEST` has more important role. The first thing it will do is:

```cpp
	friend class entt::meta_factory<ReflectedClass>;
```

to add this block to the class ENGINE_API with macros. Okay we add this but what is this? This is where entt::meta docs comes in. According to the docs, the `entt::meta_factory` is the class ENGINE_API where 
the ones that need to be reflected will be stored. But it needs everything as public in order to work. So, that's why we have TypeRegistry. Because adding the factory as a friend alone
is not enough. On the other hand, TypeRegistry is the class ENGINE_API where the core data/instance will be held during the runtime. Okay since we know what they are for, we can see how they will work.

Code Generator will generate something like this to initiate the manifestation codes so we can use them before run:

```cpp
// ReflectedClass.generated.h

#pragma once
#include <entt/entt.hpp>
using namespace entt::literals;

#undef GENERATE_MANIFEST
#define GENERATE_MANIFEST \
	friend class entt::meta_factory<ReflectedClass>; \
	public: \
		class ENGINE_API TypeRegistry \
		{ \
		public: \
			static void Register() \
			{ \
			} \
		}; \
	private: \
		friend class TypeRegistry

```

Okay, we have the core but we still don't have the meta's code. That's where the REFLECT_X() comes in. They will add couple lines in here. I will shoot the guy, then ask the questions, so you
can understand the code a bit better.

```cpp
// ReflectedClass.generated.h

#pragma once
#include <entt/entt.hpp>
using namespace entt::literals;

#undef GENERATE_MANIFEST
#define GENERATE_MANIFEST \
	friend class entt::meta_factory<ReflectedClass>; \
	public: \
		class ENGINE_API TypeRegistry \
		{ \
		public: \
			static void Register() \
			{ \
				entt::meta<ReflectedClass>() \
					.type("ReflectedClass"_hs) \
					.data<&ReflectedClass::mReflectedVariable>("ReflectedVariable"_hs) \
					.data<&ReflectedClass::mReflectedString>("ReflectedString"_hs); \
			} \
		}; \
	private: \
		friend class TypeRegistry

```

`entt::meta<Class>()` is the begin method of the reflections. Then the rest is about adding the data and functions. As I understand the `_hs` you see is already in the entt::meta and it's there
for handling hash strings. There is no big deal about adding your datas and functions. There are more advanced stuffs about adding derived classes and etc. We will get there as well. Since we have 
the register method, We can manually test it in the main.cpp:

```cpp
#include "Engine/Core/Core.h"

using namespace MAGE;

#include <entt/entt.hpp>
using namespace entt::literals;

#include "ReflectedClass.h"

int main(i32 argC, char** argV)
{
	ReflectedClass::TypeRegistry::Register();

	auto by_id = entt::resolve("ReflectedClass"_hs);
	auto reflected_variable = by_id.data("ReflectedVariable"_hs);
	auto test = reflected_variable.get(by_id.construct()).cast<int>();

	printf("ReflectedVariable: %d\n", test);
}
```

This is the very basic implementation of what I come up with. We registered the related data and now we can access whatever we want. It also creates a problem right now. The problem is that, if you
get the instance of the class ENGINE_API, you can literally access anything you want. So, we need to find out a way to control the reflection system for only the ones that we want to expose.

```terminal
ReflectedVariable: 32
```

The thing I wonder is if I can modify the reflected data's value for reflection system. So that, whenever you get an instance you can access the new value. I will be testing this in the future.

**TO BE CONTINUED...**

</div>
