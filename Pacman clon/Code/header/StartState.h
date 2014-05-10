#pragma once

#include "State.h"
#include "SceneNode.h"
#include "CommandQueue.h"
#include "Image.h"
#include "Text.h"
#include "Player.h"


class StartState : public State
{
	public:
		StartState(StateStack &c_state_stack, sf::RenderWindow &c_window, Player &c_player, TextureGenerator &c_texture_factory);
		virtual ~StartState();

		void loadResources();
		void createScene();


		virtual void readInput();
		virtual void draw();
		virtual void update(sf::Time dt);

		virtual void enterUpdate(sf::Time dt);
		virtual void exitUpdate(sf::Time dt);
		virtual void enterDraw();
		virtual void exitDraw();

		

	private:
		CommandQueue command_queue;
		TextureGenerator texture_generator;
		std::vector<SceneNode*> layers;
		Player &player;

		sf::Time time_to_press_key;

		SceneNode scene_graph;
		sf::Texture t_pacman_logo;
		sf::Sprite s_pacman_logo;
		bool key_to_continue;

};

