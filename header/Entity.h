#pragma once

#include "SceneNode.h"
#include "Animation.h"
#include "Resources.h"
#include "ResourceTypes.h"

enum class Animations
{
	GoLeft,
	GoRight,
	GoUp,
	GoDown
};

class Entity : public SceneNode
{
	public:
		Entity();
		virtual ~Entity();

		inline void setRealRect(sf::FloatRect c_real_rect){ real_rect = c_real_rect; }
		inline sf::FloatRect getRealRect(){ return real_rect; }

		inline sf::Vector2f getInitPos(){ return initial_pos; }
		inline void setInitPos(sf::Vector2f pos){ initial_pos = pos; }
		inline void restartPosition(){ setPosition(initial_pos); }

		void scaleEntity(sf::Vector2f factor);


	protected:
		sf::FloatRect real_rect;
		sf::Vector2f  initial_pos;
		
};

