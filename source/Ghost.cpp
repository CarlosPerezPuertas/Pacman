#include "..\header\Ghost.h"

GhostChase::GhostChase()
{

}

void GhostChase::enter(Ghost *entity)
{
	entity->changeAnimation(GhostAnimations::GoRight);
}

void GhostChase::execute(Ghost *entity, sf::Time df)
{
	entity->updateAnimation(df);
}

void GhostChase::exit(Ghost *entity)
{

}

GhostFrightened::GhostFrightened()
: frightened_time(sf::seconds(10.f))
, init_flash_time(sf::seconds(3.5f))
, flashing(false)
{

}

void GhostFrightened::enter(Ghost *entity)
{
	entity->changeAnimation(GhostAnimations::Escape);
}

void GhostFrightened::execute(Ghost *entity, sf::Time df)
{
	frightened_time -= df;
	entity->updateAnimation(df);

	if (frightened_time <= init_flash_time && flashing == false)
	{
		flashing = true;
		entity->changeAnimation(GhostAnimations::EscapeFlashing);
	}

	if (frightened_time <= sf::Time::Zero)
	{
		entity->changeState(GhostBehaviour::Chase);
	}
}

void GhostFrightened::exit(Ghost *entity)
{
	entity->changeAnimation(GhostAnimations::GoRight);
}


Ghost::Ghost(GhostType c_type, Textures::Type type, TextureGenerator &tg)
: MoveableObject()
, state_machine(*this)
, sprite(tg.get(type))
, animation(sprite, sf::seconds(0.1f), GhostAnimations::GoRight)
, behaviour(GhostBehaviour::Chase)
, type(c_type)
{

	animation.addFrame(GhostAnimations::Escape, sf::IntRect(0, 80, 20, 20));
	animation.addFrame(GhostAnimations::Escape, sf::IntRect(20, 80, 20, 20));

	animation.addFrame(GhostAnimations::EscapeFlashing, sf::IntRect(0, 80, 20, 20));
	animation.addFrame(GhostAnimations::EscapeFlashing, sf::IntRect(20, 80, 20, 20));
	animation.addFrame(GhostAnimations::EscapeFlashing, sf::IntRect(0, 100, 20, 20));
	animation.addFrame(GhostAnimations::EscapeFlashing, sf::IntRect(20, 100, 20, 20));

	animation.addFrame(GhostAnimations::GoRight, sf::IntRect(0, 60, 20, 20));
	animation.addFrame(GhostAnimations::GoRight, sf::IntRect(20, 60, 20, 20));

	animation.addFrame(GhostAnimations::GoLeft, sf::IntRect(0, 40, 20, 20));
	animation.addFrame(GhostAnimations::GoLeft, sf::IntRect(20, 40, 20, 20));

	animation.addFrame(GhostAnimations::GoUp, sf::IntRect(0, 0, 20, 20));
	animation.addFrame(GhostAnimations::GoUp, sf::IntRect(20, 0, 20, 20));
	
	animation.addFrame(GhostAnimations::GoDown, sf::IntRect(0, 20, 20, 20));
	animation.addFrame(GhostAnimations::GoDown, sf::IntRect(20, 20, 20, 20));

	animation.play();

	state_machine.registerState<GhostFrightened>(GhostBehaviour::Frightened);
	state_machine.registerState<GhostChase>(GhostBehaviour::Chase);
	state_machine.changeState(GhostBehaviour::Chase);
}


Ghost::~Ghost()
{
}

void Ghost::drawCurrent(sf::RenderTarget &target, sf::RenderStates states) const
{
	if (isActive())
	target.draw(sprite, states);
}

void Ghost::updateCurrent(CommandQueue &command_queue, const sf::Time df)
{
	if (isActive())
	state_machine.update(df);
}

unsigned int Ghost::getCategory() const
{
	return Category::Ghost;
}

sf::FloatRect Ghost::getGlobalRect()
{
	return getWorldTransform().transformRect(sprite.getGlobalBounds());

}

GhostBehaviour Ghost::getBehaviour()
{
	return behaviour;
}

void Ghost::escape()
{ 
	state_machine.changeState(GhostBehaviour::Frightened);
	behaviour = GhostBehaviour::Frightened; 
}




