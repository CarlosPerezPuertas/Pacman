#pragma once

#include "MoveableObject.h"



enum class PacmanBehaviour
{
	Chase,
	Escape
};



class Pacman : public MoveableObject
{
	public:
	    Pacman(Textures::Type type, TextureGenerator &tg);
		virtual ~Pacman(){}

		virtual void drawCurrent(sf::RenderTarget &target, sf::RenderStates states) const;
		virtual void updateCurrent(CommandQueue &command_queue, const sf::Time df);
		virtual unsigned int getCategory() const;

		inline void playAnimation(){ animation.play(); }
		inline void stopAnimation(){ animation.setFrameIterator(1); }
		bool isOtherAnimation(const Animations state){ return state != animation.getState(); }
		void changeAnimation(const Animations state);
		void restart();
		
		sf::FloatRect getGlobalRect();
		sf::FloatRect getInteriorRect();
		sf::Vector2f getCenter();
		PacmanBehaviour getBehaviour();

		void die();
		bool isDead();

		inline void addScore(int c_score) { score += c_score; }
		inline int  getScore() { return score; }
		inline bool isHigherScore(){ return score > high_score; }

	private:
		sf::Sprite  sprite;  
		PacmanBehaviour behaviour;
		int lifes;
		int score;
		int high_score;
		ga::Animation<Animations> animation;
		

};
