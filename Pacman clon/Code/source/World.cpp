#include "..\header\World.h"


World::World(sf::RenderWindow &c_window, Player &c_player)
: window(c_window)
, player(c_player)
, pacman(nullptr)
, game_over(nullptr)
, lifes(nullptr)
, bonus(nullptr)
, labyrinth()
, scene_graph()
, command_queue()
, collider(*this, labyrinth, command_queue, player)
, isGameOver(false)
, total_dots(247)
, remaining_dots(total_dots)
, num_level(1)
, first_fruit_created(false)
, second_fruit_created(false)
, score_1up(0)
{
	restartLogic();
}


void World::update(const sf::Time dt)
{
	//Broadcast callbacks
	while (command_queue.isEmpty() == false)
	{
		scene_graph.recieveCommand(command_queue.pop(), dt);
	}

	labyrinth.update(dt);
	if (pacman != nullptr)   collider.checkMapCollision(*pacman);
	for (auto &itr : ghosts) collider.checkMapCollision(*itr); 
	scene_graph.update(command_queue, dt);
	collider.checkSceneCollision(node_vector);

	runLogic();
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
	texture_generator.load(Textures::Life, "textures/life.png");
	texture_generator.load(Textures::Cherry, "textures/cherry.png");
	texture_generator.load(Textures::Strawerry, "textures/strawerry.png");
	texture_generator.load(Textures::Peach, "textures/peach.png");
	texture_generator.load(Textures::Apple, "textures/apple.png");
	texture_generator.load(Textures::Key, "textures/key.png");
	texture_generator.load(Textures::Galaxian, "textures/galaxian.png");
	texture_generator.load(Textures::Grapes, "textures/grapes.png");
	texture_generator.load(Textures::Bell, "textures/bell.png");

	sounds.load(SoundEffect::EatPickup, "sfx/Pacman Eating Cherry.wav" );
	sounds.load(SoundEffect::PacmanDies, "sfx/Pacman Dies.wav");
	sounds.load(SoundEffect::EatGhost, "sfx/Pacman Eating Ghost.wav");
	sounds.load(SoundEffect::Waka1, "sfx/waka1.wav");
	sounds.load(SoundEffect::Waka2, "sfx/waka2.wav");
	sounds.load(SoundEffect::Siren, "sfx/Pacman Siren.wav");
	sounds.load(SoundEffect::OneUp, "sfx/Pacman Extra Live.wav");
	sounds.load(SoundEffect::BigDot, "sfx/large+pellet+loop.wav");
	sounds.load(SoundEffect::Opening, "sfx/Pacman Opening Song.wav");
}

