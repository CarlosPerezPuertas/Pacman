#pragma once

#include "MoveableObject.h"
#include "EntityState.h"
#include "StateMachine.h"




enum class GhostAnimations
{
	GoLeft,
	GoRight,
	GoUp,
	GoDown,
	Escape,
	EscapeFlashing
};


enum class GhostBehaviour
{
	Chase,
	Scater,
	Frightened
};

enum class GhostType
{
	Blinky,
	Pinky,
	Inky,
	Clyde
};

class Ghost;

class GhostChase: public EntityState<Ghost>
{
	public:
		GhostChase();

		virtual void enter(Ghost *entity);
		virtual void execute(Ghost *entity, sf::Time df);
		virtual void exit(Ghost *entity);
};


class GhostFrightened : public EntityState<Ghost>
{
	public:
		GhostFrightened();

		virtual void enter(Ghost *entity);
		virtual void execute(Ghost *entity, sf::Time df);
		virtual void exit(Ghost *entity);

	private:
		sf::Time frightened_time;
		sf::Time init_flash_time;
		bool flashing;
};



class Ghost : public MoveableObject
{
	public:
		Ghost(GhostType c_type, Textures::Type type, TextureGenerator &tg);
		virtual ~Ghost();

		virtual void drawCurrent(sf::RenderTarget &target, sf::RenderStates states) const;
		virtual void updateCurrent(CommandQueue &command_queue, const sf::Time df);

		void updateAnimation(sf::Time df){ animation.update(df); }
		void changeAnimation(GhostAnimations anim){ animation.changeState(anim); }
		void escape();

		virtual unsigned int getCategory() const;
		sf::FloatRect getGlobalRect();
		GhostBehaviour getBehaviour();

		inline void changeState(GhostBehaviour c_behaviour) { state_machine.changeState(c_behaviour); }

		

	private:
		sf::Sprite  sprite;
		ga::Animation<GhostAnimations> animation;
		GhostBehaviour behaviour;
		GhostType type;

		StateMachine<Ghost, GhostBehaviour> state_machine;	
};




