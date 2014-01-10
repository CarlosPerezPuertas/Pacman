#pragma once

#include <SFML/Graphics.hpp>
#include <iostream>
#include <vector>
#include <string>
#include <set>

#include "CommandQueue.h"
#include "SceneNode.h"

namespace CollisionEvent
{
	enum Id
	{
		PlayerGhost,
		PlayerFruit,
		PlayerPacdot,
		PlayerBigdot,
		PlayerWarp,
		PlayerMapIntersection,
		GhostMapIntersection
	};
}

class Collider
{
	public:
		Collider(SceneNode &c_scene_graph);
		virtual ~Collider();

		void checkCollisions(CommandQueue &command_queue);

		template<typename T1, typename T2>
		void checkCollision(T1 &entity1, T2 &entity2);

	private:

		std::map<CollisionEvent::Id, std::pair<Category::Type, Category::Type>> interaction_events;
		std::map<std::pair<Category::Type, Category::Type>, CommandCollision> interactions;

		SceneNode &scene_graph;
};

template<typename T1, typename T2>
void Collider::checkCollision(T1 &entity1, T2 &entity2)
{
	Category::Type category1 = entity1.getCategory();
	Category::Type category2 = entity2.getCategory();

	auto itr = interactions.find(interactions[std::pair<category1, category2>]);
	assert(itr != itr.end());
	itr->second.action(entity1, entity2);
}

