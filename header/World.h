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
#include "Ghost.h"
#include "Resources.h"
#include "ResourceTypes.h"
#include "Map.h"
#include "Collider.h"
#include "PickupObject.h"
#include "Warp.h"
#include "Text.h"


class World
{
	public:
		explicit World(sf::RenderWindow &c_window);

		void update(const sf::Time df);
		void render();

		void load_resources();
		void create_scene();
		void restartPositions();
		void activateAll(){ scene_graph.activateAll(); }
		void deleteActors();

		CommandQueue& getCommandQueue(){ return command_queue; }
		std::vector<SceneNode*> getNodeVector() { return node_vector; }


	private:
		sf::RenderWindow &window;
		TextureGenerator texture_generator;
		CommandQueue command_queue;
		Collider collider;

		//Labyrinth map
		TMX::Map labyrinth;
		
		//Separate entities
		Pacman* pacman;
		std::vector<Ghost*> ghosts;
		std::vector<PickupObject*> pick_ups;
		std::vector<Warp*> warps;

		//All entities stored in a tree and in a vector
		std::vector<SceneNode*> node_vector;
		std::vector<SceneNode*> layers;
		SceneNode scene_graph;
};

