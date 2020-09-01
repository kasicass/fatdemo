mode: lib

out: ../../BinLib/FatFramework.a
int: ../../BinTemp/FatFramework

link: stdc++

flag: -Wall -Wformat-security
flag: -std=c++11
flag: -fPIC

inc: ../../Src/Framework

# Kernel
src: ../../Src/Framework/Kernel/Common/Assertion.cpp
src: ../../Src/Framework/Kernel/Common/Log.cpp
src: ../../Src/Framework/Kernel/Common/Memory.cpp
src: ../../Src/Framework/Kernel/Common/Time.cpp
src: ../../Src/Framework/Kernel/UnitTest/UnitTest.cpp

# Application
src: ../../Src/Framework/Application/Application.cpp
