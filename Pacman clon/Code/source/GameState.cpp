#include "..\header\GameState.h"

#include "..\header\StateStack.h"

GameState::GameState(StateStack &c_state_stack, sf::RenderWindow &c_window, Player &c_player, TextureGenerator &c_texture_factory)
: State(c_state_stack, c_window, c_player, c_texture_factory)
, world(c_window, c_player)
, enter_time(sf::seconds(0.f))
, exit_time(sf::seconds(0.f))
, wait_to_load_time(sf::seconds(0.5f))
, isLoaded(false)
, isLoadBegin(true)
{
	setEnterLifetime(enter_time.asSeconds());

	setTransitionState(Transitions::None);
	std::cout << "Game state created" << std::endl;

}


GameState::~GameState()
{
	std::cout << "GameState deleted" << std::endl;
}

void GameState::readInput()
{
	if (getPlayer()->isEnabled())
	getPlayer()->updateInput(world.getCommandQueue());	
}

void GameState::draw()
{
	world.render();
}

void GameState::update(sf::Time df)
{
	world.update(df);

	if (world.isGameOverEnd())
	{
		setTransitionState(Transitions::OnExit);
	}
}

void GameState::enterUpdate(sf::Time df)
{
	if (isLoadBegin == true)
	{
		world.load_resources();
		isLoadBegin = false;
	}

	wait_to_load_time -= df;

	if (wait_to_load_time <= sf::Time::Zero)
	{
		std::cout << "Gamestate to update" << std::endl;
		setTransitionState(Transitions::None);
		world.create_scene();
		world.registerPickups();
		getPlayer()->enable();

	}
}

void GameState::enterDraw()
{
	
}

void GameState::exitUpdate(sf::Time df)
{
	requestPop();
	setTransitionState(Transitions::OnEnter);

	std::cout << "Gamestate to OnEnter Logo" << std::endl;
}

void GameState::exitDraw()
{

}

