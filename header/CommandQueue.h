#pragma once

#include <iostream>
#include <vector>
#include <functional>
#include <cassert>
#include <queue>

#include "Command.h"

class CommandQueue
{
	public:
		void push(Command &command);
		Command pop();
		bool isEmpty();

	private:
		std::queue<Command> command_queue;
};

