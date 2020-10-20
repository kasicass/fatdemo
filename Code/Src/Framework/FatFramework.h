#pragma once

// Platform-specific
#include "Kernel/PlatformConfig.h"
#include "Kernel/PlatformHeaders.h"

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
#include "Kernel/Common/Types.h"
#include "Kernel/Common/Macros.h"
#include "Kernel/Common/OS.h"
#include "Kernel/Common/Log.h"
#include "Kernel/Common/Assertion.h"
#include "Kernel/Common/NonCopyable.h"
#include "Kernel/Common/Memory.h"
#include "Kernel/Common/Time.h"
#include "Kernel/String/TCharTraits.h"
#include "Kernel/String/TString.h"
#include "Kernel/String/TStackString.h"
#include "Kernel/UnitTest/UnitTest.h"
#include "Kernel/Thread/AtomicInt.h"
#include "Kernel/Thread/TAutoLock.h"
#include "Kernel/Thread/MutexFast.h"
#include "Kernel/Thread/ConditionVariable.h"
#include "Kernel/Common/SmartPtr.h"
#include "Kernel/Thread/Thread.h"
#include "Kernel/Math/Utils.h"
#include "Kernel/Math/Float3.h"
#include "Kernel/Math/Float4.h"

// FatFramework - Driver
#include "Driver/Client/FactorySelector.h"
#include "Driver/Server/Interface/IServerObject.h"
#include "Driver/Server/Interface/IServerFactory.h"
#include "Driver/Server/Interface/IServerDevice.h"
#include "Driver/Server/Interface/IServerCommand.h"
#include "Driver/Server/Interface/IServerState.h"
#include "Driver/Client/CommandLocker.h"
#include "Driver/Client/IObject.h"
#include "Driver/Client/ICommand.h"
#include "Driver/Client/StateType.h"
#include "Driver/Client/IState.h"
#include "Driver/Client/Device.h"
#include "Driver/Client/DriverStatistics.h"
#include "Driver/Client/StateCache.h"
#include "Driver/Client/CommandBuffer.h"
#include "Driver/Client/Context.h"
#include "Driver/Client/Command/DrawCommand.h"
#include "Driver/Client/Command/SwapCommand.h"
#include "Driver/Client/State/CameraState.h"
#include "Driver/Client/State/RenderTargetState.h"

// FatFramework - Driver - DX9
#if defined(FAT_OS_WINDOWS)
#include "Driver/Server/D3D9/D3D9Common.h"
#include "Driver/Server/D3D9/D3D9Reset.h"
#include "Driver/Server/D3D9/D3D9StateShadow.h"
#include "Driver/Server/D3D9/D3D9GPUTimer.h"
#include "Driver/Server/D3D9/D3D9Device.h"
#include "Driver/Server/D3D9/D3D9Factory.h"
#include "Driver/Server/D3D9/Command/D3D9SwapCommand.h"
#include "Driver/Server/D3D9/State/D3D9RenderTargetState.h"
#endif

// FatFramework - Application
#include "Application/IWidget.h"
#include "Application/Widget_Win32.h"
#include "Application/IApplication.h"
#include "Application/Application.h"
