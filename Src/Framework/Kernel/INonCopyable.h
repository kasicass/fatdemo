#pragma once

namespace Fat {

class INonCopyable
{
protected:
	INonCopyable() {}
	~INonCopyable() {}

private:
	INonCopyable(const INonCopyable&);
	INonCopyable& operator=(const INonCopyable&);
};

}