void World::create_scene()
{
	labyrinth.load("maps/pacman_board.tmx");

	SceneNode::PtrNode layer1(new SceneNode());
	layers.push_back(layer1.get());
	scene_graph.addChild(std::move(layer1));

	SceneNode::PtrNode layer2(new SceneNode());
	layers.push_back(layer2.get());
	scene_graph.addChild(std::move(layer2));

	std::unique_ptr<Pacman> pac(new Pacman(Textures::Pacman, texture_generator, player));
	pacman = pac.get();
	pacman->scale(sf::Vector2f(2.5f, 2.5f));
	pacman->setPosition(sf::Vector2f(360.f, 587.f));
	pacman->setInitPos(sf::Vector2f(360.f, 587.f));
	pacman->setRealRect(sf::FloatRect(0.f, 0.f, 2.f, 2.f));
	pacman->setSpeed(speed, 0.8f);
	pacman->setVelRatio(0.8f);
	pac->desactivate(4.8f);
	node_vector.push_back(pacman);
	layers[1]->addChild(std::move(pac));

	std::unique_ptr<Ghost> g1(new Ghost(GhostType::Blinky, Textures::Ghost1, texture_generator));
	blinky = g1.get();
	g1->scale(sf::Vector2f(2.5f, 2.5f));
	g1->setPosition(sf::Vector2f(360.f, 299.f));
	g1->setInitPos(sf::Vector2f(360.f, 299.f));
	g1->desactivate();
	g1->setRealRect(sf::FloatRect(0.f, 0.f, 2.f, 2.f)); //Stack overflow when is less square 
	g1->setCurrentDirection(Direction::Right);
	g1->setScatterTarget(59); //Look pacman dossier (Fixed Target Tiles)
	g1->setCaveReturnTarget(375);
	g1->setSpeed(speed, 0.75f);
	g1->setVelRatio(0.75f);
	g1->setCaveRatio(0.4f);
	g1->setFrightenedRatio(0.5f);
	g1->setFrightenedTime(6.f);
	g1->desactivate(4.8f);
	ghosts.push_back(g1.get());
	node_vector.push_back(g1.get());
	layers[1]->addChild(std::move(g1));

	std::unique_ptr<Ghost> g2(new Ghost(GhostType::Pinky, Textures::Ghost2, texture_generator));
	g2->setPosition(sf::Vector2f(372.f, 371.f));
	g2->setInitPos(sf::Vector2f(372.f, 371.f));
	g2->scale(sf::Vector2f(2.5f, 2.5f));
	g2->setRealRect(sf::FloatRect(0.f, 0.f, 2.f, 2.f));
	g2->desactivate();
	g2->setScatterTarget(0); //Look pacman dossier (Fixed Target Tiles)
	g2->setCaveReturnTarget(375);
	g2->setCurrentDirection(Direction::Left);
	g2->setSpeed(speed, 0.75f);
	g2->setVelRatio(0.75f);
	g2->setCaveRatio(0.4f);
	g2->setFrightenedRatio(0.5f);
	g2->setExitCaveTime(1);
	g2->setFrightenedTime(6.f);
	g2->desactivate(4.8f);
	ghosts.push_back(g2.get());
	node_vector.push_back(g2.get());
	layers[1]->addChild(std::move(g2));

	std::unique_ptr<Ghost> g3(new Ghost(GhostType::Inky, Textures::Ghost3, texture_generator));
	g3->setPosition(sf::Vector2f(312.f, 371.f));
	g3->setInitPos(sf::Vector2f(312.f, 371.f));
	g3->scale(sf::Vector2f(2.5f, 2.5f));
	g3->setRealRect(sf::FloatRect(0.f, 0.f, 2.f, 2.f));
	g3->desactivate();
	g3->setScatterTarget(988); //Look pacman dossier (Fixed Target Tiles)
	g3->setCaveReturnTarget(375);
	g3->setCurrentDirection(Direction::Right);
	g3->setSpeed(speed, 0.75f);
	g3->setVelRatio(0.75f);
	g3->setCaveRatio(0.4f);
	g3->setFrightenedRatio(0.5f);
	g3->setExitCaveTime(6);
	g3->setFrightenedTime(6.f);
	g3->desactivate(4.8f);
	ghosts.push_back(g3.get());
	node_vector.push_back(g3.get());
	layers[1]->addChild(std::move(g3));

	std::unique_ptr<Ghost> g4(new Ghost(GhostType::Clyde, Textures::Ghost4, texture_generator));
	g4->setPosition(sf::Vector2f(408.f, 371.f));
	g4->setInitPos(sf::Vector2f(408.f, 371.f));
	g4->scale(sf::Vector2f(2.5f, 2.5f));
	g4->setRealRect(sf::FloatRect(0.f, 0.f, 2.f, 2.f));
	g4->desactivate();
	g4->setScatterTarget(961); //Look pacman dossier (Fixed Target Tiles)
	g4->setCaveReturnTarget(375);
	g4->setCurrentDirection(Direction::Left);
	g4->setSpeed(speed, 0.75f);
	g4->setVelRatio(0.75f);
	g4->setCaveRatio(0.4f);
	g4->setFrightenedRatio(0.5f);
	g4->setExitCaveTime(8);
	g4->setFrightenedTime(6.f);
	g4->desactivate(4.8f);
	ghosts.push_back(g4.get());
	node_vector.push_back(g4.get());
	layers[1]->addChild(std::move(g4));

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
	high_score_text->setPosition(sf::Vector2f(380.f, -40.f));
	node_vector.push_back(high_score_text.get());
	layers[0]->addChild(std::move(high_score_text));

	std::unique_ptr<Text> high_score(new Text(std::to_string(pacman->getHighScore()), "fonts/Press Start Regular.ttf", 15, TextType::HighScore));
	high_score->setPosition(sf::Vector2f(410.f, -10.f));
	node_vector.push_back(high_score.get());
	layers[0]->addChild(std::move(high_score));

	std::unique_ptr<Text> one_up(new Text("1UP", "fonts/Press Start Regular.ttf", 15));
	one_up->setPosition(sf::Vector2f(220.f, -40.f));
	node_vector.push_back(one_up.get());
	layers[0]->addChild(std::move(one_up));

	std::unique_ptr<Text> score(new Text("00", "fonts/Press Start Regular.ttf", 15, TextType::Score));
	score->setPosition(sf::Vector2f(220.f, -10.f));
	node_vector.push_back(score.get());
	layers[0]->addChild(std::move(score));

	std::unique_ptr<Text> player_one(new Text("PLAYER ONE", "fonts/Press Start Regular.ttf", 22, TextType::Static, sf::Color(0,255,255,255)));
	player_one->setPosition(sf::Vector2f(250.f, 285.f));
	player_one->setLifeTime(4.8f);
	node_vector.push_back(player_one.get());
	layers[0]->addChild(std::move(player_one));

	std::unique_ptr<Text> ready(new Text("READY!", "fonts/Press Start Regular.ttf", 22, TextType::Static, sf::Color(255, 255, 0, 255)));
	ready->setPosition(sf::Vector2f(295.f, 430.f));
	ready->setLifeTime(4.8f);
	node_vector.push_back(ready.get());
	layers[0]->addChild(std::move(ready));

	std::unique_ptr<ImageContainer> lifes_container(new ImageContainer(ContainerType::Lifes));
	lifes = lifes_container.get();
	lifes_container->setContainerMode(ContainerMode::Horizontal);
	lifes_container->setPosition(sf::Vector2f(80.f, 780.f));
	lifes_container->scale(sf::Vector2f(2.f, 2.f));
	lifes_container->pushBack(Textures::Life, texture_generator);
	lifes_container->pushBack(Textures::Life, texture_generator);
	node_vector.push_back(lifes_container.get());
	layers[0]->addChild(std::move(lifes_container));

	std::unique_ptr<ImageContainer> bonus_container(new ImageContainer(ContainerType::Pickups));
	bonus = bonus_container.get();
	bonus_container->setContainerMode(ContainerMode::Horizontal);
	bonus_container->setPosition(sf::Vector2f(660.f, 780.f));
	bonus_container->scale(sf::Vector2f(2.f, 2.f));
	node_vector.push_back(bonus_container.get());
	layers[0]->addChild(std::move(bonus_container));
	nextBonus();

	//Insert dots
	restartDots();

	std::unique_ptr<SoundNode> sound_eat_pickup(new SoundNode(SoundEffect::EatPickup, sounds));
	node_vector.push_back(sound_eat_pickup.get());
	layers[0]->addChild(std::move(sound_eat_pickup));

	std::unique_ptr<SoundNode> sound_pacman_dies(new SoundNode(SoundEffect::PacmanDies, sounds));
	node_vector.push_back(sound_pacman_dies.get());
	layers[0]->addChild(std::move(sound_pacman_dies));

	std::unique_ptr<SoundNode> sound_waka1(new SoundNode(SoundEffect::Waka1, SoundEffect::Waka2, sounds));
	node_vector.push_back(sound_waka1.get());
	layers[0]->addChild(std::move(sound_waka1));

	std::unique_ptr<SoundNode> sound_siren(new SoundNode(SoundEffect::Siren, sounds));
	node_vector.push_back(sound_siren.get());
	sound_siren->playAfter(4.8f);
	sound_siren->setLoop(true);
	layers[0]->addChild(std::move(sound_siren));

	std::unique_ptr<SoundNode> sound_eat_ghost(new SoundNode(SoundEffect::EatGhost, sounds));
	node_vector.push_back(sound_eat_ghost.get());
	layers[0]->addChild(std::move(sound_eat_ghost));

	std::unique_ptr<SoundNode> sound_one_up(new SoundNode(SoundEffect::OneUp, sounds));
	node_vector.push_back(sound_one_up.get());
	layers[0]->addChild(std::move(sound_one_up));

	std::unique_ptr<SoundNode> sound_big_dot(new SoundNode(SoundEffect::BigDot, sounds));
	node_vector.push_back(sound_big_dot.get());
	layers[0]->addChild(std::move(sound_big_dot));

	std::unique_ptr<SoundNode> sound_opening(new SoundNode(SoundEffect::Opening, sounds));
	node_vector.push_back(sound_opening.get());
	sound_opening->play();
	layers[0]->addChild(std::move(sound_opening));
}

