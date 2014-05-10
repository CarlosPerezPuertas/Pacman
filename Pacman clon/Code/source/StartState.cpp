#include "..\header\StartState.h"


StartState::StartState(StateStack &c_state_stack, sf::RenderWindow &c_window, Player &c_player, TextureGenerator &c_texture_factory)
: State(c_state_stack, c_window, c_player, c_texture_factory)
, time_to_press_key(sf::seconds(3.f))
, key_to_continue(false)
, player(c_player)
{
	/*t_pacman_logo.loadFromFile("textures/pacman start logo.png");
	s_pacman_logo.setTexture(t_pacman_logo);
	s_pacman_logo.scale(sf::Vector2f(1.3f, 1.3f));
	s_pacman_logo.setPosition(sf::Vector2f(10.f, 200.f));*/

	loadResources();
	createScene();

}


StartState::~StartState()
{
}

void StartState::readInput()
{
	if (time_to_press_key <= sf::Time::Zero)
	{
		if (isKeyPressed()) key_to_continue = true;
	}
}

void StartState::loadResources()
{
	texture_generator.load(Textures::StartLogo, "textures/pacman start logo.png");
}

void StartState::createScene()
{
	SceneNode::PtrNode layer(new SceneNode());
	layers.push_back(layer.get());
	scene_graph.addChild(std::move(layer));

	std::unique_ptr<Image> pacman_logo(new Image(Textures::StartLogo, texture_generator));
	pacman_logo->scale(sf::Vector2f(1.f, 1.f));
	pacman_logo->setPosition(sf::Vector2f(0.f, -50.f));
	pacman_logo->moveDuring(sf::Vector2f(0.f, 1.f), 2.5f);
	layers[0]->addChild(std::move(pacman_logo));

	std::unique_ptr<Text> press_key(new Text("PRESS ANY KEY TO START", "fonts/Press Start Regular.ttf", 20));
	press_key->setPosition(sf::Vector2f(135.f, 600.f));
	press_key->desactivate(2.5f);
	layers[0]->addChild(std::move(press_key));
}


void StartState::draw()
{
	getWindow().draw(scene_graph);
}

void StartState::update(sf::Time dt)
{
	scene_graph.update(command_queue, dt);

	time_to_press_key -= dt;

	if (key_to_continue == true)
	{
		setTransitionState(Transitions::OnExit);
	}


}

void StartState::enterUpdate(sf::Time dt)
{
	setTransitionState(Transitions::None);
	player.enable();

}

void StartState::exitUpdate(sf::Time dt)
{
	requestPop();
	requestPush(StateName::Game);
	setTransitionState(Transitions::OnEnter);
	player.disable();

	std::cout << "Start Menu to onEnter Game" << std::endl;
}

void StartState::enterDraw()
{

}

void StartState::exitDraw()
{

}


