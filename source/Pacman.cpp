#include "..\header\Pacman.h"





Pacman::Pacman(Textures::Type type, TextureGenerator &tg) 
: MoveableObject()
, sprite(tg.get(type))
, animation(sprite, sf::seconds(0.15f), Animations::GoLeft)
, lifes(3)
, score(0)
, high_score(260)
, behaviour(PacmanBehaviour::Escape)
{
	//Set the center in order to apply transformations
	const float num_frames = 4.f;
	sf::FloatRect local_rect = sprite.getLocalBounds();
	sf::FloatRect frame_rect = sf::FloatRect(local_rect.left, 
		                                     local_rect.top, 
											 local_rect.width / num_frames, 
											 local_rect.height / num_frames);

	sprite.setOrigin(frame_rect.width / 2.f, frame_rect.height / 2.f);
	
	//ENTITY COLLISIONS Set Real Rect in the center of the entity
	float factor = 0.25f;
	setRealRect(sf::FloatRect(frame_rect.left*factor + frame_rect.width / 4.f*factor, 
		                      frame_rect.top*factor + frame_rect.height / 4.f*factor,
		                      frame_rect.width*factor, 
							  frame_rect.height*factor));

	//Allowed directions
	possible_directions.insert(Direction::Right);
	possible_directions.insert(Direction::Left);
	
	//Add animations frame x frame
	animation.addFrame(Animations::GoLeft, sf::IntRect(0, 0, 20, 20));
	animation.addFrame(Animations::GoLeft, sf::IntRect(20, 0, 20, 20));
	animation.addFrame(Animations::GoLeft, sf::IntRect(40, 0, 20, 20));
	animation.addFrame(Animations::GoLeft, sf::IntRect(60, 0, 20, 20));

	animation.addFrame(Animations::GoRight, sf::IntRect(0, 20, 20, 20));
	animation.addFrame(Animations::GoRight, sf::IntRect(20, 20, 20, 20));
	animation.addFrame(Animations::GoRight, sf::IntRect(40, 20, 20, 20));
	animation.addFrame(Animations::GoRight, sf::IntRect(60, 20, 20, 20));

	animation.addFrame(Animations::GoUp, sf::IntRect(0, 40, 20, 20));
	animation.addFrame(Animations::GoUp, sf::IntRect(20, 40, 20, 20));
	animation.addFrame(Animations::GoUp, sf::IntRect(40, 40, 20, 20));
	animation.addFrame(Animations::GoUp, sf::IntRect(60, 40, 20, 20));

	animation.addFrame(Animations::GoDown, sf::IntRect(0, 60, 20, 20));
	animation.addFrame(Animations::GoDown, sf::IntRect(20, 60, 20, 20));
	animation.addFrame(Animations::GoDown, sf::IntRect(40, 60, 20, 20));
	animation.addFrame(Animations::GoDown, sf::IntRect(60, 60, 20, 20));

	animation.play();
}


void Pacman::drawCurrent(sf::RenderTarget &target, sf::RenderStates states) const
{
	if(isActive())
	target.draw(sprite, states);
}

void Pacman::updateCurrent(CommandQueue &command_queue, const sf::Time df)
{
	if (isActive())
	animation.update(df);
}

void Pacman::changeAnimation(const Animations state)
{
	animation.changeState(state);
	animation.play();
}

void Pacman::restart()
{
	possible_directions.clear();
	possible_directions.insert(Direction::Right);
	possible_directions.insert(Direction::Left);
	setCurrentDirection(Direction::None);
	animation.changeState(Animations::GoLeft);
	restartPosition();
}


unsigned int Pacman::getCategory() const
{ 
	return Category::Pacman;
}


//The rectangle with corrected axis to make easy the collision detection
sf::FloatRect Pacman::getGlobalRect()
{
	return getWorldTransform().transformRect(sprite.getGlobalBounds());
}

sf::FloatRect Pacman::getInteriorRect()
{
	return getWorldTransform().transformRect(getRealRect());
}

sf::Vector2f Pacman::getCenter()
{
	sf::FloatRect rect = getGlobalRect();
	return sf::Vector2f(rect.left + rect.width / 2.f, rect.top + rect.height / 2.f);
}

void Pacman::die()
{

}

bool Pacman::isDead()
{
	return lifes <= 0;
}



PacmanBehaviour Pacman::getBehaviour()
{
	return behaviour;
}




