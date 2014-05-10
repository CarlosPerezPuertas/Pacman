#pragma once

#include "State.h"


class LogoState : public State
{
	public:
		LogoState(StateStack &c_state_stack, sf::RenderWindow &c_window, Player &c_player, TextureGenerator &c_texture_factory);
		virtual ~LogoState();

		virtual void readInput();
		virtual void draw();
		virtual void update(sf::Time df);

		virtual void enterUpdate(sf::Time df);
		virtual void exitUpdate(sf::Time df);
		virtual void enterDraw();
		virtual void exitDraw();

		void restart();


	private:
		sf::Texture texture;
		sf::Sprite sprite;

		ScreenEffect exit_effect;

		FadeAffector affector;
		sf::Time logo_time;
		sf::Time exit_time;

		sf::Texture texture2;

};

