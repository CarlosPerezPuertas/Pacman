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
		Ghost
	};
}

typedef Resources<sf::Texture, Textures::Type> TextureGenerator;

