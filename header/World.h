#pragma once

#include <SFML\Graphics.hpp>
#include <iostream>
#include <vector>
#include <ctime>
#include <memory>
#include <chrono>
#include "Player.h"
#include "CommandQueue.h"
#include "SceneNode.h"
#include "Pacman.h"
#include "Resources.h"
#include "ResourceTypes.h"
#include "Map.h"
#include "Collider.h"


class World
{
	public:
		explicit World(sf::RenderWindow &c_window);

		void update(const sf::Time df);
		void render();

		void load_resources();
		void create_scene();

		CommandQueue& getCommandQueue(){ return command_queue; }

	private:
		sf::RenderWindow &window;
		Pacman* pacman;
		std::vector<SceneNode*> layers;
		SceneNode scene_graph;
		TextureGenerator texture_generator;
		CommandQueue command_queue;
		TMX::Map labyrinth;
		Collider collider;
};

