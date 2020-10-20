#pragma once

// TODO
#if 0

namespace Fat {

class CameraStateData : public IStateData
{
public:
	CameraStateData();
	virtual ~CameraStateData();

	// ICommandData interface
	virtual void Clear() override;
	virtual void Copy(const ICommandData* rhs) override;

	void SetEye(const Float3& value);
	const Float3& GetEye() const;

	void SetAt(const Float3& value);
	const Float3& GetAt() const;

	void SetUp(const Float3& value);
	const Float3& GetUp() const;

	void SetFocalAngle(F32 value);
	F32 GetFocalAngle() const;

	void SetRatio(F32 value);
	F32 GetRatio() const;

	void SetNear(F32 value);
	F32 GetNear() const;

	void SetFar(F32 value);
	F32 GetFar() const;

private:
	Float3 eye_;
	Float3 at_;
	Float3 up_;
	F32 focalAngle_;
	F32 ratio_;      // width/height ratio
	F32 near_;       // near plane
	F32 far_;        // far plane
};

typedef TStateWrapper<CameraStateData,
	EStateType::eCamera,
	EFactoryObject::eCameraState> CameraState;

}

#endif
