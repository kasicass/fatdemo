#pragma once

#include "Kernel/Math/Float3.h"
#include "Kernel/Math/Float4.h"

namespace Fat {

template <typename T>
inline T FatMin(T left, T right)
{
	return (left < right) ? left : right;
}

template <typename T>
inline T FatMax(T left, T right)
{
	return (left > right) ? left : right;
}

inline Bool FatAlmostEqual(F32 a, F32 b, F32 epsilon = 0.01f)
{
	return (fabs(a - b) < epsilon);
}

inline Bool FatAlmostEqual(F64 a, F64 b, F64 epsilon = 0.001)
{
	return (abs(a - b) < epsilon);
}

}
