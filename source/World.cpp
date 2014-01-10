#include "..\header\World.h"


World::World(sf::RenderWindow &c_window) : 
  window(c_window)
, pacman(nullptr)
, scene_graph()
, command_queue()
, collider(scene_graph)
{
	load_resources();
	create_scene();
}


void World::update(const sf::Time df)
{
	scene_graph.update(command_queue, df);

	//Broadcast callbacks
	while (command_queue.isEmpty() == false)
	{
		scene_graph.recieveCommand(command_queue.pop(), df);
	}
}

void World::render()
{
	window.draw(labyrinth);
	window.draw(scene_graph);
}

void World::load_resources()
{
	texture_generator.load(Textures::Pacman, "textures/pacman.png");
	labyrinth.load("maps/pacman_board.tmx");
}

void World::create_scene()
{
	SceneNode::PtrNode layer(new SceneNode());
	layers.push_back(layer.get());
	scene_graph.addChild(std::move(layer));

	std::unique_ptr<Pacman> pac(new Pacman(Textures::Pacman, texture_generator));
	pacman = pac.get();
	pacman->setPosition(sf::Vector2f(200.f, 300.f));
	pacman->scale(sf::Vector2f(2.f, 2.f));
	layers[0]->addChild(std::move(pac));
	
}