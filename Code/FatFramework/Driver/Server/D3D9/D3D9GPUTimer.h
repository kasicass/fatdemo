#pragma once

#include "FatFramework/Kernel/PlatformConfig.h"

#if FAT_OS_WINDOWS

#include "FatFramework/Driver/Server/D3D9/D3D9Common.h"
#include "FatFramework/Driver/Server/D3D9/D3D9Reset.h"
#include "FatFramework/Kernel/Common/Assertion.h"

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

	bool IsValid() const;
	F32 GetElapsedTime() const; // Can only be called if timer is valid

protected:
	virtual void PreReset() override;
	virtual void PostResetSucceed() override;

private:
	IDirect3DQuery9Ptr pTimeQueryBegin_;
	IDirect3DQuery9Ptr pTimeQueryEnd_;
	IDirect3DQuery9Ptr pTimeQueryFreq_;

	F32 elapsedTime_;
	bool issued_;     // Are quries issued
	bool valid_;      // Is timer valid
	FatIfBuildAssertion(bool insideBeginEnd_);
};

}

#endif
