#include "..\header\CommandQueue.h"



void CommandQueue::push(Command &command)
{
	command_queue.push(command);
}

Command CommandQueue::pop()
{
	Command command = command_queue.front();
	command_queue.pop();
	return command;
}

bool CommandQueue::isEmpty()
{
	return command_queue.empty();
}
