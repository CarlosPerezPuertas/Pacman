#pragma once

#include <SFML\Graphics.hpp>
#include <iostream>
#include <vector>
#include <ctime>
#include <chrono>
#include "Player.h"
#include "CommandQueue.h"
#include "World.h"

#include "StateStack.h"

const float MAX_FPS = 60;

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

//class StateStack;

class GameApp
{
	public:
		GameApp();

		void run();
		void readInput();
		void update(sf::Time df);
		void render();

		void update_fps(sf::Time df);

	private:
		sf::RenderWindow window;
		Player player;
		TextureGenerator texture_generator;
		sf::Event event;
		StateStack state_stack;

		sf::Time timeSinceLastUpdate;
		sf::Time TimePerFrame;
		sf::Time OneSecondCounter;
		float frame_counter;
		
};

