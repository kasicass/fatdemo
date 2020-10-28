// uber build
#include "Demo/00-UnitTest/Main.cpp"

// UnitTest for Kernel
#include "Demo/00-UnitTest/Kernel/Thread/InterlockedTest.cpp"
#include "Demo/00-UnitTest/Kernel/Thread/SpinLockTest.cpp"
#include "Demo/00-UnitTest/Kernel/Thread/MutexFastTest.cpp"
#include "Demo/00-UnitTest/Kernel/Thread/MutexTest.cpp"
#include "Demo/00-UnitTest/Kernel/AtomicIntTest.cpp"
#include "Demo/00-UnitTest/Kernel/ConditionVariableTest.cpp"
#include "Demo/00-UnitTest/Kernel/MathTest.cpp"
#include "Demo/00-UnitTest/Kernel/SmartPtrTest.cpp"
#include "Demo/00-UnitTest/Kernel/ThreadTest.cpp"
#include "Demo/00-UnitTest/Kernel/TimeTest.cpp"
#include "Demo/00-UnitTest/Kernel/UnitTestSelfTest.cpp"

// UnitTest for Driver
#include "Demo/00-UnitTest/Driver/Client/CommandBufferTest.cpp"
#include "Demo/00-UnitTest/Driver/Client/CommandLockerTest.cpp"
#include "Demo/00-UnitTest/Driver/Client/CommandTest.cpp"
#include "Demo/00-UnitTest/Driver/Client/DeviceTest.cpp"
#include "Demo/00-UnitTest/Driver/Client/StateCacheTest.cpp"
#include "Demo/00-UnitTest/Driver/Client/Command/FakeCommand.cpp"
#include "Demo/00-UnitTest/Driver/Server/FakeDevice.cpp"
#include "Demo/00-UnitTest/Driver/Server/FakeFactory.cpp"
#include "Demo/00-UnitTest/Driver/Server/Command/FakeServerCommand.cpp"
#include "Demo/00-UnitTest/Driver/Server/State/FakeServerState.cpp"
