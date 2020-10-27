#pragma once

// Platform-specific
#include "FatFramework/Kernel/PlatformConfig.h"
#include "FatFramework/Kernel/PlatformHeaders.h"

// c headers
#include <stdlib.h>
#include <stdio.h>
#include <stdarg.h>
#include <math.h>
#include <string.h>
#include <ctype.h>
#include <wctype.h>
#include <wchar.h>
#include <assert.h>

// c++ headers
#include <vector>
#include <list>
#include <map>
#include <atomic>
#include <sstream>

// FatFramework - Kernel
#include "FatFramework/Kernel/Common/Types.h"
#include "FatFramework/Kernel/Common/Macros.h"
#include "FatFramework/Kernel/Common/OS.h"
#include "FatFramework/Kernel/Common/Log.h"
#include "FatFramework/Kernel/Common/Assertion.h"
#include "FatFramework/Kernel/Common/NonCopyable.h"
#include "FatFramework/Kernel/Common/Memory.h"
#include "FatFramework/Kernel/Common/Time.h"
#include "FatFramework/Kernel/String/TCharTraits.h"
#include "FatFramework/Kernel/String/TString.h"
#include "FatFramework/Kernel/String/TStackString.h"
#include "FatFramework/Kernel/UnitTest/UnitTest.h"
#include "FatFramework/Kernel/Thread/Interlocked.h"
#include "FatFramework/Kernel/Thread/AtomicInt.h"
#include "FatFramework/Kernel/Thread/TAutoLock.h"
#include "FatFramework/Kernel/Thread/MutexFast.h"
#include "FatFramework/Kernel/Thread/ConditionVariable.h"
#include "FatFramework/Kernel/Common/SmartPtr.h"
#include "FatFramework/Kernel/Thread/Thread.h"
#include "FatFramework/Kernel/Math/Utils.h"
#include "FatFramework/Kernel/Math/Float3.h"
#include "FatFramework/Kernel/Math/Float4.h"

// FatFramework - Driver
#include "FatFramework/Driver/Client/FactorySelector.h"
#include "FatFramework/Driver/Server/Interface/IServerObject.h"
#include "FatFramework/Driver/Server/Interface/IServerFactory.h"
#include "FatFramework/Driver/Server/Interface/IServerDevice.h"
#include "FatFramework/Driver/Server/Interface/IServerCommand.h"
#include "FatFramework/Driver/Server/Interface/IServerState.h"
#include "FatFramework/Driver/Client/CommandLocker.h"
#include "FatFramework/Driver/Client/IObject.h"
#include "FatFramework/Driver/Client/ICommand.h"
#include "FatFramework/Driver/Client/StateType.h"
#include "FatFramework/Driver/Client/IState.h"
#include "FatFramework/Driver/Client/Device.h"
#include "FatFramework/Driver/Client/DriverStatistics.h"
#include "FatFramework/Driver/Client/StateCache.h"
#include "FatFramework/Driver/Client/CommandBuffer.h"
#include "FatFramework/Driver/Client/Context.h"
#include "FatFramework/Driver/Client/Command/DrawCommand.h"
#include "FatFramework/Driver/Client/Command/SwapCommand.h"
#include "FatFramework/Driver/Client/State/CameraState.h"
#include "FatFramework/Driver/Client/State/RenderTargetState.h"

// FatFramework - Driver - DX9
#if FAT_OS_WINDOWS
#include "FatFramework/Driver/Server/D3D9/D3D9Common.h"
#include "FatFramework/Driver/Server/D3D9/D3D9Reset.h"
#include "FatFramework/Driver/Server/D3D9/D3D9StateShadow.h"
#include "FatFramework/Driver/Server/D3D9/D3D9GPUTimer.h"
#include "FatFramework/Driver/Server/D3D9/D3D9Device.h"
#include "FatFramework/Driver/Server/D3D9/D3D9Factory.h"
#include "FatFramework/Driver/Server/D3D9/Command/D3D9SwapCommand.h"
#include "FatFramework/Driver/Server/D3D9/State/D3D9RenderTargetState.h"
#endif

// FatFramework - Application
#include "FatFramework/Application/IWidget.h"
#include "FatFramework/Application/Widget_Win32.h"
#include "FatFramework/Application/IApplication.h"
#include "FatFramework/Application/Application.h"
