#include "..\header\GameState.h"

#include "..\header\StateStack.h"

GameState::GameState(StateStack &c_state_stack, sf::RenderWindow &c_window, Player &c_player, TextureGenerator &c_texture_factory)
: State(c_state_stack, c_window, c_player, c_texture_factory)
, world(c_window)
, game_time(sf::seconds(2.f))
, enter_time(sf::seconds(1.f))
, exit_time(sf::seconds(5.f))
, enter_effect(getWindow().getSize(), sf::Color(0,0,0))
, enter_affector(1.f, 0.f)
{
	enter_effect.setLifeTime(enter_time);
	enter_effect.addAffector(enter_affector);

	setTransitionState(Transitions::None);
	std::cout << "Game state created" << std::endl;
}


GameState::~GameState()
{
}

void GameState::readInput()
{
	getPlayer()->updateInput(world.getCommandQueue());
	
}

void GameState::draw()
{
	world.render();
}

void GameState::update(sf::Time df)
{
	world.update(df);
}

void GameState::enterUpdate(sf::Time df)
{
	enter_effect.update(df);

	if (!enter_effect.isRunning()) setTransitionState(Transitions::None);
}

void GameState::enterDraw()
{
	getWindow().draw(enter_effect);
}

void GameState::exitUpdate(sf::Time df)
{

}

void GameState::exitDraw()
{

}

