#pragma once

namespace Fat {

class ICommand;
class IState;

// Driver rendering context object. Implement a rendering state machine context and its associated command
// buffer and server.
class Context : public CommandBuffer, public StateCache
{
public:
	Context(Device& device);
	virtual ~Context();

	// Queue the command into the command buffer. State cache is internally flush before the command is
	// effectively queued.
	void Queue(ICommand* pCommand);

	// Notify context queuing/binding is ending. Queuing must be began before calling this function. This
	// overrides the command buffer function in order to reset the state cache.
	virtual void End();

private:
	// Disable State object direct queuing at compilation time by declaring the function as private
	void Queue(IState*);
};

}
