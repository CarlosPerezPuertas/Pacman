#pragma once

//Forward declaration of texture and Resources
namespace sf
{
	class Texture;
}

template <typename Resource, typename EnumType>
class Resources;


namespace Textures
{
	enum Type
	{
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
		Key
	};
}

typedef Resources<sf::Texture, Textures::Type> TextureGenerator;

