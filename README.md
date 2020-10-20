# FAT DEMO

 * a simple render framework for algorithm testing
 * framework init from "Cross-Platform Rendering Thread: Design and Implementation", ShaderX 7


## Build

 * open Code/Solutions/VS2017/FatDemo.sln
 * build & play~


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
[Bin32]
[Bin64]
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
    [Emake]                     Linux/OpenBSD
    [VS2017]                    Windows
```


[1]:https://docs.microsoft.com/en-us/windows/win32/direct3d9/dx9-graphics
[2]:https://docs.microsoft.com/en-us/windows/win32/direct3d11/atoc-dx-graphics-direct3d-11
[3]:https://www.khronos.org/vulkan/
[4]:https://www.khronos.org/opengles/
