#include "..\header\ScreenEffect.h"




//Create from sprite
ScreenEffect::ScreenEffect(sf::Texture &texture)
: sprite(texture)
, elapsed(sf::Time::Zero)
, life_time(sf::Time::Zero)
, is_clearing(false)
{

}

//Create a rectangle with the given size and color
ScreenEffect::ScreenEffect(sf::Vector2u rect, sf::Color color = sf::Color::Black)
{
	sf::Image image;
	image.create(rect.x, rect.y, color);
	texture.loadFromImage(image);
	sprite.setTexture(texture);
	this->color = color;
}


void ScreenEffect::draw(sf::RenderTarget &target, sf::RenderStates states) const
{
	states.transform *= getTransform();
	target.draw(sprite, states);
	if (is_clearing == true) target.draw(clear_rectangle, states);
}

void ScreenEffect::update(sf::Time df)
{
	for (auto &itr : affectors)
	{
		itr(*this, df);
		sprite.setColor(color);

		if (is_clearing == true) clear_rectangle.setFillColor(color);
	}	
}

void ScreenEffect::setTexture(sf::Texture &texture)
{
	sprite.setTexture(texture);
}

void ScreenEffect::setTexture(sf::Vector2i rect, sf::Color color)
{
	sf::Image image;
	image.create(rect.x, rect.y, color);
	texture.loadFromImage(image);
	sprite.setTexture(texture);
	this->color = color;
}

bool ScreenEffect::isRunning()
{
	return life_time.asSeconds() > elapsed.asSeconds();
}

void ScreenEffect::addClearRectangle(sf::Vector2u window_size)
{
	clear_rectangle.setFillColor(sf::Color::Black);
	clear_rectangle.setSize(static_cast<sf::Vector2f>(window_size));
	is_clearing = true;
}





