#pragma once

#include "SceneNode.h"

class Image : public SceneNode
{
	public:
		explicit Image(const Textures::Type type, const TextureGenerator &tg);

		virtual void updateCurrent(CommandQueue &command_queue, const sf::Time dt);
		virtual void drawCurrent(sf::RenderTarget &target, sf::RenderStates states) const;
		sf::FloatRect getRect() { return getWorldTransform().transformRect(sprite.getGlobalBounds()); }

		void moveDuring(sf::Vector2f c_displacement, float seconds);

	private:
		sf::Sprite sprite;

		bool has_movement;
		sf::Vector2f displacement;
		sf::Time movement_time;

};

