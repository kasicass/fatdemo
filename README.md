# FAT DEMO

 * a simple render framework for algorithm testing
 * framework init from "Cross-Platform Rendering Thread: Design and Implementation", ShaderX 7


## Build

**Windows**

```
open Code/Solutions/VS2017/FatDemo.sln
```

**Linux**

```
$ Code/Solutions/Emake
$ ./build.sh debug
```

**Android**

```
> cd Code/Solution/Android
> setup_build_env.cmd
> gradlew.bat assembleDebug
```


## Support Platforms

 * Win7 and later
 * Linux
 * Android


## Support Graphics APIs

 * [D3D9][1]
 * [D3D11][2]
 * [Vulkan][3]
 * [OpenGLES][4]


## Directory Layout

```
[Bin32]                       x86 output
[Bin64]                       x64 output
[BinLib]                      .lib output directory
[BinTemp]                     intermediate build files
[Assets]                      asset resource
[Code]
  [Demo]                      all kind of demos
    [00-UnitTest]
    [01-HelloWnd]
  [FatFramework]              mini framework
    [Application]               windows & app
    [Driver]                    dx9/dx11/opengl/opengles driver
    [Kernel]                    common stuff, e.g. threading, log, unittest
  [External]                  3rdparty libraries
    [d3dx9sdk]
  [Solutions]                 build files
    [Android]                   Android gradle files
    [Emake]                     Linux
    [VS2017]                    Windows
```


[1]:https://docs.microsoft.com/en-us/windows/win32/direct3d9/dx9-graphics
[2]:https://docs.microsoft.com/en-us/windows/win32/direct3d11/atoc-dx-graphics-direct3d-11
[3]:https://www.khronos.org/vulkan/
[4]:https://www.khronos.org/opengles/
