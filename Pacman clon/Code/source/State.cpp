#include "..\header\State.h"
#include "..\header\StateStack.h"


State::State(StateStack &c_state_stack, sf::RenderWindow &c_window, Player &c_player, TextureGenerator &c_texture_factory)
: window(c_window)
, state_stack(&c_state_stack)
, player(&c_player)
, texture_factory(&c_texture_factory)
, elapsed(sf::Time::Zero)
, lifetime(sf::Time::Zero)
, is_key_pressed(false)
{
}


State::~State()
{
}

void State::requestPop()
{
	state_stack->pop();
}

void State::requestPush(StateName::Id state_type)
{
	state_stack->push(state_type);
}

/*void State::push(unsigned int state)
{
	state_stack->push(state);
}

void State::pop()
{
	state_stack->pop();
}*/
