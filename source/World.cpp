#include "..\header\World.h"


World::World(sf::RenderWindow &c_window) 
: window(c_window)
, pacman(nullptr)
, labyrinth()
, scene_graph()
, command_queue()
, collider(*this, labyrinth, command_queue)
{
	load_resources();
	create_scene();
}


void World::update(const sf::Time df)
{
	collider.checkSceneCollision(node_vector);
	if (pacman != nullptr) collider.checkMapCollision(*pacman);
	scene_graph.update(command_queue, df);

	//Broadcast callbacks
	while (command_queue.isEmpty() == false)
	{
		scene_graph.recieveCommand(command_queue.pop(), df);
	}

	deleteActors();
}

void World::render()
{
	window.draw(labyrinth);
	window.draw(scene_graph);
}

void World::load_resources()
{
	texture_generator.load(Textures::Pacman, "textures/pacman.png");
	texture_generator.load(Textures::Ghost1, "textures/ghost1.png");
	texture_generator.load(Textures::Ghost2, "textures/ghost2.png");
	texture_generator.load(Textures::Ghost3, "textures/ghost3.png");
	texture_generator.load(Textures::Ghost4, "textures/ghost4.png");
	texture_generator.load(Textures::SmallDot, "textures/small_dot.png");
	texture_generator.load(Textures::BigDot, "textures/big_dot_animated.png");
	texture_generator.load(Textures::Logo, "textures/Logotipo.png");
	texture_generator.load(Textures::Labyrinth, "textures/labyrinth.png");
	texture_generator.load(Textures::ScreenTransition1, "textures/effect02.png");

	labyrinth.load("maps/pacman_board.tmx");
}

