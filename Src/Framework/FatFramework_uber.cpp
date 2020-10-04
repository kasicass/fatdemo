// uber build

// Kernel
#include "Kernel/Common/Assertion.cpp"
#include "Kernel/Common/Log.cpp"
#include "Kernel/Common/Memory.cpp"
#include "Kernel/Common/OS.cpp"
#include "Kernel/Common/Time.cpp"

#include "Kernel/Math/Float3.cpp"
#include "Kernel/Math/Float4.cpp"

#include "Kernel/Thread/AtomicInt.cpp"
#include "Kernel/Thread/ConditionVariable.cpp"
#include "Kernel/Thread/MutexFast.cpp"
#include "Kernel/Thread/Thread.cpp"

#include "Kernel/UnitTest/UnitTest.cpp"

// Driver - Client
#include "Driver/Client/CommandBuffer.cpp"
#include "Driver/Client/CommandLocker.cpp"
#include "Driver/Client/Context.cpp"
#include "Driver/Client/Device.cpp"
#include "Driver/Client/DriverStatistics.cpp"
#include "Driver/Client/FactorySelector.cpp"
#include "Driver/Client/ICommand.cpp"
#include "Driver/Client/IObject.cpp"
#include "Driver/Client/IState.cpp"
#include "Driver/Client/StateCache.cpp"

#include "Driver/Client/Command/DrawCommand.cpp"
#include "Driver/Client/Command/SwapCommand.cpp"

#include "Driver/Client/State/CameraState.cpp"
#include "Driver/Client/State/RenderTargetState.cpp"

// Driver - Server
#include "Driver/Server/Interface/IServerDevice.cpp"
#include "Driver/Server/Interface/IServerFactory.cpp"

#if defined(FAT_OS_WINDOWS)
#include "Driver/Server/D3D9/D3D9Common.cpp"
#include "Driver/Server/D3D9/D3D9Device.cpp"
#include "Driver/Server/D3D9/D3D9Factory.cpp"
#include "Driver/Server/D3D9/D3D9GPUTimer.cpp"
#include "Driver/Server/D3D9/D3D9Reset.cpp"
#include "Driver/Server/D3D9/D3D9StateShadow.cpp"
#include "Driver/Server/D3D9/dxerr.cpp"
#include "Driver/Server/D3D9/Command/D3D9SwapCommand.cpp"
#include "Driver/Server/D3D9/State/D3D9RenderTargetState.cpp"
#endif

// Application
#include "Application/Application.cpp"
#if defined(FAT_OS_ANDROID)
#include "Application/Application_Android.cpp"
#endif
#include "Application/IWidget.cpp"
#if defined(FAT_OS_WINDOWS)
#include "Application/Widget_Win32.cpp"
#endif
