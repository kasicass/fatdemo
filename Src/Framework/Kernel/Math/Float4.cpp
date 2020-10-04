#include "Kernel/Math/Float4.h"
#include "Kernel/Math/Utils.h"

namespace Fat {

Float4::Float4()
{
}

Float4::~Float4()
{
}

Float4::Float4(F32 _x, F32 _y, F32 _z, F32 _w) :
	x(_x),
	y(_y),
	z(_z),
	w(_w)
{
}

Float4::Float4(const Float4& rhs) :
	x(rhs.x),
	y(rhs.y),
	z(rhs.z),
	w(rhs.w)
{
}

Float4& Float4::operator=(const Float4& rhs)
{
	x = rhs.x;
	y = rhs.y;
	z = rhs.z;
	w = rhs.w;
	return *this;
}

Bool Float4::operator==(const Float4& rhs)
{
	return FatAlmostEqual(x, rhs.x) && FatAlmostEqual(y, rhs.y) && FatAlmostEqual(z, rhs.z) && FatAlmostEqual(w, rhs.w);
}

Bool Float4::operator!=(const Float4& rhs)
{
	return !FatAlmostEqual(x, rhs.x) || !FatAlmostEqual(y, rhs.y) || !FatAlmostEqual(z, rhs.z) || !FatAlmostEqual(w, rhs.w);
}

}
