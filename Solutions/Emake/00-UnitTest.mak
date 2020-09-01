mode: exe

out: ../../../Bin/00-UnitTest
int: ../../../BinTemp/00-UnitTest

link: m
link: stdc++

flag: -Wall -Wformat-security

link: ../../BinLib/FatCommon.a

inc: ../../Src/Framework

src: ../../Src/Demo/00-UnitTest/Main.cpp
