#include "..\header\LogoState.h"



LogoState::LogoState(StateStack &c_state_stack, sf::RenderWindow &c_window, Player &c_player, TextureGenerator &c_texture_factory)
: State(c_state_stack, c_window, c_player, c_texture_factory)
, logo_time(sf::seconds(2.f))
, exit_time(sf::seconds(1.f))
//, exit_effect(getWindow().getSize(), sf::Color(0, 0, 0))
, affector(0.f, 1.f)
{
	exit_effect.setPosition(sf::Vector2f(-400.f, -400.f));
	exit_effect.scale(sf::Vector2f(2.f, 2.f));
	texture.loadFromFile("textures/Logotipo.png");
	sprite.setTexture(texture);
	sprite.setPosition(sf::Vector2f(0.f, 90.f));
}


LogoState::~LogoState()
{
}

void LogoState::readInput()
{

}

void LogoState::draw()
{
	getWindow().draw(sprite);
}

void LogoState::update(sf::Time df)
{
	logo_time -= df;

	if (logo_time <= sf::Time::Zero)
	{
		std::cout << "Logo to OnExit" << std::endl;
		setTransitionState(Transitions::OnExit);
	}
}

void LogoState::enterUpdate(sf::Time df)
{
	restart();
	std::cout << "Logo to update" << std::endl;
	setTransitionState(Transitions::None);
}

void LogoState::enterDraw()
{

}

void LogoState::exitUpdate(sf::Time df)
{
	exit_effect.update(df);
	if (!exit_effect.isRunning())
	{
		requestPush(StateName::StartMenu);
		setTransitionState(Transitions::OnEnter);
		std::cout << "Logo to onEnter StartMenu" << std::endl;
	}
	
}


void LogoState::exitDraw()
{
	getWindow().draw(exit_effect);	
}

void LogoState::restart()
{
	logo_time = sf::seconds(2.f);
	exit_time = sf::seconds(1.f);
	
	exit_effect.restart();
	exit_effect.setLifeTime(exit_time);
	exit_effect.addClearRectangle(getWindow().getSize());
	exit_effect.addAffector(affector);
	
	setTransitionState(Transitions::None);
}

