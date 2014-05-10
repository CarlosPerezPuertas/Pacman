#include "..\header\StateStack.h"
#include "..\header\Player.h"


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
	if (!state_stack.empty())
	{
		if (state_stack.top()->getTransitionState() == Transitions::None && player->isEnabled())
			state_stack.top()->readInput();
	}	
}

void StateStack::update(sf::Time df)
{
	
	if (!state_stack.empty())
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

	applyDelayedOrders();

}

void StateStack::render()
{
	if (!state_stack.empty())
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

	
	
}

void StateStack::applyDelayedOrders()
{
	for (auto &itr : delayed_orders)
	{
		if (itr.action == Action::Pop)
		{
			if (!state_stack.empty()) state_stack.pop();
		}
		else if (itr.action == Action::Push)
		{
			std::unique_ptr<State> new_state(createState(itr.id));
			new_state->setTransitionState(Transitions::OnEnter);
			state_stack.push(std::move(new_state));
		}
	}

	delayed_orders.clear();
}

void StateStack::push(unsigned int state)
{
	DelayedOrder order;
	order.action = Action::Push;
	order.id = static_cast<StateName::Id>(state);
	delayed_orders.push_back(order);
	
}

void StateStack::pop()
{
	DelayedOrder order;
	order.action = Action::Pop;
	delayed_orders.push_back(order);	
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