//We register the pickup objects that will be displayed depending on the level
//The info (scores and levels) is taken from : http://nrchapman.com/pacman/
void World::registerPickups()
{
	pickup_factory[1] = [this]()
	{
		std::unique_ptr<PickupObject> pickup(new PickupObject(Pickup::Cherry, Textures::Cherry, texture_generator));
		pickup->setLifeTime(6.f);
		pickup->setPosition(sf::Vector2f(350.f, 435.f));
		pickup->scale(sf::Vector2f(2.3f, 2.3f));
		pickup->setScore(100);
		return pickup;
	};

	pickup_factory[2] = [this]()
	{
		std::unique_ptr<PickupObject> pickup(new PickupObject(Pickup::Strawerry, Textures::Strawerry, texture_generator));
		pickup->setLifeTime(6.f);
		pickup->setPosition(sf::Vector2f(350.f, 435.f));
		pickup->scale(sf::Vector2f(2.3f, 2.3f));
		pickup->setScore(300);
		return pickup;
	};

	pickup_factory[3] = [this]()
	{
		std::unique_ptr<PickupObject> pickup(new PickupObject(Pickup::Peach, Textures::Peach, texture_generator));
		pickup->setLifeTime(6.f);
		pickup->setPosition(sf::Vector2f(350.f, 435.f));
		pickup->scale(sf::Vector2f(2.3f, 2.3f));
		pickup->setScore(500);
		return pickup;
	};

	pickup_factory[4] = [this]()
	{
		std::unique_ptr<PickupObject> pickup(new PickupObject(Pickup::Peach, Textures::Peach, texture_generator));
		pickup->setLifeTime(6.f);
		pickup->setPosition(sf::Vector2f(350.f, 435.f));
		pickup->scale(sf::Vector2f(2.3f, 2.3f));
		pickup->setScore(500);
		return pickup;
	};

	pickup_factory[5] = [this]()
	{
		std::unique_ptr<PickupObject> pickup(new PickupObject(Pickup::Apple, Textures::Apple, texture_generator));
		pickup->setLifeTime(6.f);
		pickup->setPosition(sf::Vector2f(350.f, 435.f));
		pickup->scale(sf::Vector2f(2.3f, 2.3f));
		pickup->setScore(700);
		return pickup;
	};

	pickup_factory[6] = [this]()
	{
		std::unique_ptr<PickupObject> pickup(new PickupObject(Pickup::Apple, Textures::Apple, texture_generator));
		pickup->setLifeTime(6.f);
		pickup->setPosition(sf::Vector2f(350.f, 435.f));
		pickup->scale(sf::Vector2f(2.3f, 2.3f));
		pickup->setScore(700);
		return pickup;
	};

	pickup_factory[7] = [this]()
	{
		std::unique_ptr<PickupObject> pickup(new PickupObject(Pickup::Grapes, Textures::Grapes, texture_generator));
		pickup->setLifeTime(6.f);
		pickup->setPosition(sf::Vector2f(350.f, 435.f));
		pickup->scale(sf::Vector2f(2.3f, 2.3f));
		pickup->setScore(1000);
		return pickup;
	};

	pickup_factory[8] = [this]()
	{
		std::unique_ptr<PickupObject> pickup(new PickupObject(Pickup::Grapes, Textures::Grapes, texture_generator));
		pickup->setLifeTime(6.f);
		pickup->setPosition(sf::Vector2f(350.f, 435.f));
		pickup->scale(sf::Vector2f(2.3f, 2.3f));
		pickup->setScore(1000);
		return pickup;
	};

	pickup_factory[9] = [this]()
	{
		std::unique_ptr<PickupObject> pickup(new PickupObject(Pickup::Galaxian, Textures::Galaxian, texture_generator));
		pickup->setLifeTime(6.f);
		pickup->setPosition(sf::Vector2f(350.f, 435.f));
		pickup->scale(sf::Vector2f(2.3f, 2.3f));
		pickup->setScore(2000);
		return pickup;
	};

	pickup_factory[10] = [this]()
	{
		std::unique_ptr<PickupObject> pickup(new PickupObject(Pickup::Galaxian, Textures::Galaxian, texture_generator));
		pickup->setLifeTime(6.f);
		pickup->setPosition(sf::Vector2f(350.f, 435.f));
		pickup->scale(sf::Vector2f(2.3f, 2.3f));
		pickup->setScore(2000);
		return pickup;
	};

	pickup_factory[11] = [this]()
	{
		std::unique_ptr<PickupObject> pickup(new PickupObject(Pickup::Bell, Textures::Bell, texture_generator));
		pickup->setLifeTime(6.f);
		pickup->setPosition(sf::Vector2f(350.f, 435.f));
		pickup->scale(sf::Vector2f(2.3f, 2.3f));
		pickup->setScore(3000);
		return pickup;
	};

	pickup_factory[12] = [this]()
	{
		std::unique_ptr<PickupObject> pickup(new PickupObject(Pickup::Bell, Textures::Bell, texture_generator));
		pickup->setLifeTime(6.f);
		pickup->setPosition(sf::Vector2f(350.f, 435.f));
		pickup->scale(sf::Vector2f(2.3f, 2.3f));
		pickup->setScore(3000);
		return pickup;
	};

	pickup_factory[13] = [this]()
	{
		std::unique_ptr<PickupObject> pickup(new PickupObject(Pickup::Key, Textures::Key, texture_generator));
		pickup->setLifeTime(6.f);
		pickup->setPosition(sf::Vector2f(350.f, 435.f));
		pickup->scale(sf::Vector2f(2.3f, 2.3f));
		pickup->setScore(5000);
		return pickup;
	};
}

