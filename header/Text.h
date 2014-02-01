#pragma once

#include "SceneNode.h"

enum TextType
{
	Static,
	Score,
	HighScore
};

class Text : public SceneNode
{
	public:
		explicit Text(std::string c_text, std::string font_type, unsigned int font_size, TextType c_type = TextType::Static, sf::Color color = sf::Color::White);

		virtual void drawCurrent(sf::RenderTarget &target, sf::RenderStates states) const;
		virtual void updateCurrent(CommandQueue &command_queue, const sf::Time df);

		inline virtual unsigned int getCategory() const { return Category::Text; }
		inline void setText(std::string data){ text.setString(data); }
		inline const TextType getType() const { return type; }
		inline void setLifeTime(float seconds) { hasTime = true;  lifetime = sf::seconds(seconds); }
		

	private:
		sf::Text text;
		sf::Font font;
		TextType type;
		sf::Time lifetime;
		bool     hasTime;

		
};

