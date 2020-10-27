# 开发日志

## 2020-10-27

 * 将 Interlocked 模块纳入 Kernel，封装 atomic 的行为。可以去掉 std::atomic 的依赖了
 * 统一全局 singleton var 的命名

Framework

```
GPerfCounter                 时间控制器
GJobSystem                   JobSystem 总控
GDriverStats                 gfx stats
GDriverFactorySelector       gfx factory selector
GDriverD3D9Factory           gfx d3d9 factory
```

UnitTest

```
GFakeFactory                 gfx unit-test factory
GUnitTestMgr                 unit test manager
```


## 2020-10-20

 * 对代码结构做一下重构, PrecompiledHeaders 统一为一个 root directory
 * 符合个人最新的代码习惯


## 2020-10-04

 * 所有 .cpp 都引用 FatFramework.h，在没有 PCH 的基础上，compile 很慢
 * 对于 FatFramework.lib 中的 .cpp，不再依赖 FatFramework.h，根据需要单独 include 对应的 .h
 * 上面方案可以保证 FatFramework.lib 编译加快
 * 支持 uber-build，将所有 .cpp 合并成一个文件，编译飞速


## 2020-09-07

Driver 层的设计

Client

 * IFactorySelector，决定用哪个 IServerFactory

Server

 * Platform-specific Graphics API
 * IServerFactory, 负责创建各种 IServerObject
 * IServerObject, 各种 ServerObject 的基类


## 2020-09-04

 * 花了一些时间，将 Android 的 build 环境搭建起来


## 2020-08-31

 * 《Shader X7》中看到一篇有趣的文章，"Cross-Platform Rendering Thread: Design and Implementation"
 * 打算自己实现一遍，验证下作者 cross-platform graphics api 的封装思路
