#pragma once

#include <SFML/Graphics.hpp>
#include <iostream>
#include <vector>
#include <functional>
#include <cassert>

class SceneNode;

struct Command
{
	std::function<void(SceneNode&, sf::Time)> action;
	unsigned int category;
};

struct CommandCollision
{
	std::function<void(SceneNode&, SceneNode&)> action;
	unsigned int category;
};

template<typename object, typename callback>
std::function<void(SceneNode&, sf::Time)> get_function(callback c)
{
	return[=](SceneNode& node, sf::Time df)
	{
		c(static_cast<object&>(node), df);	
	};
}

template<typename object1, typename object2, typename callback>
std::function<void(SceneNode&, SceneNode&)> get_function_collision(callback c)
//std::function<void(std::pair<SceneNode&, SceneNode&>&, sf::Time)> get_function_collision(callback c)
{
	return[=](SceneNode& node1, SceneNode& node2)
	{
		//std::pair<object1&, object2&> pair = std::make_pair(static_cast<object1&>(node1), static_cast<object2&>(node2));
	
		c(static_cast<object1&>(node1), static_cast<object2&>(node2));
	};
}

