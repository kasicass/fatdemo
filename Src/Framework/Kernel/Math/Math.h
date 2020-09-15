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

}
