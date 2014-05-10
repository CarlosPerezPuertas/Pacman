#include "..\header\MoveableObject.h"


MoveableObject::MoveableObject(sf::Texture &texture)
: Entity(texture)
, speed(0.f) //0px/second
, current_direction(Direction::None)
, previous_direction(Direction::None)
, tile_map_id(-1)
, vel_ratio(0.f)
, frightened_ratio(0.f)
, cave_speed_ratio(0.f)
, is_elroy1(false)
, is_elroy2(false)
{
	
}


MoveableObject::~MoveableObject()
{
}

void MoveableObject::setSpeed(float c_speed, float c_ratio)
{
	movement_map[Direction::Left] = sf::Vector2f(-c_speed * c_ratio, 0.f);
	movement_map[Direction::Right] = sf::Vector2f(c_speed * c_ratio, 0.f);
	movement_map[Direction::Up] = sf::Vector2f(0.f, -c_speed * c_ratio);
	movement_map[Direction::Down] = sf::Vector2f(0.f, c_speed * c_ratio);

	speed = c_speed;
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

void MoveableObject::goLeft(sf::Time dt)
{
	setPosition(getPosition() + sf::Vector2f(movement_map[Direction::Left].x * dt.asSeconds(), 0));
}

void MoveableObject::goRight(sf::Time dt)
{
	setPosition(getPosition() + sf::Vector2f(movement_map[Direction::Right].x * dt.asSeconds(), 0));
}

void MoveableObject::goDown(sf::Time dt)
{
	setPosition(getPosition() + sf::Vector2f(0, movement_map[Direction::Down].y * dt.asSeconds()));
}

void MoveableObject::goUp(sf::Time dt)
{
	setPosition(getPosition() + sf::Vector2f(0, movement_map[Direction::Up].y * dt.asSeconds()));

}
