#pragma once

#include <SFML/Graphics.hpp>
#include <iostream>
#include <vector>
#include <map>
#include <set>

#include "Command.h"

class CommandQueue;

class Player
{
	public:
		Player();

		void updateInput(CommandQueue &command_queue);

	private:

		enum Action
		{
			GoLeft,
			GoRight,
			GoUp,
			GoDown
		};

		Action current_action;
		std::map<sf::Keyboard::Key, Action> key_to_action;
		std::map<Action, Command> action_to_command;
		
		std::set<sf::Keyboard::Key> keys_pressed;
		sf::Keyboard::Key last_key_pressed;
};


