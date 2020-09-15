#pragma once

namespace Fat {

class CommandBuffer;

// Implement an entry in the state cache. Inherit ReaderOnlyLockerHolder class in order to keep track of
// server command references and acquiring command data.
class CacheEntry : public ReadOnlyLockerHolder
{
public:
	CacheEntry(const IState* pState);
	CacheEntry(const CacheEntry& rhs);
	~CacheEntry();

	Bool operator!=(const IState& state);

	void operator=(const IState& state);
	void operator=(const CacheEntry& rhs);

	// Queue an entry to the CommandBuffer passed as argument
	void Queue(CommandBuffer& commandBuffer) const;
};

class StateCache : private NonCopyable
{
public:
	// Register call state constructors, must be called before new StateCache()
	static void RegisterStateConstructors();

public:
	StateCache();
	virtual ~StateCache();

	// Binds a state to the cache. Returns true if the cache entry was not out-dated yet
	Bool Bind(IState& state);

	// Reset a stateType entry to its default state. Returns true if the cache entry was not out-dated yet.
	Bool Reset(EStateType::EValue stateType);

	// Reset all entries to their default state. Returns the number of states that wasn't out-dated yet.
	UInt32 Reset();

protected:
	// Flushes all out-dated states to the command buffer
	UInt32 Flush(CommandBuffer& commandBuffer);

private:
	// State constructor registration function
	typedef IState* (*TStateDefaultConstructorFn)(void);
	static void RegisterDefaultConstructor(EStateType::EValue stateType, TStateDefaultConstructorFn fn);

	// Modify an entry in the cache. Maintains internal structures coherency.
	Bool SetEntry(EStateType::EValue stateType, IState& state);

private:
	static TStateDefaultConstructorFn s_defaultStateConstructor[EStateType::eValuesCount];

	typedef std::vector<CacheEntry> EntryVector;
	typedef std::vector<Bool> OutdatedEntries;
	typedef std::vector<UInt32> FlushableEntries;
	typedef std::vector<IState*> DefaultStates;

	EntryVector entries_;
	OutdatedEntries outdatedEntries_;
	FlushableEntries flushableEntries_;
	DefaultStates defaultStates_;
};

}
