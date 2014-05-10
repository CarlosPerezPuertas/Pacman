#include "..\header\Entity.h"


Entity::Entity(sf::Texture &texture)
: SceneNode()
, sprite(texture)
{
	
}


Entity::~Entity()
{
}


void Entity::scaleEntity(sf::Vector2f factor)
{
	scale(factor);

	real_rect.left *= factor.x;
	real_rect.top *= factor.y;
	real_rect.width *= factor.x;
	real_rect.height *= factor.y;
}

//The rectangle with corrected axis to make easy the collision detection
sf::FloatRect Entity::getGlobalRect()
{
	return getWorldTransform().transformRect(sprite.getGlobalBounds());
}

sf::FloatRect Entity::getInteriorRect()
{
	return getWorldTransform().transformRect(getRealRect());
}

sf::Vector2f Entity::getCenter()
{
	sf::FloatRect rect = getGlobalRect();
	return sf::Vector2f(rect.left + rect.width / 2.f, rect.top + rect.height / 2.f);
}






