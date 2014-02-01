#include "..\header\MoveableObject.h"


MoveableObject::MoveableObject()
: Entity()
, speed(100.f) //100px/second
, current_direction(Direction::None)
{
	movement_map[Direction::Left] = sf::Vector2f(-speed, 0.f);
	movement_map[Direction::Right] = sf::Vector2f(+speed, 0.f);
	movement_map[Direction::Up] = sf::Vector2f(0.f, -speed);
	movement_map[Direction::Down] = sf::Vector2f(0.f, speed);
}


MoveableObject::~MoveableObject()
{
}

void MoveableObject::move(const sf::Vector2f displacement)
{
	setPosition(getPosition() + displacement);
}

void MoveableObject::moveByMap(Direction dir, sf::Time dt)
{
	if (dir == Direction::Left || dir == Direction::Right)
		setPosition(getPosition() + sf::Vector2f(movement_map[dir].x * dt.asSeconds(), 0));
	else if (dir == Direction::Up || dir == Direction::Down)
		setPosition(getPosition() + sf::Vector2f(0, movement_map[dir].y * dt.asSeconds()));
}
