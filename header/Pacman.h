#pragma once

#include "Entity.h"


enum Animations
{
	GoLeft,
	GoRight,
	GoUp,
	GoDown
};




class Pacman : public Entity
{
	public:
	    Pacman(Textures::Type type, TextureGenerator &tg);
		virtual ~Pacman(){}

		virtual void drawCurrent(sf::RenderTarget &target, sf::RenderStates states) const;
		virtual void updateCurrent(CommandQueue &command_queue, const sf::Time df);

		virtual unsigned int getCategory() const;
		
		void changeAnimation(const Animations state);
		bool isOtherAnimation(const Animations state){ return state != animation.getState(); }
		sf::FloatRect getGlobalRect();

		void die();
		bool isDead();
		void addScore(int c_score);
		
	private:
		sf::Sprite  sprite;  
		ga::Animation<Animations> animation;	
		int lifes;
		int score;

};
