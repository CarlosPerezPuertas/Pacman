#include "..\header\State.h"
#include "..\header\StateStack.h"


State::State(StateStack &c_state_stack, sf::RenderWindow &c_window, Player &c_player, TextureGenerator &c_texture_factory)
: window(c_window)
, state_stack(&c_state_stack)
, player(&c_player)
, texture_factory(&c_texture_factory)
, elapsed(sf::Time::Zero)
, lifetime(sf::Time::Zero)
{
}


State::~State()
{
}

void State::push(unsigned int state)
{
	state_stack->push(state);
}

void State::pop()
{
	state_stack->pop();
}
