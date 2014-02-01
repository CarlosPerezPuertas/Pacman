#include "..\header\Text.h"


Text::Text(std::string c_text, std::string font_type, unsigned int font_size, TextType c_type, sf::Color color)
: type(c_type)
, lifetime(sf::seconds(0.f))
, hasTime(false)
{ 
	font.loadFromFile(font_type);
	text.setFont(font);
	text.setString(c_text);
	text.setCharacterSize(font_size);
	text.setColor(color);	
}


void Text::drawCurrent(sf::RenderTarget &target, sf::RenderStates states) const
{
	target.draw(text, states);
}

void Text::updateCurrent(CommandQueue &command_queue, const sf::Time df)
{
	
	if (hasTime == true)
	{
		lifetime -= df;

		if (lifetime <= sf::Time::Zero)
		{
			hasTime = false;
			removeFromScene();
		}
	}
}