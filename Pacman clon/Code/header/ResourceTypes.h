#pragma once

//Forward declaration of texture and Resources
namespace sf
{
	class Texture;
	class Music;
	class SoundBuffer;
}


namespace Textures
{
	enum Type
	{
		StartLogo,
		Pacman,
		Ghost1,
		Ghost2,
		Ghost3,
		Ghost4,
		Labyrinth,
		Logo,
		ScreenTransition1,
		SmallDot,
		BigDot,
		Cherry,
		Strawerry,
		Peach,
		Apple,
		Grapes,
		Galaxian,
		Bell,
		Key,
		Life
	};
}

namespace SoundEffect
{
	enum Type
	{
		EatPickup,
		PacmanDies,
		OneUp,
		EatGhost,
		Waka1,
		Waka2,
		Siren,
		BigDot,
		Opening
	};
}

template <typename Resource, typename EnumType>
class Resources;


typedef Resources<sf::Texture, Textures::Type> TextureGenerator;
typedef Resources<sf::SoundBuffer, SoundEffect::Type> SoundBufferFactory;

