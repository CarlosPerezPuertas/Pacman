#pragma once

#include <SFML/Graphics.hpp>
#include <iostream>
#include <vector>
#include <map>
#include <cassert>
#include <functional>
#include "ResourceTypes.h"
#include "Resources.h"
#include "ScreenAffectors.h"


//ScreeEffect class is used to do simple effects in the screen that are applied in the whole screen.
//For example to make transition between stages we fade the screen making it dark in order to make softs transitions

/*######INSTRUCTIONS######
	
	1 Create Screen effect clas and assign a texture  => ScreenEffect(sf::Texture &texture);
			 or a shape specifying the size and color => ScreenEffect(sf::Vector2i rect, sf::Color color);	
	
	2 Set a lifetime.                                 => screen_effect.setLifeTime(sf::seconds(2.f));

	3 Create an affector.                             => FadeAffector fade(double init_alpha , int end_alpha)

	4 Add the affector to the screen effect system.   => screen_effect.addAffector(fade);
	  Because it's a template it's compatible with 
	  differents affectors.

	Options:

	a If you want to have a soft clear screen         => screen_effect.addClearRectangle(window_size);

																			+

														     screen_effect.addAffector(fade);

	At this moment there is only one affector.
*/


class ScreenEffect : public sf::Drawable, public sf::Transformable
{
	public:
		typedef std::vector<std::function<void(ScreenEffect&, sf::Time)>> Affectors;

	public:
		ScreenEffect(){}
		ScreenEffect(sf::Texture &texture);
		ScreenEffect(sf::Vector2u rect, sf::Color color);

		virtual void draw(sf::RenderTarget &target, sf::RenderStates states) const;
		void update(sf::Time df);

		template<typename T>
		void addAffector(T aff);
		void addClearRectangle(sf::Vector2u window_size);

		void setTexture(sf::Texture &texture);
		void setTexture(sf::Vector2i rect, sf::Color color);
		void setLifeTime(sf::Time life_time){ this->life_time = life_time; }
		sf::Time getLifeTime() { return elapsed; }
		void setAlpha(sf::Uint8 alpha)
		{ 
			sprite.setColor(sf::Color(color.r, color.g, color.b, alpha)); 
			clear_rectangle.setFillColor(sf::Color(color.r, color.g, color.b, alpha));
		}
		
		
		bool isRunning();
	
	public:   
		sf::Color color;     //Attributes used in affectors
		sf::Time elapsed;	
		sf::Time life_time;
		
	private:
		sf::Texture texture; //Needed to create colored rectangles from sf::Image
		sf::Sprite sprite;

		bool is_clearing;				    // The difference between window.clear() it's that this have alpha chanel
		sf::RectangleShape clear_rectangle; // Used to make soft transitions
		
		Affectors affectors;	
};

template<typename T>
void ScreenEffect::addAffector(T aff)
{
	aff.init(*this);
	affectors.push_back(aff.get());
}




