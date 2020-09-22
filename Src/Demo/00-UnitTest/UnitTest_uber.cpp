// uber build for Linux/OpenBSD/Android

#include "Main.cpp"

// UnitTest for Kernel
#include "Kernel/AtomicIntTest.cpp"
#include "Kernel/ConditionVariableTest.cpp"
#include "Kernel/MathTest.cpp"
#include "Kernel/MutexFastTest.cpp"
#include "Kernel/SmartPtrTest.cpp"
#include "Kernel/ThreadTest.cpp"
#include "Kernel/TimeTest.cpp"
#include "Kernel/UnitTestSelfTest.cpp"

// UnitTest for Driver
#include "Driver/Client/CommandBufferTest.cpp"
#include "Driver/Client/CommandLockerTest.cpp"
#include "Driver/Client/CommandTest.cpp"
#include "Driver/Client/DeviceTest.cpp"
#include "Driver/Client/StateCacheTest.cpp"
#include "Driver/Client/Command/FakeCommand.cpp"
#include "Driver/Server/FakeDevice.cpp"
#include "Driver/Server/FakeFactory.cpp"
#include "Driver/Server/Command/FakeServerCommand.cpp"
#include "Driver/Server/State/FakeServerState.cpp"
