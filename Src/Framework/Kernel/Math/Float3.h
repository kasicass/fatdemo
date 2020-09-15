#pragma once

namespace Fat {

class Float3
{
public:
	Float3();
	Float3(F32 _x, F32 _y, F32 _z);
	Float3(const Float3& rhs);
	~Float3();

	Float3& operator=(const Float3& rhs);

	Bool operator==(const Float3& rhs);
	Bool operator!=(const Float3& rhs);

public:
	F32 x;
	F32 y;
	F32 z;
};

}
