#pragma once

#include "Entity.h"

enum class Direction
{
	None,
	Up,
	Down,
	Left,
	Right
};

class MoveableObject : public Entity
{
	public:
		MoveableObject();
		virtual ~MoveableObject();

		void move(const sf::Vector2f displacement);
		void moveByMap(Direction dir, sf::Time dt);
		const float getSpeed(){ return speed; }

		inline void addDirection(Direction direction){ possible_directions.insert(direction); }
		inline void removeDirection(Direction direction){ possible_directions.erase(direction); }
		inline bool isPossibleDirection(Direction direction){ return (possible_directions.find(direction) != possible_directions.end()) && direction != Direction::None; }
		inline void setCurrentDirection(Direction direction){ current_direction = direction; }
		inline Direction getCurrentDirection(){ return current_direction; }



	protected:
		const float speed;
		std::set<Direction> possible_directions;
		Direction current_direction;
		std::map<Direction, sf::Vector2f> movement_map;
};

