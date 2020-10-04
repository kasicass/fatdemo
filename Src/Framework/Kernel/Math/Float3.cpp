#include "Kernel/Math/Float3.h"
#include "Kernel/Math/Utils.h"

namespace Fat {

Float3::Float3()
{
}

Float3::~Float3()
{
}

Float3::Float3(F32 _x, F32 _y, F32 _z) :
	x(_x),
	y(_y),
	z(_z)
{
}

Float3::Float3(const Float3& rhs) :
	x(rhs.x),
	y(rhs.y),
	z(rhs.z)
{
}

Float3& Float3::operator=(const Float3& rhs)
{
	x = rhs.x;
	y = rhs.y;
	z = rhs.z;
	return *this;
}

Bool Float3::operator==(const Float3& rhs)
{
	return FatAlmostEqual(x, rhs.x) && FatAlmostEqual(y, rhs.y) && FatAlmostEqual(z, rhs.z);
}

Bool Float3::operator!=(const Float3& rhs)
{
	return !FatAlmostEqual(x, rhs.x) || !FatAlmostEqual(y, rhs.y) || !FatAlmostEqual(z, rhs.z);
}

}
