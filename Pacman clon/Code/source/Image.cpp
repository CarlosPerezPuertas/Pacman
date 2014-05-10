#include "..\header\Image.h"


Image::Image(const Textures::Type type, const TextureGenerator &tg )
: sprite(tg.get(type))
, has_movement(false)
, movement_time(sf::Time::Zero)
{
}


void Image::updateCurrent(CommandQueue &command_queue, const sf::Time dt)
{
	if (has_movement == true)
	{
		setPosition(sf::Vector2f(getPosition().x + displacement.x, getPosition().y + displacement.y));
		movement_time -= dt;

		if (movement_time <= sf::Time::Zero)
		{
			has_movement = false;
		}
	}
}

void Image::drawCurrent(sf::RenderTarget &target, sf::RenderStates states) const
{
	target.draw(sprite, states);
}

void Image::moveDuring(sf::Vector2f c_displacement, float seconds)
{
	has_movement = true;
	displacement = c_displacement;
	movement_time = sf::seconds(seconds);
}
