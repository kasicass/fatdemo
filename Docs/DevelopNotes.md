# 开发日志

## 2020-09-07

Driver 层的设计

Client

 * IFactorySelector，决定用哪个 IServerFactory

Server

 * Platform-specific Graphics API
 * IServerFactory, 负责创建各种 IServerObject
 * IServerObject, 各种 ServerObject 的基类


## 2020-09-04

 * 花了一下时间，将 Android 的 build 环境搭建起来


## 2020-08-31

 * 《Shader X7》中看到一篇有趣的文章，"Cross-Platform Rendering Thread: Design and Implementation"
 * 打算自己实现一遍，验证下作者 cross-platform graphics api 的封装思路
