#pragma once

#include "SceneNode.h"
#include "Animation.h"

//Drawable entities

class Entity : public SceneNode
{
	public:
		explicit Entity(sf::Texture &texture);
		virtual ~Entity();

		inline void setRealRect(sf::FloatRect c_real_rect){ real_rect = c_real_rect; }
		inline sf::FloatRect getRealRect(){ return real_rect; }

		inline sf::Vector2f getInitPos(){ return initial_pos; }
		inline void setInitPos(sf::Vector2f pos){ initial_pos = pos; }
		inline void restartPosition(){ setPosition(initial_pos);}
		void scaleEntity(sf::Vector2f factor);

		sf::FloatRect getGlobalRect();
		sf::FloatRect getInteriorRect();
		sf::Vector2f getCenter();

	protected:
		sf::Sprite  sprite;
		sf::FloatRect real_rect;
		sf::Vector2f  initial_pos;
		
};

