#pragma once

#include <SFML/Graphics.hpp>

template <typename Entity>
class EntityState
{
	public:
		EntityState(){}
		virtual ~EntityState(){}

		virtual void enter(Entity *entity) = 0;
		virtual void execute(Entity *entity, sf::Time df) =  0;
		virtual void exit(Entity *entity) = 0;
};

