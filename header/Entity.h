#pragma once

#include "SceneNode.h"
#include "Animation.h"
#include "Resources.h"
#include "ResourceTypes.h"

class Entity : public SceneNode
{
	public:
		Entity();
		virtual ~Entity();

		void move(const sf::Vector2f displacement);
		const float getSpeed(){ return speed; }

		sf::IntRect getRect(){ return getRect(); }

	protected:
		const float speed;
};

