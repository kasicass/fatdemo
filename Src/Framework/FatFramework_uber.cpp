// uber build for Linux/OpenBSD/Android

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

// Application
#include "Application/Application.cpp"
#include "Application/Application_Android.cpp"
#include "Application/IWidget.cpp"