void World::restartPositions()
{
	Command ghost_restart;
	ghost_restart.category = Category::Ghost;
	ghost_restart.action = get_function<Ghost>([this](Ghost &ghost, sf::Time dt)
	{
		ghost.restart();


		if (num_level == 1) 
		{ 
			ghost.setCaveRatio(0.4f);
			ghost.setFrightenedTime(6.f);
			ghost.setFrightenedRatio(0.5f);
			ghost.setSpeed(speed, 0.75f);
			ghost.setVelRatio(0.75f);
			
		}
		else if (num_level > 1 && num_level <= 4)
		{
			ghost.setCaveRatio(0.45f);
			ghost.setFrightenedRatio(0.55f);
			ghost.setSpeed(speed, 0.85f);
			ghost.setVelRatio(0.85f);
		}
		else if (num_level > 4)
		{
			ghost.setCaveRatio(0.5f);
			ghost.setFrightenedRatio(0.6f);
			ghost.setSpeed(speed, 0.95f);
			ghost.setVelRatio(0.95f);
		}

		if (num_level == 2) ghost.setFrightenedTime(5.5f);
		else if (num_level == 3) ghost.setFrightenedTime(5.2f);
		else if (num_level == 4) ghost.setFrightenedTime(5.f);
		else if (num_level == 5) ghost.setFrightenedTime(4.6f);
		else if (num_level == 6) ghost.setFrightenedTime(4.f);
		else if (num_level == 7) ghost.setFrightenedTime(5.f);
		else if (num_level == 8) ghost.setFrightenedTime(3.5f);
		else if (num_level == 9) ghost.setFrightenedTime(3.f);
		else if (num_level == 10) ghost.setFrightenedTime(4.f);
		else if (num_level == 11) ghost.setFrightenedTime(3.f);
		else if (num_level == 12) ghost.setFrightenedTime(2.8f);
		else if (num_level == 13) ghost.setFrightenedTime(3.5f);
		else if (num_level == 14) ghost.setFrightenedTime(2.7f);
		else if (num_level >= 15) ghost.setFrightenedTime(2.5f);


		if (remaining_dots < 247)
		{
			if (ghost.getType() == GhostType::Blinky) ghost.setExitCaveTime(0.1f);
			else  ghost.setExitCaveTime(6.f);
		}
		else
		{
			if (num_level % 3 == 1)
			{
				if (ghost.getType() == GhostType::Clyde) ghost.setExitCaveTime(7.f);
				else if (ghost.getType() == GhostType::Inky) ghost.setExitCaveTime(6.f);
				else if (ghost.getType() == GhostType::Pinky) ghost.setExitCaveTime(0.5f);
			}
			else if (num_level % 3 == 2)
			{
				if (ghost.getType() == GhostType::Clyde) ghost.setExitCaveTime(7.f);
				else if (ghost.getType() == GhostType::Inky) ghost.setExitCaveTime(0.5f);
				else if (ghost.getType() == GhostType::Pinky) ghost.setExitCaveTime(0.5f);
			}
			else if (num_level % 3 == 0)
			{
				if (ghost.getType() == GhostType::Clyde) ghost.setExitCaveTime(0.5f);
				else if (ghost.getType() == GhostType::Inky) ghost.setExitCaveTime(0.5f);
				else if (ghost.getType() == GhostType::Pinky) ghost.setExitCaveTime(0.5f);
			}
		}
	});

	Command pacman_restart;
	pacman_restart.category = Category::Pacman;
	pacman_restart.action = get_function<Pacman>([this](Pacman &pacman, sf::Time dt)
	{
		pacman.restart();
		player.enable();

		if (num_level == 1) pacman.setSpeed(speed, 0.8f);
		else if (num_level > 1 && num_level <= 4)
		{
			pacman.setSpeed(speed, 0.9f);
			pacman.setVelRatio(0.9f);
		}
		else if (num_level > 4 && num_level <= 20)
		{
			pacman.setSpeed(speed, 1.f);
			pacman.setVelRatio(1.f);
		}
		else if (num_level > 20)
		{
			pacman.setSpeed(speed, 0.9f);
			pacman.setVelRatio(0.9f);
		}
		
	});

	command_queue.push(pacman_restart);	
	command_queue.push(ghost_restart);
}

