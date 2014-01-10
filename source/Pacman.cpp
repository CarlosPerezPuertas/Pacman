#include "..\header\Pacman.h"





Pacman::Pacman(Textures::Type type, TextureGenerator &tg) :
  animation(sprite, sf::seconds(0.15f), Animations::GoRight)
, sprite(tg.get(type))
, lifes(3)
, score(0)
{
	//Set the center in order to apply transformations
	sf::FloatRect local_rect = sprite.getLocalBounds();
	sprite.setOrigin(10.f, 10.f);
	
	//Add animations frame x frame
	animation.addFrame(GoLeft, sf::IntRect(0, 0, 20, 20));
	animation.addFrame(GoLeft, sf::IntRect(20, 0, 20, 20));
	animation.addFrame(GoLeft, sf::IntRect(40, 0, 20, 20));
	animation.addFrame(GoLeft, sf::IntRect(60, 0, 20, 20));

	animation.addFrame(GoRight, sf::IntRect(0, 20, 20, 20));
	animation.addFrame(GoRight, sf::IntRect(20, 20, 20, 20));
	animation.addFrame(GoRight, sf::IntRect(40, 20, 20, 20));
	animation.addFrame(GoRight, sf::IntRect(60, 20, 20, 20));

	animation.addFrame(GoUp, sf::IntRect(0, 40, 20, 20));
	animation.addFrame(GoUp, sf::IntRect(20, 40, 20, 20));
	animation.addFrame(GoUp, sf::IntRect(40, 40, 20, 20));
	animation.addFrame(GoUp, sf::IntRect(60, 40, 20, 20));

	animation.addFrame(GoDown, sf::IntRect(0, 60, 20, 20));
	animation.addFrame(GoDown, sf::IntRect(20, 60, 20, 20));
	animation.addFrame(GoDown, sf::IntRect(40, 60, 20, 20));
	animation.addFrame(GoDown, sf::IntRect(60, 60, 20, 20));

	animation.play();
}


void Pacman::drawCurrent(sf::RenderTarget &target, sf::RenderStates states) const
{
	target.draw(sprite, states);
}

void Pacman::updateCurrent(CommandQueue &command_queue, const sf::Time df)
{
	animation.update(df);
}

void Pacman::changeAnimation(const Animations state)
{
	animation.changeState(state);
}

unsigned int Pacman::getCategory() const
{ 
	return Category::Pacman;
}


//The rectangle with corrected axis to make easy the collision detection
sf::FloatRect Pacman::getGlobalRect()
{
	return getWorlTransform().transformRect(sprite.getGlobalBounds());
}

void Pacman::die()
{

}

bool Pacman::isDead()
{
	return lifes <= 0;
}

void Pacman::addScore(int c_score)
{
	score += c_score;
}


