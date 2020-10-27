// uber build

// Kernel
#include "FatFramework/Kernel/Common/Assertion.cpp"
#include "FatFramework/Kernel/Common/Log.cpp"
#include "FatFramework/Kernel/Common/Memory.cpp"
#include "FatFramework/Kernel/Common/OS.cpp"
#include "FatFramework/Kernel/Common/Time.cpp"

#include "FatFramework/Kernel/Math/Float3.cpp"
#include "FatFramework/Kernel/Math/Float4.cpp"

#include "FatFramework/Kernel/Thread/AtomicInt.cpp"
#include "FatFramework/Kernel/Thread/ConditionVariable.cpp"
#include "FatFramework/Kernel/Thread/MutexFast.cpp"
#include "FatFramework/Kernel/Thread/Thread.cpp"

#include "FatFramework/Kernel/UnitTest/UnitTest.cpp"

// Driver - Client
#include "FatFramework/Driver/Client/CommandBuffer.cpp"
#include "FatFramework/Driver/Client/CommandLocker.cpp"
#include "FatFramework/Driver/Client/Context.cpp"
#include "FatFramework/Driver/Client/Device.cpp"
#include "FatFramework/Driver/Client/DriverStatistics.cpp"
#include "FatFramework/Driver/Client/FactorySelector.cpp"
#include "FatFramework/Driver/Client/ICommand.cpp"
#include "FatFramework/Driver/Client/IObject.cpp"
#include "FatFramework/Driver/Client/IState.cpp"
#include "FatFramework/Driver/Client/StateCache.cpp"

#include "FatFramework/Driver/Client/Command/DrawCommand.cpp"
#include "FatFramework/Driver/Client/Command/SwapCommand.cpp"

#include "FatFramework/Driver/Client/State/CameraState.cpp"
#include "FatFramework/Driver/Client/State/RenderTargetState.cpp"

// Driver - Server
#include "FatFramework/Driver/Server/Interface/IServerDevice.cpp"
#include "FatFramework/Driver/Server/Interface/IServerFactory.cpp"

#if FAT_OS_WINDOWS
#include "FatFramework/Driver/Server/D3D9/D3D9Common.cpp"
#include "FatFramework/Driver/Server/D3D9/D3D9Device.cpp"
#include "FatFramework/Driver/Server/D3D9/D3D9Factory.cpp"
#include "FatFramework/Driver/Server/D3D9/D3D9GPUTimer.cpp"
#include "FatFramework/Driver/Server/D3D9/D3D9Reset.cpp"
#include "FatFramework/Driver/Server/D3D9/D3D9StateShadow.cpp"
#include "FatFramework/Driver/Server/D3D9/dxerr.cpp"
#include "FatFramework/Driver/Server/D3D9/Command/D3D9SwapCommand.cpp"
#include "FatFramework/Driver/Server/D3D9/State/D3D9RenderTargetState.cpp"
#endif

// Application
#include "FatFramework/Application/Application.cpp"
#if FAT_OS_ANDROID
#include "FatFramework/Application/Application_Android.cpp"
#endif
#include "FatFramework/Application/IWidget.cpp"
#if FAT_OS_WINDOWS
#include "FatFramework/Application/Widget_Win32.cpp"
#endif