void World::restartLogic()
{
	remaining_dots = total_dots;
	first_fruit_created = false;
	second_fruit_created = false;
	player.initAction(Action::GoLeft);

	Command elroy_off;
	elroy_off.category = Category::Ghost;
	elroy_off.action = get_function<Ghost>([this](Ghost &ghost, sf::Time dt)
	{
		if (ghost.getType() == GhostType::Blinky)
		{
			ghost.setElroy2(false);
			ghost.setElroy1(false);
		}

	});

	command_queue.push(elroy_off);
}


void World::restartDots()
{
	std::vector<sf::Vector2f> small_dot_pos = labyrinth.getObjectGroup().small_dots_positions;

	for (auto &itr : small_dot_pos)
	{
		std::unique_ptr<PickupObject> small_dot(new PickupObject(Pickup::SmallDot, Textures::SmallDot, texture_generator));

		//Object groups are placed on the downleft corner so we correct the position to place it in the center of the tile
		//+12 to the right and -12 up
		float x_corrected = itr.x * 2.f + labyrinth.getPosition().x + labyrinth.tmx_info.tile_size.x;
		float y_corrected = itr.y * 2.f + labyrinth.getPosition().y - labyrinth.tmx_info.tile_size.y;
		small_dot->setPosition(sf::Vector2f(x_corrected, y_corrected));
		small_dot->setScore(10);
		small_dot->setRealRect(sf::FloatRect(0.f, 0.f, 2.f, 2.f));
		node_vector.push_back(small_dot.get());
		layers[0]->addChild(std::move(small_dot));
	}

	std::vector<sf::Vector2f> big_dot_pos = labyrinth.getObjectGroup().big_dots_positions;

	for (auto &itr : big_dot_pos)
	{
		std::unique_ptr<PickupObject> big_dot(new PickupObject(Pickup::BigDot, Textures::BigDot, texture_generator));

		float x_corrected = itr.x * 2.f + labyrinth.getPosition().x + labyrinth.tmx_info.tile_size.x;
		float y_corrected = itr.y * 2.f + labyrinth.getPosition().y - labyrinth.tmx_info.tile_size.y;
		big_dot->setPosition(sf::Vector2f(x_corrected, y_corrected));
		big_dot->scale(sf::Vector2f(2.f, 2.f));
		big_dot->setScore(50);
		node_vector.push_back(big_dot.get());
		layers[0]->addChild(std::move(big_dot));
	}


}

