#pragma once

#if defined(FAT_OS_WINDOWS)

namespace Fat {

// D3D9 GPU timer. Measures the elapsed time on the GPU between two Begin/End calls. It uses D3D9 queries to
// insert events in the D3D9 command buffer.
class D3D9GPUTimer : public ID3D9ResetListener
{
public:
	D3D9GPUTimer();
	virtual ~D3D9GPUTimer();

	void Begin();
	void End();

	Bool IsValid() const;
	F32 GetElapsedTime() const; // Can only be called if timer is valid

protected:
	virtual void PreReset() override;
	virtual void PostResetSucceed() override;

private:
	IDirect3DQuery9Ptr pTimeQueryBegin_;
	IDirect3DQuery9Ptr pTimeQueryEnd_;
	IDirect3DQuery9Ptr pTimeQueryFreq_;

	F32 elapsedTime_;
	Bool issued_;     // Are quries issued
	Bool valid_;      // Is timer valid
	FatIfBuildAssertion(Bool insideBeginEnd_);
};

}

#endif
