#include "..\header\PickupObject.h"


PickupObject::PickupObject(Pickup c_type, Textures::Type t_type, TextureGenerator &t)
: StaticObject(t.get(t_type))
, type(c_type)
, points(0)
{
	sf::FloatRect local_rect = Entity::sprite.getLocalBounds();
	Entity::sprite.setOrigin(6.f, 6.f);

	animated[Pickup::SmallDot] = false;
	animated[Pickup::BigDot] = true;

	create_animation();
}


PickupObject::~PickupObject()
{
}

void PickupObject::drawCurrent(sf::RenderTarget &target, sf::RenderStates states) const
{
	target.draw(Entity::sprite, states);
}

void PickupObject::updateCurrent(CommandQueue &command_queue, const sf::Time df)
{
	if (hasLifetime == true)
	{
		lifetime_elapsed -= df;
		if (lifetime_elapsed <= sf::Time::Zero) removeFromScene();
	}
	
	if (animated[type])
	{
		animation->update(df);
	}
}

unsigned int PickupObject::getCategory() const
{
	if (type == Pickup::SmallDot) return Category::SmallDot;
	if (type == Pickup::BigDot)   return Category::BigDot;
	if (type == Pickup::Cherry || type == Pickup::Peach || type == Pickup::Key || type == Pickup::Apple ||
		type == Pickup::Galaxian || type == Pickup::Grapes || type == Pickup::Bell || type == Pickup::Strawerry)
		return Category::Bonus;

	return Category::Other;
}

void PickupObject::create_animation()
{
	std::unique_ptr<ga::Animation<DotAnimation>> anim(new ga::Animation<DotAnimation>(Entity::sprite, sf::seconds(0.2f), DotAnimation::unique));
	
	if (type == Pickup::BigDot)
	{
		anim->addFrame(DotAnimation::unique, sf::IntRect(0, 0, 12, 12));
		anim->addFrame(DotAnimation::unique, sf::IntRect(12, 0, 12, 12));
		anim->addFrame(DotAnimation::unique, sf::IntRect(24, 0, 12, 12));
		anim->play();
	}

	animation = std::move(anim);
}