void World::nextBonus()
{
	switch (num_level)
	{
		case 1:
			bonus->pushFront(Textures::Cherry, texture_generator);
			break;
		case 2:
			bonus->pushFront(Textures::Strawerry, texture_generator);
			break;
		case 3:
			bonus->pushFront(Textures::Peach, texture_generator);
			break;
		case 4:
			bonus->pushFront(Textures::Peach, texture_generator);
			break;
		case 5:
			bonus->pushFront(Textures::Apple, texture_generator);
			break;
		case 6:
			bonus->pushFront(Textures::Apple, texture_generator);
			break;
		case 7:
			bonus->pushFront(Textures::Grapes, texture_generator);
			break;
		case 8:
			bonus->pushFront(Textures::Grapes, texture_generator);
			break;
		case 9:
			bonus->pushFront(Textures::Galaxian, texture_generator);
			break;
		case 10:
			bonus->pushFront(Textures::Galaxian, texture_generator);
			break;
		case 11:
			bonus->pushFront(Textures::Bell, texture_generator);
			break;
		case 12:
			bonus->pushFront(Textures::Bell, texture_generator);
			break;
		case 13:
			bonus->pushFront(Textures::Key, texture_generator);
			break;
		case 14:
			bonus->pushFront(Textures::Key, texture_generator);
			break;
		case 15:
			bonus->pushFront(Textures::Key, texture_generator);
			break;
		case 16:
			bonus->pushFront(Textures::Key, texture_generator);
			break;
		case 17:
			bonus->pushFront(Textures::Key, texture_generator);
			break;
		case 18:
			bonus->pushFront(Textures::Key, texture_generator);
			break;
		case 19:
			bonus->pushFront(Textures::Key, texture_generator);
			break;

		default:
			break;

	}
}

void World::nextLevel()
{
	num_level++;

	nextBonus();
	restartLogic();
	restartPositions();
	restartDots();

	Command siren_on;
	siren_on.category = Category::SoundSiren;
	siren_on.action = get_function<SoundNode>([this](SoundNode &sound, sf::Time dt)
	{
		sound.playAfter(3.35f);
		sound.setLoop(true);
	});
	command_queue.push(siren_on);

	std::unique_ptr<Text> ready(new Text("READY!", "fonts/Press Start Regular.ttf", 22, TextType::Static, sf::Color(255, 255, 0, 255)));
	ready->setPosition(sf::Vector2f(295.f, 430.f));
	ready->setLifeTime(3.35f);
	node_vector.push_back(ready.get());
	layers[0]->addChild(std::move(ready));

	labyrinth.disable(1.f);
	scene_graph.desactivate(1.f);
	pacman->desactivate(1.f);
}

