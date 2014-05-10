#pragma once

#include "MoveableObject.h"
#include "Player.h"
#include <fstream>


/*enum class GhostBehaviour
{
	None,
	Chase,
	Escape
};*/

enum class Animations
{
	GoLeft,
	GoRight,
	GoUp,
	GoDown,
	Die
};




class Pacman : public MoveableObject
{
	public:
		Pacman(Textures::Type type, TextureGenerator &tg, Player &c_player);
		virtual ~Pacman(){}

		virtual void drawCurrent(sf::RenderTarget &target, sf::RenderStates states) const;
		virtual void updateCurrent(CommandQueue &command_queue, const sf::Time df);
		virtual unsigned int getCategory() const;

		inline void playAnimation(){ animation.play(); }
		inline void stopAnimation(){ animation.setFrameIterator(1); }
		bool isOtherAnimation(const Animations state){ return state != animation.getState(); }
		void changeAnimation(const Animations state);
		void restart();


		void die();
		inline bool isDeadAnimationEnd(){ return animation.getState() == Animations::Die && animation.isEndLoop(); }
		bool hasLifes();
		inline void earnLife(){ lifes++; }
		inline int getLifes() { return lifes; }
		inline bool isDiying(){ return animation.getState() == Animations::Die; }

		inline void addScore(int c_score) { score += c_score; }
		inline int  getScore() { return score; }
		inline int  getHighScore() { return high_score; }
		inline bool isHigherScore(){ return score > high_score; }

		inline GhostBehaviour getCurrentBehaviour() { return GhostBehaviour::None; }

	private:
		ga::Animation<Animations> animation;
		Player &player;

		int lifes;
		int score;
		int high_score;	

};
