#pragma once

#include <SFML/Graphics.hpp>
#include <map>
#include <functional>
#include <memory>
#include "EntityState.h"

template<typename EntityType, typename Behaviour>
class StateMachine
{
	public:
		StateMachine(EntityType &c_entity) : entity(&c_entity), current_state(nullptr) {}
		~StateMachine(){}

		void update(sf::Time df);

		template<typename T>
		void registerState(Behaviour c_behaviour);
		void changeState(Behaviour c_behaviour);

	private:
		EntityType *entity;
		std::unique_ptr<EntityState<EntityType>> current_state;
		std::map<Behaviour, std::function<std::unique_ptr<EntityState<EntityType>>()>> state_register;
};
template<typename EntityType, typename Behaviour>
void StateMachine<typename EntityType, typename Behaviour>::update(sf::Time df)
{
	current_state->execute(entity, df);
}

template<typename EntityType, typename Behaviour>
template<typename T>
void StateMachine<typename EntityType, typename Behaviour>::registerState(Behaviour c_behaviour)
{
	state_register[c_behaviour] = [](){return std::unique_ptr<EntityState<EntityType>>(new T()); };
}

template<typename EntityType, typename Behaviour>
void StateMachine<typename EntityType, typename Behaviour>::changeState(Behaviour c_behaviour)
{
	if (current_state.get() != nullptr) current_state->exit(entity);

	auto itr = state_register.find(c_behaviour);
	assert(itr != state_register.end());
	current_state = state_register[c_behaviour]();

	current_state->enter(entity);
}


