#include "..\header\SoundNode.h"

bool SoundNode::first_waka = true;

SoundNode::SoundNode(SoundEffect::Type c_type, SoundBufferFactory &buffer_factory)
: sound(buffer_factory.get(c_type))
, type(c_type)
, time_after(sf::Time::Zero)
{
}

SoundNode::SoundNode(SoundEffect::Type c_type1, SoundEffect::Type c_type2, SoundBufferFactory &buffer_factory)
: sound(buffer_factory.get(c_type1))
, second_sound(buffer_factory.get(c_type2))
, type(c_type1)
{

}


SoundNode::~SoundNode()
{
}

unsigned int SoundNode::getCategory() const
{
	if (type == SoundEffect::EatPickup) return Category::SoundEatPickup;
	else if (type == SoundEffect::PacmanDies) return Category::SoundPacmanDies;
	else if (type == SoundEffect::OneUp) return Category::SoundOneUp;
	else if (type == SoundEffect::EatGhost) return Category::SoundEatGhost;
	else if (type == SoundEffect::Siren) return Category::SoundSiren;
	else if (type == SoundEffect::Waka1) return Category::SoundWaka;
	else if (type == SoundEffect::Waka2) return Category::SoundWaka;
	else if (type == SoundEffect::BigDot) return Category::SoundBigDot;
	else if (type == SoundEffect::Opening) return Category::SoundOpening;
	
	return Category::Other;
}

void SoundNode::play()
{
	
	if (getCategory() == Category::SoundWaka)
	{
		if (first_waka == true)
		{
			if (sound.getStatus() != sf::Sound::Status::Playing && second_sound.getStatus() != sf::Sound::Status::Playing)
			{
				sound.play();
				first_waka = false;
			}
		}
		else if (first_waka == false)
		{
			if (sound.getStatus() != sf::Sound::Status::Playing && second_sound.getStatus() != sf::Sound::Status::Playing)
			{
				second_sound.play();
				first_waka = true;
			}
		}
	}
	else sound.play();
	
}

void SoundNode::playAfter(const float seconds)
{
	time_disabled = sf::seconds(seconds);
	isTemporalDisabled = true;
}

void SoundNode::playDuring(const float seconds)
{
	hasLifetime = true;
	lifetime_elapsed = sf::seconds(seconds);
	sound.play();
}


void SoundNode::updateCurrent(CommandQueue &command_queue, const sf::Time dt)
{
	if (isTemporalDisabled == true)
	{
		time_disabled -= dt;

		if (time_disabled <= sf::Time::Zero)
		{
			sound.play();
			isTemporalDisabled = false;
		}	
	}	

	if (hasLifetime == true)
	{
		lifetime_elapsed -= dt;

		if (lifetime_elapsed <= sf::Time::Zero)
		{
			sound.stop();
			hasLifetime = false;
		}
	}
}

void SoundNode::stop()
{
	sound.stop();
	//second_sound.stop();
}

void SoundNode::setLoop(bool x)
{
	sound.setLoop(x);
}

void SoundNode::setPitch(float pitch)
{
	sound.setPitch(pitch);
	second_sound.setPitch(pitch);
}
