#include "..\header\PickupObject.h"


PickupObject::PickupObject(Pickup c_type, Textures::Type t_type, TextureGenerator &t)
: StaticObject()
, sprite(t.get(t_type))
, type(c_type)
{
	sf::FloatRect local_rect = sprite.getLocalBounds();
	sprite.setOrigin(6.f, 6.f);

	animated[Pickup::SmallDot] = false;
	animated[Pickup::BigDot] = true;

	create_animation();
}


PickupObject::~PickupObject()
{
}

void PickupObject::drawCurrent(sf::RenderTarget &target, sf::RenderStates states) const
{
	target.draw(sprite, states);
}

void PickupObject::updateCurrent(CommandQueue &command_queue, const sf::Time df)
{
	if (animated[type])
	{
		animation->update(df);
	}
}

unsigned int PickupObject::getCategory() const
{
	if (type == Pickup::SmallDot) return Category::SmallDot;
	if (type == Pickup::BigDot)   return Category::BigDot;

	return Category::Other;
}

void PickupObject::create_animation()
{
	std::unique_ptr<ga::Animation<DotAnimation>> anim(new ga::Animation<DotAnimation>(sprite, sf::seconds(0.2f), DotAnimation::unique));
	
	if (type == Pickup::BigDot)
	{
		anim->addFrame(DotAnimation::unique, sf::IntRect(0, 0, 12, 12));
		anim->addFrame(DotAnimation::unique, sf::IntRect(12, 0, 12, 12));
		anim->addFrame(DotAnimation::unique, sf::IntRect(24, 0, 12, 12));
		anim->play();
	}

	animation = std::move(anim);
}

sf::FloatRect PickupObject::getGlobalRect()
{
	return getWorldTransform().transformRect(sprite.getGlobalBounds());
}
