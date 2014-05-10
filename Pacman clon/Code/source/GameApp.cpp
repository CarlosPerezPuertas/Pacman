#include "..\header\GameApp.h"

#include "..\header\State.h"
#include "..\header\GameState.h"
#include "..\header\LogoState.h"
#include "..\header\StartState.h"



GameApp::GameApp() :
window(sf::VideoMode::getDesktopMode(), "Pacman", sf::Style::Fullscreen) 
, TimePerFrame(sf::seconds(1.f / MAX_FPS))
, frame_counter(0)
, OneSecondCounter(sf::Time::Zero)
, state_stack(window, player, texture_generator)
{
	sf::VideoMode desktop = sf::VideoMode::getDesktopMode();
	float scale_factor = 0.f;
	scale_factor = (840.f / desktop.height) + 0.12f;
	view.setCenter(720.f / 2.f, 840.f / 2.f - 30.f);
	view.setSize(sf::Vector2f(desktop.width * scale_factor, desktop.height * scale_factor));

	window.setView(view);
	window.resetGLStates();
	window.setVerticalSyncEnabled(true);
	//window.setFramerateLimit(60);

	state_stack.registerState<GameState>(StateName::Game);
	state_stack.registerState<LogoState>(StateName::Logo);
	state_stack.registerState<StartState>(StateName::StartMenu);
	state_stack.push(StateName::Logo);	
}
 
void GameApp::run()
{
	sf::Clock clock;
	sf::Time timeSinceLastUpdate;
	//sf::Time sleep_time;
	
	while (window.isOpen())
	{
		sf::Time elapsed = clock.restart();
		timeSinceLastUpdate += elapsed;

		while (timeSinceLastUpdate >= elapsed)
		{
			timeSinceLastUpdate -= TimePerFrame;
			readInput();
			update(TimePerFrame);	
		}

		//When elapsed is higher we don't wait //NEED TO BE FIXED IN SFML.
		//if (TimePerFrame > elapsed)
		//{
			//sleep_time = TimePerFrame - clock2.getElapsedTime();
			//sf::sleep(sleep_time);
		//}

		update_fps(elapsed);
		render();
	}	


}

void GameApp::readInput()
{
	
	//Input common to all states
	if (window.pollEvent(event))
	{
		if (event.type == sf::Event::Closed)
			window.close();

		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape))
			window.close();

		//Start Screen read input
		if (event.type == sf::Event::KeyPressed)
		if (state_stack.isEmpty() == false) state_stack.setIskeyPressed(true);

		if (event.type == sf::Event::KeyReleased)
		if (state_stack.isEmpty() == false) state_stack.setIskeyPressed(false);

	}

	//Update and collect commands. Each state input processing is different. WE apply polymorfism
	state_stack.readInput();
}

void GameApp::update(sf::Time df)
{
	state_stack.update(df);
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