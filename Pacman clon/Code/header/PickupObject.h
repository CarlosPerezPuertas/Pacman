#pragma once

#include "StaticObject.h"

enum class Pickup
{
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

enum class DotAnimation
{
	unique
};

class PickupObject : public StaticObject
{
	public:
		PickupObject(Pickup c_type, Textures::Type t_type, TextureGenerator &t);
		virtual ~PickupObject();

		virtual void drawCurrent(sf::RenderTarget &target, sf::RenderStates states) const;
		virtual void updateCurrent(CommandQueue &command_queue, const sf::Time df);
		virtual unsigned int getCategory() const;

		void create_animation();

		inline int getScore(){ return points; }
		inline void setScore(int score){ points = score; }

	private:
		int points;
		Pickup type;

		std::map<Pickup, bool> animated;
		std::unique_ptr<ga::Animation<DotAnimation>> animation;	
};

