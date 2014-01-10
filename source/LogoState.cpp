#include "..\header\LogoState.h"



LogoState::LogoState(StateStack &c_state_stack, sf::RenderWindow &c_window, Player &c_player, TextureGenerator &c_texture_factory)
: State(c_state_stack, c_window, c_player, c_texture_factory)
, logo_time(sf::seconds(2.f))
, exit_time(sf::seconds(1.f))
//, exit_effect(getWindow().getSize(), sf::Color(0, 0, 0))
, affector(0.f, 1.f)
{
	texture2.loadFromFile("textures/effect02.png");
	exit_effect.addClearRectangle(getWindow().getSize());
	exit_effect.setTexture(texture2);
	exit_effect.setPosition(sf::Vector2f(-400.f, -400.f));
	exit_effect.scale(sf::Vector2f(2.f, 2.f));
	exit_effect.addAffector(affector);
	exit_effect.setLifeTime(exit_time);
	

	setTransitionState(Transitions::None);
	texture.loadFromFile("textures/Logotipo.png");
	sprite.setTexture(texture);
	sprite.setPosition(sf::Vector2f(0.f, +100.f));
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
		setTransitionState(Transitions::OnExit);
	}
}

void LogoState::enterUpdate(sf::Time df)
{
	setTransitionState(Transitions::None);
}

void LogoState::enterDraw()
{

}

void LogoState::exitUpdate(sf::Time df)
{
	exit_effect.update(df);
	if (!exit_effect.isRunning()) push(2);
}


void LogoState::exitDraw()
{
	getWindow().draw(exit_effect);	
}

