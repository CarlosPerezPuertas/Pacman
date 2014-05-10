#pragma once

#include "MoveableObject.h"
#include "EntityState.h"
#include "StateMachine.h"
#include <random>


enum class GhostAnimations
{
	GoLeft,
	GoRight,
	GoUp,
	GoDown,
	EyeLeft,
	EyeRight,
	EyeUp,
	EyeDown,
	Escape,
	EscapeFlashing
};


enum class GhostType
{
	Blinky,
	Pinky,
	Inky,
	Clyde
};

class Ghost;

class GhostScatter : public EntityState<Ghost>
{
	public:
		GhostScatter();
		~GhostScatter(){ }

		virtual void enter(Ghost *entity);
		virtual void execute(Ghost *entity, sf::Time dt);
		virtual void exit(Ghost *entity);

		void choosePath(sf::Time dt, Ghost *ghost);
		void moveByDirection(sf::Time dt, Ghost *ghost);
		void selectDirection(Ghost *ghost);

	private:
		sf::Time elapsed;
};

class GhostChase: public EntityState<Ghost>
{
	public:
		GhostChase();
		~GhostChase(){ }

		virtual void enter(Ghost *entity);
		virtual void execute(Ghost *entity, sf::Time dt);
		virtual void exit(Ghost *entity);

		void choosePath(sf::Time dt, Ghost *ghost);
		void moveByDirection(sf::Time dt, Ghost *ghost);
		void selectDirection(Ghost *ghost);

	private:
		Direction last_direction;

};


class GhostFrightened : public EntityState<Ghost>
{
	public:
		GhostFrightened();

		virtual void enter(Ghost *entity);
		virtual void execute(Ghost *entity, sf::Time dt);
		virtual void exit(Ghost *entity);

		void randomPath(sf::Time dt, Ghost *ghost);
		void moveByDirection(sf::Time dt, Ghost *ghost);
	
	private:
		sf::Time frightened_time;
		sf::Time init_flash_time;
		bool flashing;
};

class GhostCaveReturn: public EntityState<Ghost>
{
	public:
		virtual void enter(Ghost *entity);
		virtual void execute(Ghost *entity, sf::Time dt);
		virtual void exit(Ghost *entity);

		void choosePath(sf::Time dt, Ghost *ghost);
		void moveByDirection(sf::Time dt, Ghost *ghost);
		void selectDirection(Ghost *ghost);
};



class Ghost : public MoveableObject
{
	public:
		Ghost(GhostType c_type, Textures::Type type, TextureGenerator &tg);
		virtual ~Ghost();

		virtual void drawCurrent(sf::RenderTarget &target, sf::RenderStates states) const;
		virtual void updateCurrent(CommandQueue &command_queue, const sf::Time df);

		void updateAnimation(sf::Time dt){ animation.update(dt); }
		void changeAnimation(GhostAnimations anim){ animation.changeState(anim); }
		void escape(){ changeState(GhostBehaviour::Frightened); }
		void discardOppositeDirection();
		void restart();

		bool selectRightDirection();
		bool selectLeftDirection();
		bool selectUpDirection();
		bool selectDownDirection();
		

		inline void setScatterTarget(int num_tile){ num_scatter_target = num_tile; state_machine.changeState(GhostBehaviour::Scatter); }
		inline void setCaveReturnTarget(int num_tile){ num_cave_return_target = num_tile; }
		inline void setChaseTarget(int num_tile){ num_chase_target = num_tile; }
		inline void setCurrentTarget(int num_tile){ current_target = num_tile; }

		inline int getScatterTarget(){ return num_scatter_target; }
		inline int getCaveReturnTarget(){ return num_cave_return_target; }
		inline int getChaseTarget(){ return num_chase_target; }
		inline int getCurrentTarget(){ return current_target; }

		virtual unsigned int getCategory() const;
		GhostBehaviour getCurrentBehaviour();
		inline GhostType getType(){ return type; }
		inline void changeState(GhostBehaviour c_behaviour) { state_machine.changeState(c_behaviour); }
		inline GhostBehaviour getCurrentState() { return state_machine.getCurrentState(); }
		inline GhostAnimations getAnimationState() { return animation.getState(); }
		inline int getGhostsEaten(){ return ghosts_eaten; }
		inline void isEaten(){ ghosts_eaten++; }
		inline void ghostEatenToZero(){ ghosts_eaten = 0; }
		
		inline void setExitCaveTime(float seconds){ exit_cave_time = sf::seconds(seconds); exit_cave_elapsed = exit_cave_time; }
		inline void setFrightenedTime(float seconds){ frightened_time = sf::seconds(seconds);}
		inline sf::Time getFrightenedTime(){ return frightened_time; }

		inline void setElroy1(bool x){ is_elroy1 = x; }
		inline void setElroy2(bool x){ is_elroy2 = x; }
		

	private:
		ga::Animation<GhostAnimations> animation;
		GhostType type;
		StateMachine<Ghost, GhostBehaviour> state_machine;

		int num_scatter_target;
		int num_chase_target;
		int num_cave_return_target;
		int current_target;

		static int ghosts_eaten;


	public:
		sf::Time exit_cave_elapsed;
		sf::Time exit_cave_time;
		sf::Time frightened_time;
};




