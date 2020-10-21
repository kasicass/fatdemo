#pragma once

#if defined(FAT_OS_WINDOWS)

namespace Fat {

class ID3D9ResetListener;

// Interface of all D3D9 objects that needs to warn for Reset function calls.
class ID3D9ResetWarner
{
public:
	ID3D9ResetWarner();
	virtual ~ID3D9ResetWarner();

protected:
	// Call PreReset/PostResetSucceed/PostResetFailed on all ID3D9ResetListener objects
	void SendPreReset();
	void SendPostResetSucceed();
	void SendPostResetFailed();

private:
	friend class ID3D9ResetListener;
	void RegisterResetListener(ID3D9ResetListener* pListener);
	void UnregisterResetListener(ID3D9ResetListener* pListener);

private:
	typedef std::list<ID3D9ResetListener*> ResetAwareList;
	ResetAwareList videoMemoryResources_;
	MutexFast listMutex_;
};
extern ID3D9ResetWarner* theD3D9ResetWarner;

// Interface of an object that needs to listen for Reset function calls.
class ID3D9ResetListener
{
public:
	ID3D9ResetListener();
	virtual ~ID3D9ResetListener();

protected:
	void RegisterMe();
	void UnregisterMe();

private:
	friend class ID3D9ResetWarner;

	virtual void PreReset() {}
	virtual void PostResetSucceed() {}
	virtual void PostResetFailed() {}
};

}

#endif
