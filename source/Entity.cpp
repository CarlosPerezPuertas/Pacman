#include "..\header\Entity.h"


Entity::Entity() 
: SceneNode()
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






