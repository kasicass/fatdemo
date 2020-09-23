#include "FatFramework.h"

namespace Fat {

CameraStateData::CameraStateData()
{
	Clear();
}

CameraStateData::~CameraStateData()
{
}

void CameraStateData::Clear()
{
	eye_ = Float3(0, 0, 0);
	at_  = Float3(0, 0, 0);
	up_  = Float3(0, 1, 0);

	focalAngle_ = 90;
	ratio_ = 4.f / 3.f;

	near_ = 0.1f;
	far_ = 100.f;
}

void CameraStateData::Copy(const ICommandData* rhs)
{
	const CameraStateData* pData = (const CameraStateData*)rhs;

	eye_ = pData->eye_;
	at_  = pData->at_;
	up_  = pData->up_;

	focalAngle_ = pData->focalAngle_;
	ratio_ = pData->ratio_;

	near_ = pData->near_;
	far_ = pData->far_;
}

void CameraStateData::SetEye(const Float3& value)
{
	eye_ = value;
}

const Float3& CameraStateData::GetEye() const
{
	return eye_;
}

void CameraStateData::SetAt(const Float3& value)
{
	at_ = value;
}

const Float3& CameraStateData::GetAt() const
{
	return at_;
}

void CameraStateData::SetUp(const Float3& value)
{
	up_ = value;
}

const Float3& CameraStateData::GetUp() const
{
	return up_;
}

void CameraStateData::SetFocalAngle(F32 value)
{
	focalAngle_ = value;
}

F32 CameraStateData::GetFocalAngle() const
{
	return focalAngle_;
}

void CameraStateData::SetRatio(F32 value)
{
	ratio_ = value;
}

F32 CameraStateData::GetRatio() const
{
	return ratio_;
}

void CameraStateData::SetNear(F32 value)
{
	near_ = value;
}

F32 CameraStateData::GetNear() const
{
	return near_;
}

void CameraStateData::SetFar(F32 value)
{
	far_ = value;
}

F32 CameraStateData::GetFar() const
{
	return far_;
}

}
