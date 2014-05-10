#pragma once

#include "SceneNode.h"

class Warp : public SceneNode
{
	public:
		explicit Warp(sf::RectangleShape c_rect);

		virtual void updateCurrent(CommandQueue &command_queue, const sf::Time df);
		virtual unsigned int getCategory() const { return Category::Warp; }

		sf::FloatRect getGlobalRect();
		inline void setDestiny(sf::Vector2f des) { warp_destiny = des; }
		inline sf::Vector2f getDestiny() { return warp_destiny; }

	private:
		sf::RectangleShape warp_rect;
		sf::Vector2f    warp_destiny;

};

