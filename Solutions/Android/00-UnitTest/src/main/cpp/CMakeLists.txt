#
# Copyright (C) The Android Open Source Project
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#      http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.
#

cmake_minimum_required(VERSION 3.4.1)

# build native_app_glue as a static lib
set(${CMAKE_C_FLAGS}, "${CMAKE_C_FLAGS}")
add_library(native_app_glue STATIC
    ${ANDROID_NDK}/sources/android/native_app_glue/android_native_app_glue.c)

# now build app's shared lib
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=gnu++11 -Wall -Werror -DFAT_DEBUG_BUILD")

# Export ANativeActivity_onCreate(),
# Refer to: https://github.com/android-ndk/ndk/issues/381.
set(CMAKE_SHARED_LINKER_FLAGS
    "${CMAKE_SHARED_LINKER_FLAGS} -u ANativeActivity_onCreate")

add_library(00-UnitTest SHARED
    ../../../../../../Src/Demo/00-UnitTest/Main.cpp
    ../../../../../../Src/Demo/00-UnitTest/TimeTest.cpp
    ../../../../../../Src/Demo/00-UnitTest/AtomicIntTest.cpp
    ../../../../../../Src/Demo/00-UnitTest/UnitTestSelfTest.cpp
    ../../../../../../Src/Demo/00-UnitTest/MutexFastTest.cpp
    ../../../../../../Src/Demo/00-UnitTest/ConditionVariableTest.cpp
    ../../../../../../Src/Demo/00-UnitTest/ThreadTest.cpp
    ../../../../../../Src/Demo/00-UnitTest/SmartPtrTest.cpp
    ../../../../../../Src/Demo/00-UnitTest/Driver/Client/DeviceTest.cpp
    ../../../../../../Src/Demo/00-UnitTest/Driver/Server/FakeDevice.cpp
    ../../../../../../Src/Demo/00-UnitTest/Driver/Server/FakeFactory.cpp
    ../../../../../../Src/Framework/Kernel/Common/Assertion.cpp
    ../../../../../../Src/Framework/Kernel/Common/Log.cpp
    ../../../../../../Src/Framework/Kernel/Common/Memory.cpp
    ../../../../../../Src/Framework/Kernel/Common/Time.cpp
    ../../../../../../Src/Framework/Kernel/Common/OS.cpp
    ../../../../../../Src/Framework/Kernel/UnitTest/UnitTest.cpp
    ../../../../../../Src/Framework/Kernel/Thread/AtomicInt.cpp
    ../../../../../../Src/Framework/Kernel/Thread/MutexFast.cpp
    ../../../../../../Src/Framework/Kernel/Thread/ConditionVariable.cpp
    ../../../../../../Src/Framework/Kernel/Thread/Thread.cpp
    ../../../../../../Src/Framework/Driver/Client/CommandBuffer.cpp
    ../../../../../../Src/Framework/Driver/Client/CommandLocker.cpp
    ../../../../../../Src/Framework/Driver/Client/Device.cpp
    ../../../../../../Src/Framework/Driver/Client/FactorySelector.cpp
    ../../../../../../Src/Framework/Driver/Client/ICommand.cpp
    ../../../../../../Src/Framework/Driver/Client/IObject.cpp
    ../../../../../../Src/Framework/Driver/Client/IState.cpp
    ../../../../../../Src/Framework/Driver/Client/Command/DrawCommand.cpp
    ../../../../../../Src/Framework/Driver/Client/Command/SwapCommand.cpp
    ../../../../../../Src/Framework/Driver/Server/Interface/IServerDevice.cpp
    ../../../../../../Src/Framework/Driver/Server/Interface/IServerFactory.cpp
    ../../../../../../Src/Framework/Application/Application.cpp
    ../../../../../../Src/Framework/Application/Application_Android.cpp
    )

target_include_directories(00-UnitTest PRIVATE
    ${ANDROID_NDK}/sources/android/native_app_glue
    ../../../../../../Src/Framework)

# add lib dependencies
target_link_libraries(00-UnitTest
    android
    native_app_glue
    EGL
    GLESv2
    log)
