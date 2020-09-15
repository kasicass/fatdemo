#pragma once

namespace Fat {

class Float4
{
public:
	Float4();
	Float4(F32 _x, F32 _y, F32 _z, F32 _w);
	Float4(const Float4& rhs);
	~Float4();

	Float4& operator=(const Float4& rhs);

	Bool operator==(const Float4& rhs);
	Bool operator!=(const Float4& rhs);

public:
	F32 x;
	F32 y;
	F32 z;
	F32 w;
};

}
