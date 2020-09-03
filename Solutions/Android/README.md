# Building for Android

## local.properties.template

 * 复制 local.properties.template 到 00-UnitTest\local.properties
 * 修改 00-UnitTest\local.properties，提供 AndroidSdk 路径

## setup_build_env.cmd.template

 * 复制 setup_build_env.cmd.template 为 setup_build_env.cmd
 * 修改 setup_build_env.cmd，提供 AndroidStudio / AndroidSdk 路径

## build

```
> setup_build_env.cmd
> cd 00-UnitTest
> gradlew assemble
```
