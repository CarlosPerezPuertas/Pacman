#include "..\header\StateStack.h"


StateStack::StateStack(sf::RenderWindow &c_window, Player &c_player, TextureGenerator &c_texture_factory)
: window(c_window)
, player(&c_player)
, texture_factory(&c_texture_factory)
{
	
}

StateStack::~StateStack()
{
}


//We only control when the transitions finishes
void StateStack::readInput()
{
	if(state_stack.top()->getTransitionState() == Transitions::None)
	state_stack.top()->readInput();
}

void StateStack::update(sf::Time df)
{
	switch (state_stack.top()->getTransitionState())
	{
		case Transitions::None:
			state_stack.top()->update(df);
			break;
		case Transitions::OnEnter:
			state_stack.top()->enterUpdate(df);
			break;
		case Transitions::OnExit:
			state_stack.top()->exitUpdate(df);
			break;
	}
}

void StateStack::render()
{
	state_stack.top()->draw();

	switch (state_stack.top()->getTransitionState())
	{
		case Transitions::OnEnter:
			state_stack.top()->enterDraw();
			break;
		case Transitions::OnExit:
			state_stack.top()->exitDraw();
			break;
	}
	
}

void StateStack::push(unsigned int state)
{
	std::unique_ptr<State> new_state(createState(state));
	new_state->setTransitionState(Transitions::OnEnter);
	state_stack.push(std::move(new_state));
}

void StateStack::pop()
{
	if (!state_stack.empty())
	{
		state_stack.pop();
	}
}

void StateStack::clear()
{
	while(!state_stack.empty())
	{
		state_stack.pop();
	}
}



std::unique_ptr<State> StateStack::createState(unsigned int type)
{
	auto itr = state_map.find(type);
	assert(itr != state_map.end());
	return state_map[type]();
}

