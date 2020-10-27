#pragma once

#include "FatFramework/Kernel/Common/Types.h"
#include <math.h>

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

inline bool FatAlmostEqual(F32 a, F32 b, F32 epsilon = 0.01f)
{
	return (fabsf(a - b) < epsilon);
}

inline bool FatAlmostEqual(F64 a, F64 b, F64 epsilon = 0.001)
{
	return (fabs(a - b) < epsilon);
}

}
