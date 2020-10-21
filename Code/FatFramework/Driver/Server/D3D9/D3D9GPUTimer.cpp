#include "FatFramework/FatFramework.h"

#if defined(FAT_OS_WINDOWS)

namespace Fat {

D3D9GPUTimer::D3D9GPUTimer()
{
	elapsedTime_ = 0;
	issued_ = false;
	valid_ = false;
	FatIfBuildAssertion(insideBeginEnd_ = false);
}

D3D9GPUTimer::~D3D9GPUTimer()
{
}

void D3D9GPUTimer::Begin()
{
	FatAssert(insideBeginEnd_ == false, L"Timer has already begin");
	FatIfBuildAssertion(insideBeginEnd_ = true);

	// Early out if queries aren't valid
	if (!valid_)
		return;

	// If queries were already issued, then try to get the result
	if (issued_)
	{
		UInt64 dataBegin = 0;
		UInt64 dataEnd   = 0;
		UInt64 dataFreq  = 0;

		if (pTimeQueryFreq_->GetData(&dataFreq, sizeof(dataFreq), 0) == S_OK &&
			pTimeQueryBegin_->GetData(&dataBegin, sizeof(dataBegin), 0) == S_OK &&
			pTimeQueryEnd_->GetData(&dataEnd, sizeof(dataEnd), 0) == S_OK)
		{
			UInt64 deltaTime = dataEnd - dataBegin;
			elapsedTime_ = F32(F64(deltaTime) / F64(dataFreq));

			// The results are there, so asked for re-issuing again
			issued_ = false;
		}
	}

	// If queries aren't issued yet, then start issuing
	if (!issued_)
	{
		pTimeQueryFreq_->Issue(D3DISSUE_END);
		pTimeQueryBegin_->Issue(D3DISSUE_END);
	}
}

void D3D9GPUTimer::End()
{
	FatAssert(insideBeginEnd_ == true, L"Timer Begin hasn't beed called");
	FatIfBuildAssertion(insideBeginEnd_ = false);

	if (!valid_)
		return;

	if (!issued_)
	{
		pTimeQueryEnd_->Issue(D3DISSUE_END);
		issued_ = true;
	}
}

Bool D3D9GPUTimer::IsValid() const
{
	return valid_;
}

F32 D3D9GPUTimer::GetElapsedTime() const
{
	FatAssert(IsValid(), L"Can't be called if timer isn't valid");
	return elapsedTime_;
}

void D3D9GPUTimer::PreReset()
{
	FatAssertNoText(insideBeginEnd_ == false);

	pTimeQueryBegin_ = NULL;
	pTimeQueryEnd_   = NULL;
	pTimeQueryFreq_  = NULL;

	valid_ = false;
}

void D3D9GPUTimer::PostResetSucceed()
{
	FatAssertNoText(valid_ == false && insideBeginEnd_ == false);

	// (Re)build quries
	IDirect3DDevice9* pD3DDevice = theD3D9Device->GetD3DDevice();

	HRESULT hr = pD3DDevice->CreateQuery(D3DQUERYTYPE_TIMESTAMP, GetD3D9SmartModifier(pTimeQueryBegin_));
	if (FAILED(hr))
		return;

	hr = pD3DDevice->CreateQuery(D3DQUERYTYPE_TIMESTAMP, GetD3D9SmartModifier(pTimeQueryEnd_));
	if (FAILED(hr))
		return;

	hr = pD3DDevice->CreateQuery(D3DQUERYTYPE_TIMESTAMPFREQ, GetD3D9SmartModifier(pTimeQueryFreq_));
	if (FAILED(hr))
		return;

	// Every query is created, then timer is valid
	valid_ = true;
}

}

#endif
