#include "FatFramework.h"

namespace Fat {

//
// StateCache
//

StateCache::TStateDefaultConstructorFn StateCache::s_defaultStateConstructor[EStateType::eValuesCount];

void StateCache::RegisterDefaultConstructor(EStateType::EValue stateType, TStateDefaultConstructorFn fn)
{
	FatAssertNoText(fn != NULL);
	FatAssert(s_defaultStateConstructor[stateType] == NULL, L"State %d is already registered", stateType);
	s_defaultStateConstructor[stateType] = fn;
}

void StateCache::RegisterStateConstructors()
{
	// TODO
}

StateCache::StateCache()
{
	// Allocate and initialize default state vector
	FAT_ENUM_FOREACH(stateType, EStateType)
	{
		TStateDefaultConstructorFn pDefaultConstructor = s_defaultStateConstructor[stateType];
		FatAssert(pDefaultConstructor != NULL, L"No registered state for state type %d", stateType);

		IState* pState = (*pDefaultConstructor)();
		defaultStates_.push_back(pState);
		FatAssertNoText(defaultStates_[stateType] == pState);

		entries_.emplace_back(pState);
		flushableEntries_.push_back(stateType);
		outdatedEntries_.push_back(true);
	}
}

StateCache::~StateCache()
{
	entries_.clear();
	FAT_ENUM_FOREACH(stateType, EStateType)
	{
		IState* pState = defaultStates_[stateType];
		FatDelete(pState);
		defaultStates_[stateType] = NULL;
	}
}

Bool StateCache::Bind(IState& state)
{
	EStateType::EValue stateType = state.GetStateType();
	CacheEntry& entry = entries_[stateType];
	if (entry != state)
	{
		// changed, then update the entry
		SetEntry(stateType, state);
		return true;
	}
	return false;
}

Bool StateCache::Reset(EStateType::EValue stateType)
{
	IState& defaultState = *defaultStates_[stateType];
	return Bind(defaultState);
}

Bool StateCache::SetEntry(EStateType::EValue stateType, IState& state)
{
	Bool alreadyOutdated = outdatedEntries_[stateType];
	if (!alreadyOutdated)
	{
		// make it out-dated
		outdatedEntries_[stateType] = true;
		flushableEntries_.push_back(stateType);
	}
	entries_[stateType] = state;
	return (alreadyOutdated == false);
}

UInt32 StateCache::Flush(CommandBuffer& commandBuffer)
{
	UInt32 queuedStates = (UInt32)flushableEntries_.size();
	for (size_t i = 0; i < flushableEntries_.size(); ++i)
	{
		// Fetch state index to flush
		const UInt32 stateType = flushableEntries_[i];

		// Reset state cache internal flags
		FatAssertNoText(outdatedEntries_[stateType]);
		outdatedEntries_[stateType] = false;

		// Fetch the state to flush
		CacheEntry& entry = entries_[stateType];
		entry.Queue(commandBuffer);
	}

	// No more state to flush
	flushableEntries_.clear();
	return queuedStates;
}

UInt32 StateCache::Reset()
{
	UInt32 boundState = 0;
	FAT_ENUM_FOREACH(stateType, EStateType)
	{
		boundState += Reset(stateType);
	}
	return boundState;
}

//
// CacheEntry
//

CacheEntry::CacheEntry(const IState* pState) :
	ReadOnlyLockerHolder(pState)
{
}

CacheEntry::CacheEntry(const CacheEntry& rhs) :
	ReadOnlyLockerHolder(rhs)
{

}

CacheEntry::~CacheEntry()
{
}

Bool CacheEntry::operator!=(const IState& state)
{
	CacheEntry entry(&state);

	// Compare both the server command and date pointers in order to detect if the state is the same that the
	// one already bound to the cache.
	// - In asynchronous modes, as the state cache is reseted very frame, and as the data is duplicated if it
	// is modified, then strict pointer comparison is enough to detect that the state has a new value.
	// - In synchronous mode (no buffering), the date is duplicated if the state is already in the cache (as
	// its read counter is incremented in that case). This means that the server command pointer comparison
	// is a valid test in this mode.
	return (GetServerCommand() != entry.GetServerCommand()) || (GetData() != entry.GetData());
}

void CacheEntry::operator=(const IState& state)
{
	CacheEntry entry(&state);
	*this = entry;
}

void CacheEntry::operator=(const CacheEntry& rhs)
{
	ReadOnlyLockerHolder::operator=(rhs);
}

void CacheEntry::Queue(CommandBuffer& commandBuffer) const
{
	commandBuffer.Queue(*this);
}

}
