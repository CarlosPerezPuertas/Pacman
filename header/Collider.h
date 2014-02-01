#pragma once

#include <SFML/Graphics.hpp>
#include <iostream>
#include <vector>
#include <string>
#include <set>

#include "CommandQueue.h"
#include "SceneNode.h"
#include "Map.h"


class World;
struct TMX_Tile;

namespace CollisionEvent
{
	enum Id
	{
		PlayerGhost,
		PlayerFruit,
		PlayerSmallDot,
		PlayerBigDot,
		PlayerWarp,
		PlayerMapIntersection,
		GhostMapIntersection
	};
}


class Collider
{
	public:
		explicit Collider(World &c_world, TMX::Map &c_map, CommandQueue &c_command_queue);
		virtual ~Collider();

		void checkSceneCollision(std::vector<SceneNode*> &nodes);
		template<typename T1, typename T2>
		void checkCollision(T1 &entity1, T2 &entity2);
		template<typename T1>
		void checkMapCollision(T1 &entity1);
		template<typename T1>
		int getMapPosition(T1 &entity);
		void registerCollisions();

		inline sf::Vector2f getMapPosition() { return map.getPosition(); }


	public:
		CommandQueue &command_queue;
		World &world;

	private:
		std::map<CollisionEvent::Id, std::pair<Category::Type, Category::Type>> events;
		std::map<std::pair<Category::Type, Category::Type>, CommandCollision> interactions;
		TMX::Map &map;
		
};

template<typename T1, typename T2>
void Collider::checkCollision(T1 &entity1, T2 &entity2)
{
	Category::Type category1 = static_cast<Category::Type>(entity1.getCategory());
	Category::Type category2 = static_cast<Category::Type>(entity2.getCategory());
	std::pair<Category::Type, Category::Type> pairs = std::make_pair(category1, category2);

	auto itr = interactions.find(pairs);
	if (itr != interactions.end()) itr->second.action(entity1, entity2);
	else
	{
		std::swap(pairs.first, pairs.second);
		itr = interactions.find(pairs);

		if (itr != interactions.end()) itr->second.action(entity1, entity2);
	}
}

template<typename T1>
void Collider::checkMapCollision(T1 &entity)
{
	int pos = getMapPosition(entity);
	sf::Vector2f entity_center = entity.getCenter();
	sf::FloatRect direction_rect = map.getChangeDirectionRect(pos);
	int map_width = map.getMapWidth();

	//It works beacuse the tiles that goes around the map are not travesed by the entities
	if (direction_rect.contains(entity_center))
	{
		if (map.tmx_info.collision_map[pos - map_width].type == TileType::Collidable) entity.removeDirection(Direction::Up);
		else entity.addDirection(Direction::Up); 
		
		if (map.tmx_info.collision_map[pos + map_width].type == TileType::Collidable) entity.removeDirection(Direction::Down);
		else entity.addDirection(Direction::Down); 

		if (map.tmx_info.collision_map[pos - 1].type == TileType::Collidable) entity.removeDirection(Direction::Left);
		else entity.addDirection(Direction::Left); 

		if (map.tmx_info.collision_map[pos + 1].type == TileType::Collidable) entity.removeDirection(Direction::Right);
		else entity.addDirection(Direction::Right); 
	}
	else
	{ 
		if (entity.getCurrentDirection() != Direction::None) // Avoid direction removing in game start
		{
			entity.removeDirection(Direction::Up);
			entity.removeDirection(Direction::Down);
			entity.removeDirection(Direction::Right);
			entity.removeDirection(Direction::Left);
			entity.addDirection(entity.getCurrentDirection());
		}
	}
}

//Calcule the map position using the center of the entity as a reference
template<typename T1>
int Collider::getMapPosition(T1 &entity)
{
	int id = 0;
	int map_width = map.tmx_info.map_size.x;
	int map_height = map.tmx_info.map_size.y;
	float tile_width = map.tmx_info.collision_map[0].rect.width;
	float tile_height = map.tmx_info.collision_map[0].rect.height;

	sf::Vector2f rect;
	rect.x = entity.getGlobalRect().left;
	rect.y = entity.getGlobalRect().top;

	//The center of the entity
	rect.x += entity.getGlobalRect().width / 2.f;
	rect.y += entity.getGlobalRect().height / 2.f;
	
	//Perfect precision when it's scaled by integers +- 0.001 pixels
	id = static_cast<int>((rect.x - map.getPosition().x) / tile_width) + 
		 static_cast<int>(static_cast<int>((rect.y - map.getPosition().y) / tile_height) * map_width);

	return id;
}

