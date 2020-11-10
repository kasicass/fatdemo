#pragma once

#include "FatFramework/Kernel/PlatformConfig.h"
#include "FatFramework/Kernel/Common/Types.h"
#include "FatFramework/Kernel/Common/Assertion.h"
#include "FatFramework/Kernel/Common/Memory.h"
#include "FatFramework/Kernel/Math/Utils.h"

namespace Fat {

// TODO
#if FAT_CPU_AMD64
#  error LockFreeQueue only supports x86 right now
#endif

// A fixed-size, lockfree queue
template <typename T>
class LockFreeQueue
{
private:
	struct LFQNode
	{
		union
		{
			struct
			{
				T* Elem;
				UInt32 Key;
			};

			SInt64 Value;
		};

		LFQNode()
		{
		}

		LFQNode(T* elem, UInt32 key) :
			Elem(elem), Key(key)
		{
		}
	};

	SInt32 size_;
	LFQNode* realBuffer_;
	LFQNode* buffer_;
	volatile SInt32 readIndex_;
	volatile SInt32 writeIndex_;

public:
	LockFreeQueue() :
		size_(0),
		realBuffer_(NULL),
		buffer_(NULL),
		readIndex_(0),
		writeIndex_(0)
	{
	}

	~LockFreeQueue()
	{
		if (realBuffer_)
		{
			FatFree(realBuffer_);
		}
	}

	void Init(SInt32 size)
	{
		size_ = FatGetNextPowerOfTwo(size);
		realBuffer_ = (LFQNode*)FatMalloc(sizeof(LFQNode) * size + 128);
		FatAssertNoText(realBuffer_);
		buffer_ = (LFQNode*)(((UInt32)realBuffer_ + 127) & ~127);
		FatAssertNoText(!((UInt32)buffer_ & 127));

		for (SInt32 i = 0; i < size; ++i)
		{
			buffer_[i].Elem = NULL;
			buffer_[i].Key  = i;
		}
	}
};

}