void World::create_scene()
{
	SceneNode::PtrNode layer(new SceneNode());
	layers.push_back(layer.get());
	scene_graph.addChild(std::move(layer));

	std::unique_ptr<Pacman> pac(new Pacman(Textures::Pacman, texture_generator));
	pacman = pac.get();
	pacman->setPosition(sf::Vector2f(400.f, 504.f));
	pacman->setInitPos(sf::Vector2f(400.f, 504.f));
	pacman->scale(sf::Vector2f(2.5f, 2.5f));
	pacman->desactivate();
	node_vector.push_back(pacman);
	layers[0]->addChild(std::move(pac));

	std::unique_ptr<Ghost> g1(new Ghost(GhostType::Blinky, Textures::Ghost1, texture_generator));
	ghosts.push_back(g1.get());
	g1->setPosition(sf::Vector2f(380.f, 335.f));
	g1->setInitPos(sf::Vector2f(380.f, 335.f));
	g1->scale(sf::Vector2f(2.3f, 2.3f));
	g1->desactivate();
	node_vector.push_back(g1.get());
	layers[0]->addChild(std::move(g1));	

	std::unique_ptr<Ghost> g2(new Ghost(GhostType::Pinky, Textures::Ghost2, texture_generator));
	ghosts.push_back(g2.get());
	g2->setPosition(sf::Vector2f(380.f, 407.f));
	g2->setInitPos(sf::Vector2f(380.f, 407.f));
	g2->scale(sf::Vector2f(2.3f, 2.3f));
	g2->desactivate();
	node_vector.push_back(g2.get());
	layers[0]->addChild(std::move(g2));

	std::unique_ptr<Ghost> g3(new Ghost(GhostType::Inky, Textures::Ghost3, texture_generator));
	ghosts.push_back(g3.get());
	g3->setPosition(sf::Vector2f(332.f, 407.f));
	g3->setInitPos(sf::Vector2f(332.f, 407.f));
	g3->scale(sf::Vector2f(2.3f, 2.3f));
	g3->desactivate();
	node_vector.push_back(g3.get());
	layers[0]->addChild(std::move(g3));

	std::unique_ptr<Ghost> g4(new Ghost(GhostType::Clyde, Textures::Ghost4, texture_generator));
	ghosts.push_back(g4.get());
	g4->setPosition(sf::Vector2f(428.f, 407.f));
	g4->setInitPos(sf::Vector2f(428.f, 407.f));
	g4->scale(sf::Vector2f(2.3f, 2.3f));
	g4->desactivate();
	node_vector.push_back(g4.get());
	layers[0]->addChild(std::move(g4));

	std::vector<sf::Vector2f> small_dot_pos = labyrinth.getObjectGroup().small_dots_positions;

	for (auto &itr : small_dot_pos)
	{
		std::unique_ptr<PickupObject> small_dot(new PickupObject(Pickup::SmallDot, Textures::SmallDot, texture_generator));
		pick_ups.push_back(small_dot.get());

		//Object groups are placed on the downleft corner so we correct the position to place it in the center of the tile
		//+12 to the right and -12 up
		float x_corrected = itr.x * 2.f + labyrinth.getPosition().x + labyrinth.tmx_info.tile_size.x;
		float y_corrected = itr.y * 2.f + labyrinth.getPosition().y - labyrinth.tmx_info.tile_size.y;
		small_dot->setPosition(sf::Vector2f(x_corrected, y_corrected));
		node_vector.push_back(small_dot.get());
		layers[0]->addChild(std::move(small_dot));
	}

	std::vector<sf::Vector2f> big_dot_pos = labyrinth.getObjectGroup().big_dots_positions;

	for (auto &itr : big_dot_pos)
	{
		std::unique_ptr<PickupObject> big_dot(new PickupObject(Pickup::BigDot, Textures::BigDot, texture_generator));
		pick_ups.push_back(big_dot.get());

		float x_corrected = itr.x * 2.f + labyrinth.getPosition().x + labyrinth.tmx_info.tile_size.x;
		float y_corrected = itr.y * 2.f + labyrinth.getPosition().y - labyrinth.tmx_info.tile_size.y;
		big_dot->setPosition(sf::Vector2f(x_corrected, y_corrected));
		big_dot->scale(sf::Vector2f(2.f, 2.f));
		node_vector.push_back(big_dot.get());
		layers[0]->addChild(std::move(big_dot));
	}

	std::vector<sf::FloatRect> warp_rect = labyrinth.getObjectGroup().warp_rects;
	std::vector<sf::Vector2f>  warp_dest = labyrinth.getObjectGroup().warp_destinies;

	for (unsigned int i = 0; i < warp_rect.size(); ++i)
	{
		std::unique_ptr<Warp> warp(new Warp(sf::RectangleShape(sf::Vector2f(warp_rect[i].width, warp_rect[i].height))));
		float x_corrected = warp_rect[i].left * 2.f + labyrinth.getPosition().x;
		float y_corrected = warp_rect[i].top  * 2.f + labyrinth.getPosition().y;
		float x_destiny_corrected = warp_dest[i].x * 2.f + labyrinth.getPosition().x;
		float y_destiny_corrected = warp_dest[i].y * 2.f + labyrinth.getPosition().y;
		warp->setPosition(sf::Vector2f(x_corrected, y_corrected));
		warp->setDestiny(sf::Vector2f(x_destiny_corrected, y_destiny_corrected));
		warp->scale(sf::Vector2f(2.f, 2.f));
		node_vector.push_back(warp.get());
		layers[0]->addChild(std::move(warp));
	}

	std::unique_ptr<Text> high_score_text(new Text("HIGH SCORE", "fonts/Press Start Regular.ttf", 15));
	high_score_text->setPosition(sf::Vector2f(335.f, 20.f));
	node_vector.push_back(high_score_text.get());
	layers[0]->addChild(std::move(high_score_text));

	std::unique_ptr<Text> high_score(new Text("260", "fonts/Press Start Regular.ttf", 15, TextType::HighScore));
	high_score->setPosition(sf::Vector2f(380.f, 45.f));
	node_vector.push_back(high_score.get());
	layers[0]->addChild(std::move(high_score));

	std::unique_ptr<Text> one_up(new Text("1UP", "fonts/Press Start Regular.ttf", 15));
	one_up->setPosition(sf::Vector2f(160.f, 20.f));
	node_vector.push_back(one_up.get());
	layers[0]->addChild(std::move(one_up));

	std::unique_ptr<Text> score(new Text("00", "fonts/Press Start Regular.ttf", 15, TextType::Score));
	score->setPosition(sf::Vector2f(160.f, 45.f));
	node_vector.push_back(score.get());
	layers[0]->addChild(std::move(score));

	std::unique_ptr<Text> player_one(new Text("PLAYER ONE", "fonts/Press Start Regular.ttf", 22, TextType::Static, sf::Color(0,255,255,255)));
	player_one->setPosition(sf::Vector2f(290.f, 345.f));
	player_one->setLifeTime(0.f);
	node_vector.push_back(player_one.get());
	layers[0]->addChild(std::move(player_one));

	std::unique_ptr<Text> ready(new Text("READY!", "fonts/Press Start Regular.ttf", 22, TextType::Static, sf::Color(255, 255, 0, 255)));
	ready->setPosition(sf::Vector2f(335.f, 490.f));
	ready->setLifeTime(0.f);
	node_vector.push_back(ready.get());
	layers[0]->addChild(std::move(ready));


}

void World::restartPositions()
{
	Command ghost_restart;
	ghost_restart.category = Category::Ghost;
	ghost_restart.action = get_function<Ghost>([](Ghost &ghost, sf::Time dt)
	{
		ghost.setCurrentDirection(Direction::None);
		ghost.restartPosition();
	});

	Command pacman_restart;
	pacman_restart.category = Category::Pacman;
	pacman_restart.action = get_function<Pacman>([](Pacman &pacman, sf::Time dt)
	{
		pacman.restart();
	});

	command_queue.push(ghost_restart);
	command_queue.push(pacman_restart);	
}

// Check for the objects marked as removable and remove it from the scene
void World::deleteActors()
{
	scene_graph.deleteRemovable(node_vector);	
}