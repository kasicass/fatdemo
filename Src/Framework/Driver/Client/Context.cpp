#include "Driver/Client/Context.h"
#include "Driver/Client/ICommand.h"

namespace Fat {

Context::Context(Device& device) :
	CommandBuffer(device),
	StateCache()
{
}

Context::~Context()
{
}

void Context::Queue(ICommand* pCommand)
{
	// First flush the state cache such that all out-dated cache entries will be queued before the command
	// itself. This is important as the command relies on the state cache current state.
	StateCache::Flush(*this);

	CommandBuffer::Queue(pCommand);
}

void Context::End()
{
	// Reset state cache to default. This clear the cache from any "user" state.
	StateCache::Reset();

	CommandBuffer::End();
}

}
