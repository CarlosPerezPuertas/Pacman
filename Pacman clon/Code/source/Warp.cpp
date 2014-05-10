#include "..\header\Warp.h"


Warp::Warp(sf::RectangleShape c_rect)
: warp_rect(c_rect)
, warp_destiny(0.f, 0.f)
{
}


void Warp::updateCurrent(CommandQueue &command_queue, const sf::Time df)
{
	
}

sf::FloatRect Warp::getGlobalRect()
{
	
	return getWorldTransform().transformRect(warp_rect.getGlobalBounds());
}
