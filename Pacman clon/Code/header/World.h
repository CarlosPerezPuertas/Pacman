#pragma once

#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
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
#include "Image.h"
#include "ImageContainer.h"
#include "SoundNode.h"


class World
{
	public:
		explicit World(sf::RenderWindow &c_window, Player &c_player);

		void update(const sf::Time dt);
		void render();

		void load_resources();
		void create_scene();
		void registerPickups();
		void runLogic();

		void nextLevel();
		void restartPositions();
		bool isGameOverEnd();
		void restartLogic();
		void restartDots();
		void nextBonus();

		void activateAll(){ scene_graph.activateAll(); }
		void disableAll(float seconds);
		void deleteActors();

		inline void eatDot(){ --remaining_dots; }
		inline void refreshScore1up(int points){ score_1up += points; }
		inline void addNodeToScene(std::unique_ptr<SceneNode> node) { node_vector.push_back(node.get()); layers[0]->addChild(std::move(node)); }


		CommandQueue& getCommandQueue(){ return command_queue; }
		std::vector<SceneNode*> getNodeVector() { return node_vector; }


	private:
		sf::RenderWindow &window;
		TextureGenerator texture_generator;
		CommandQueue command_queue;
		Collider collider;
		SoundBufferFactory sounds;
		Player &player;

		//Labyrinth map
		TMX::Map labyrinth;
		
		//Separate entities
		Pacman* pacman;
		Ghost* blinky;
		ImageContainer* lifes;
		ImageContainer* bonus;
		Text* game_over;

		//All entities stored in a tree and in a vector
		std::vector<SceneNode*> node_vector;
		std::vector<Ghost*> ghosts;
		std::vector<SceneNode*> layers;
		SceneNode scene_graph;

		//Pickup factory
		std::map<int, std::function<std::unique_ptr<PickupObject>()>> pickup_factory;


		//Pacman attributes
		bool isGameOver;
		int total_dots;
		int remaining_dots;
		int num_level;
		bool first_fruit_created;
		bool second_fruit_created;
		int score_1up;
		const float speed = 185.f;
};

