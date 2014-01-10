#pragma once

#include "State.h"
#include "World.h"


class GameState : public State
{
	public:
		GameState(StateStack &c_state_stack, sf::RenderWindow &c_window, Player &c_player, TextureGenerator &c_texture_factory);
		virtual ~GameState();

		virtual void readInput();
		virtual void update(sf::Time df);
		virtual void draw();

		virtual void enterUpdate(sf::Time df);
		virtual void exitUpdate(sf::Time df);
		virtual void enterDraw();
		virtual void exitDraw();

	private:
		World world;
		sf::Time game_time;
		sf::Time enter_time;
		sf::Time exit_time;

		ScreenEffect enter_effect;
		FadeAffector enter_affector;
};

