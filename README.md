# FAT DEMO

 * a simple render framework for algorithm testing
 * framework init from "Cross-Platform Rendering Thread: Design and Implementation", ShaderX 7


## Support Platforms

 * Win7 and later
 * Linux
 * OpenBSD
 * Android


## Support Graphics APIs

 * [D3D9][1]
 * [D3D11][2]
 * [OpenGL][3]
 * [OpenGLES][4]


## Directory Layout

```
[Bin32]
[Bin64]
[Assets]                      asset resource
[Src]
  [Demo]                      all kind of test programs
    [00-UnitTest]               aha, unit test here
    [01-RenderThread]
    [02-SSAO]
  [Framework]                 FatFramework - a mini render framework
    [Application]               windows & app
    [Driver]                    dx9/dx11/opengl/opengles driver
    [Kernel]                    common stuff, e.g. threading, log, unittest
[Solutions]                   build files
  [Emake]
  [VS2017]
```


[1]:https://docs.microsoft.com/en-us/windows/win32/direct3d9/dx9-graphics
[2]:https://docs.microsoft.com/en-us/windows/win32/direct3d11/atoc-dx-graphics-direct3d-11
[3]:https://www.opengl.org/
[4]:https://www.khronos.org/opengles/
