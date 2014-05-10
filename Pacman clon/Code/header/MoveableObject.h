#pragma once

#include "Entity.h"
#include "Map.h"

enum class Direction
{
	None,
	Up,
	Down,
	Left,
	Right,
	
};


enum GhostBehaviour
{
	None,
	Chase,
	Scatter,
	Frightened,
	CaveReturn
};


class MoveableObject : public Entity
{
	public:
		MoveableObject(sf::Texture &texture);
		virtual ~MoveableObject();


		void move(const sf::Vector2f displacement);
		void moveByMap(Direction dir, sf::Time dt);
		void goLeft(sf::Time dt);
		void goRight(sf::Time dt);
		void goDown(sf::Time dt);
		void goUp(sf::Time dt);


		inline const float getSpeed() const { return speed; }
		inline const float getSpeedRatio() const { return speed * vel_ratio; }
		void setSpeed(float c_speed, float c_ratio);
		inline void setVelRatio(float c_vel_ratio){ vel_ratio = c_vel_ratio; }
		inline float getVelRatio(){ return vel_ratio; }
		

		inline void addDirection(Direction direction){ possible_directions.insert(direction); }
		inline void removeDirection(Direction direction){ possible_directions.erase(direction); }
		inline bool isPossibleDirection(Direction direction){ return (possible_directions.find(direction) != possible_directions.end()) && direction != Direction::None; }
		inline void setCurrentDirection(Direction direction){ current_direction = direction; }
		inline void setPreviousDirection(Direction direction){ previous_direction = direction; }
		inline Direction getCurrentDirection(){ return current_direction; }
		inline std::set<Direction> getPossibleDirections(){ return possible_directions; }

		inline bool isDifferentTileMap(int num_tile)
		{ 
			//Init the tile map id
			if (tile_map_id == -1) tile_map_id = num_tile;
			return tile_map_id != num_tile;
		}

		inline void setIdTileMap(int num_tile) { tile_map_id = num_tile; }
		inline int getTileMap(){ return tile_map_id; }

		inline void setFrightenedRatio(float c_frightened_ratio){ frightened_ratio = c_frightened_ratio; }
		inline float getFrightenedRatio(){ return frightened_ratio; }

		inline void setCaveRatio(float c_cave_speed_ratio){ cave_speed_ratio = c_cave_speed_ratio; }
		inline float getCaveRatio(){ return cave_speed_ratio; }

		inline void setPreviousTileType(TileType previous){ previous_tile_type = previous; }
		inline TileType getPreviousTileType(){ return previous_tile_type; }

		inline bool isElroy1(){ return is_elroy1; }
		inline bool isElroy2(){ return is_elroy2; }
		

	protected:
		float speed;
		float vel_ratio;
		float frightened_ratio;
		float cave_speed_ratio;
		int  tile_map_id;
		bool is_elroy1;
		bool is_elroy2;

		std::set<Direction> possible_directions;
		Direction current_direction;
		Direction previous_direction;
		std::map<Direction, sf::Vector2f> movement_map;

		TileType previous_tile_type;
};