void World::runLogic()
{
	
	//Blinky changes speed depending on the remaining dots. See Pacman Dossier.
	if (remaining_dots == 40)
	{
		Command elroy1On;
		elroy1On.category = Category::Ghost;
		elroy1On.action = get_function<Ghost>([&](Ghost &g, sf::Time dt)
		{
			if (g.getType() == GhostType::Blinky && g.isElroy1() == false)
			{  
				std::cout << "ELROY 1 ON" << std::endl;
				g.setElroy1(true);
			}
		});
		command_queue.push(elroy1On);
	}
	else if (remaining_dots == 20)
	{
		Command elroy2On;
		elroy2On.category = Category::Ghost;
		elroy2On.action = get_function<Ghost>([&](Ghost &g, sf::Time dt)
		{
			if (g.getType() == GhostType::Blinky && g.isElroy2() == false)
			{
				std::cout << "ELROY 2 ON" << std::endl;
				g.setElroy1(false);
				g.setElroy2(true);
			}
		});
		command_queue.push(elroy2On);
	}

	if ((total_dots - remaining_dots + 1)  % 71 == 0 && first_fruit_created == false)
	{
		first_fruit_created = true;
		int id = num_level;
		if (num_level > 13) id = 13;
		std::unique_ptr<PickupObject> pick = pickup_factory[id]();
		node_vector.push_back(pick.get());
		layers[0]->addChild(std::move(pick));
	}
	else if ((total_dots - remaining_dots + 1) % 171 == 0 && second_fruit_created == false)
	{
		second_fruit_created = true;
		int id = num_level;
		if (num_level > 13) id = 13;
		std::unique_ptr<PickupObject> pick = pickup_factory[id]();
		node_vector.push_back(pick.get());
		layers[0]->addChild(std::move(pick));
	}

	if (remaining_dots == 0)
	{
		Command ghosts_initial_state;
		ghosts_initial_state.category = Category::Ghost;
		ghosts_initial_state.action = get_function<Ghost>([&](Ghost &g, sf::Time dt)
		{
			g.changeState(GhostBehaviour::Chase);
		});

		command_queue.push(ghosts_initial_state);

		Command siren_off;
		siren_off.category = Category::SoundSiren;
		siren_off.action = get_function<SoundNode>([this](SoundNode &sound, sf::Time dt)
		{
			sound.stop();
			sound.setLoop(false);
		});
		command_queue.push(siren_off);

		Command siren2_off;
		siren2_off.category = Category::SoundBigDot;
		siren2_off.action = get_function<SoundNode>([this](SoundNode &sound, sf::Time dt)
		{
			sound.stop();
			sound.setLoop(false);
		});
		command_queue.push(siren2_off);

		nextLevel();
	}
	else if (pacman->isDeadAnimationEnd())
	{
		if (pacman->hasLifes())
		{
			std::unique_ptr<Text> ready(new Text("READY!", "fonts/Press Start Regular.ttf", 22, TextType::Static, sf::Color(255, 255, 0, 255)));
			ready->setPosition(sf::Vector2f(295.f, 430.f));
			ready->setLifeTime(3.35f);
			node_vector.push_back(ready.get());
			layers[0]->addChild(std::move(ready));
			
			restartPositions();

			Command siren_on;
			siren_on.category = Category::SoundSiren;
			siren_on.action = get_function<SoundNode>([this](SoundNode &sound, sf::Time dt)
			{
				sound.playAfter(3.35f);
				sound.setLoop(true);
			});
			command_queue.push(siren_on);
		}	
		else
		{
			if (isGameOver == false)
			{
				std::unique_ptr<Text> go(new Text("GAME OVER", "fonts/Press Start Regular.ttf", 22, TextType::Static, sf::Color::Red));
				game_over = go.get();
				go->setPosition(sf::Vector2f(265.f, 430.f));
				go->setLifeTime(3.f);
				node_vector.push_back(go.get());
				layers[0]->addChild(std::move(go));
				isGameOver = true;
			}

			if (game_over == nullptr) std::cout << "Game over end" << std::endl;
		}
	}

	//Earn one life each 10000 points
	if (score_1up >= 10000)
	{
		pacman->earnLife();
		score_1up = score_1up - 10000;

		if (pacman->getLifes() <= 5)
		{
			lifes->pushBack(Textures::Life, texture_generator);
		}

		Command play_liveup;
		play_liveup.category = Category::SoundOneUp;
		play_liveup.action = get_function<SoundNode>([](SoundNode &sound, sf::Time dt)
		{
			sound.play();
		});
		command_queue.push(play_liveup);
	}

	int blinky_tile = 0;

	//Calcule targets for each ghost IA
	for (auto &itr : ghosts)
	{
		if (itr->getType() == GhostType::Blinky)
		{
			blinky_tile = itr->getTileMap();
			itr->setChaseTarget(pacman->getTileMap());
		}
		else if (itr->getType() == GhostType::Pinky && pacman->getCurrentDirection() != Direction::None)
		{
			if (pacman->getCurrentDirection() == Direction::Left)
			{
				int map_x = pacman->getTileMap() % 30;

				if (map_x > 4) itr->setChaseTarget(pacman->getTileMap() - 4);
				else itr->setChaseTarget(pacman->getTileMap() - 2);
			}
			else if (pacman->getCurrentDirection() == Direction::Right)
			{
				int map_x = pacman->getTileMap() % 30;

				if (map_x < 26) itr->setChaseTarget(pacman->getTileMap() + 4);
				else itr->setChaseTarget(pacman->getTileMap() + 2);
			}
			else if (pacman->getCurrentDirection() == Direction::Up)
			{
				int map_y = pacman->getTileMap() / 30;

				if (map_y > 4) itr->setChaseTarget(pacman->getTileMap() - 4*30);
				else itr->setChaseTarget(pacman->getTileMap() - 2*30);
			}

			else if (pacman->getCurrentDirection() == Direction::Down)
			{
				int map_y = pacman->getTileMap() / 30;

				if (map_y < 29) itr->setChaseTarget(pacman->getTileMap() + 4*30);
				else itr->setChaseTarget(pacman->getTileMap() + 2*30);
			}
		}
		else if (itr->getType() == GhostType::Inky && pacman->getCurrentDirection() != Direction::None)
		{
			int mid_tile = 0;
			int pac_x = pacman->getTileMap() % 30;
			int pac_y = pacman->getTileMap() / 30;

			if (pacman->getCurrentDirection() == Direction::Left)
			{
				if (pac_x > 4) mid_tile = pacman->getTileMap() - 4;
				else mid_tile = pacman->getTileMap() - 2;
			}
			else if (pacman->getCurrentDirection() == Direction::Right)
			{
				if (pac_x < 26) mid_tile = pacman->getTileMap() + 4;
				else mid_tile = pacman->getTileMap() + 2;
			}
			else if (pacman->getCurrentDirection() == Direction::Up)
			{
				if (pac_y > 4)  mid_tile = pacman->getTileMap() - 4 * 30;
				else mid_tile = pacman->getTileMap() - 2 * 30;
			}

			else if (pacman->getCurrentDirection() == Direction::Down)
			{
				if (pac_y < 29)  mid_tile = pacman->getTileMap() + 4 * 30;
				else mid_tile = pacman->getTileMap() + 2 * 30;
			}

			int blinky_tile_x = blinky_tile % 30;
			int blinky_tile_y = blinky_tile / 30;
			int H_distance = 0;
			int V_distance = 0;

			H_distance = (mid_tile % 30 - blinky_tile_x) * 2 + blinky_tile_x;
			V_distance = (mid_tile / 30 - blinky_tile_y) * 2 + blinky_tile_y;

			int target = H_distance + V_distance*30;
			itr->setChaseTarget(target);
		}
		else if (itr->getType() == GhostType::Clyde)
		{
			int H_distance = pacman->getTileMap() % 30 - itr->getTileMap() % 30;
			int V_distance = pacman->getTileMap() / 30 - itr->getTileMap() / 30;
			float distance = 0.f;

			distance = sqrt(powf(static_cast<float>(H_distance), 2) + powf(static_cast<float>(V_distance), 2));

			if (distance < 8.f) itr->setChaseTarget(pacman->getTileMap());
			else if (distance >= 8.f) itr->setChaseTarget(itr->getScatterTarget());	
		}
	}
}

bool World::isGameOverEnd()
{
	if (isGameOver == true && game_over == nullptr) { std::cout << "Game over end" << std::endl; return true; }
	return false;
}

// Check for the objects marked as removable and remove it from the scene
void World::deleteActors()
{
	//if (pacman != nullptr)
	if (game_over != nullptr)
	if (game_over->isRemovable()) game_over = nullptr;
		
	//if (pacman->isRemovable()) pacman = nullptr;
	scene_graph.deleteRemovable(node_vector);	
	

}

void World::disableAll(float seconds)
{
	for (auto &itr : node_vector)
	{
		itr->desactivate(seconds);
	}

}