#pragma once

#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <iostream>
#include <vector>
#include <chrono>
#include <memory>

#include "CommandQueue.h"
#include "ScreenEffect.h"
#include "ScreenAffector.h"
#include "ScreenAffectors.h"
#include "Resources.h"
#include "ResourceTypes.h"



namespace Transitions
{
	enum State
	{
		None,
		OnEnter,
		OnExit
	};
}

namespace StateName
{
	enum Id
	{
		Logo,
		StartMenu,
		Game,
		Pause,
		Menu
	};
}


class Player;
class StateStack;


class State
{
	public:
		State(StateStack &c_state_stack, sf::RenderWindow &c_window, Player &c_player, TextureGenerator &c_texture_factory);
		virtual ~State();

		virtual void readInput() = 0;
		virtual void update(sf::Time df) = 0;
		virtual void draw() = 0;

		//Transitions between states
		virtual void enterUpdate(sf::Time df) = 0;
		virtual void exitUpdate(sf::Time df) = 0;
		virtual void enterDraw() = 0;
		virtual void exitDraw() = 0;

		void requestPop();
		void requestPush(StateName::Id state_type);


		Transitions::State getTransitionState(){ return transition_state; }
		void setTransitionState(Transitions::State state){ transition_state = state; }

		void setEnterLifetime(float time){ lifetime = sf::seconds(time); elapsed = lifetime; }
		void setIskeyPressed(bool x){ is_key_pressed = x; }
		bool isKeyPressed(){ return is_key_pressed; }


	protected:
		sf::RenderWindow& getWindow(){ return window; }
		Player* getPlayer(){ return player; }
		//void push(unsigned int state);
		//void pop();

	protected:
		sf::Time elapsed;
		sf::Time lifetime;
		bool is_key_pressed;
		
	private:
		sf::RenderWindow &window;
		StateStack *state_stack;
		Player *player;
		TextureGenerator *texture_factory;
		Transitions::State transition_state;	
		
};

