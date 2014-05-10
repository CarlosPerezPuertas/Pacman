#pragma once

#include <SFML\Graphics.hpp>
#include <iostream>
#include <vector>
#include <stack>
#include "State.h"
#include "CommandQueue.h"
#include <functional>




class StateStack : private sf::NonCopyable
{
	public:

		enum Action
		{
			Push,
			Pop,
			Clear
		};

		struct DelayedOrder
		{
			Action action;
			StateName::Id id;
		};

	public:
		StateStack(sf::RenderWindow &c_window, Player &c_player, TextureGenerator &c_texture_factory);
		virtual ~StateStack();

		void readInput();
		void update(sf::Time df);
		void render();

		template<typename T>
		void registerState(unsigned int type);
		std::unique_ptr<State> createState(unsigned int type);

		void push(unsigned int state);
		void pop();
		void clear();
		inline bool isEmpty(){ return state_stack.empty(); }

		void applyDelayedOrders();
		inline void setIskeyPressed(bool x){ state_stack.top()->setIskeyPressed(x); }

		//CommandQueue& getCommandQueue(){ return command_queue; }


	private:
		std::stack<std::unique_ptr<State>> state_stack;
		std::vector<DelayedOrder> delayed_orders;
		std::map<unsigned int, std::function<std::unique_ptr<State>()>> state_map;
		sf::RenderWindow &window;
		Player *player;
		TextureGenerator *texture_factory;
};

template<typename T>
void StateStack::registerState(unsigned int type)
{
	state_map[type] = [this]() {return std::unique_ptr<State>(new T(*this, window, *player, *texture_factory)); };
}




