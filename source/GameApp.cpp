#include "..\header\GameApp.h"

#include "..\header\State.h"
#include "..\header\GameState.h"
#include "..\header\LogoState.h"



GameApp::GameApp() :
  window(sf::VideoMode(800, 800), "Pacman")
, TimePerFrame(sf::seconds(1.f / MAX_FPS))
, timeSinceLastUpdate(TimePerFrame)
, frame_counter(0)
, OneSecondCounter(sf::Time::Zero)
, state_stack(window, player, texture_generator)
{
	state_stack.registerState<GameState>(StateName::Game);
	state_stack.registerState<LogoState>(StateName::Logo);
	state_stack.push(StateName::Logo);	
}
 
void GameApp::run()
{
	sf::Clock clock;
	timeSinceLastUpdate = sf::Time::Zero;

	while (window.isOpen())
	{
		sf::Time elapsed = clock.restart();
		timeSinceLastUpdate += elapsed;

		while (timeSinceLastUpdate > TimePerFrame)
		{
			timeSinceLastUpdate -= TimePerFrame;
			readInput();
			update(TimePerFrame);
		}

		render();
	}
}

void GameApp::readInput()
{
	//Update and collect commands. Each state input processing is different
	state_stack.readInput();

	//Input common to all states
	while (window.pollEvent(event))
	{
		if (event.type == sf::Event::Closed)
			window.close();
	}
}

void GameApp::update(sf::Time df)
{
	state_stack.update(df);
	update_fps(df);
}

void GameApp::render()
{
	window.clear();
	state_stack.render();
	window.display();
}

//Update fps each second and count fps
void GameApp::update_fps(sf::Time df)
{
	//Calcule real FPS
	OneSecondCounter += df;

	if (OneSecondCounter >= sf::seconds(1.f))
	{
		std::cout << frame_counter << std::endl;
		OneSecondCounter = sf::Time::Zero;
		frame_counter = 0;
	}
	else ++frame_counter;
}
