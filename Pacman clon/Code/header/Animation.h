#pragma once

#include <SFML/Graphics.hpp>
#include <iostream>
#include <vector>
#include <map>
#include <set>
#include <cassert>


namespace ga
{
//Template to associate different types of animations in each entity
template <typename States>
class Animation
{
	public:
		//Associate a sprite_sheet, the duration of each frame and the animation template-type
		explicit Animation(sf::Sprite &c_sprite, const sf::Time dt, const States init_state);
		
		void update(const sf::Time dt);
		void addFrame(const States id, const sf::IntRect frame_rect);

		void changeState(const States id);
		States getState(){ return state; }

		inline void play(){ playing = true; }
		inline void stop(){ playing = false; }
		inline void reset(){ if (playing == true) { playing = false; frame_itr = frame_map[state].begin(); sprite.setTextureRect(*frame_itr); } }
		inline void setFrameIterator(const int advance){ if (playing == true){ stop(); frame_itr = frame_map[state].begin(); std::advance(frame_itr, 1); sprite.setTextureRect(*frame_itr); } }
		inline bool isPlaying(){ return playing; }
		inline bool isUniqueLoop() { return loop_map.find(state) != loop_map.end() ; }
		inline void setUniqueLoop(States id){ loop_map.insert(id); }
		inline bool isEndLoop(){ return end_loop; }

	private:
		sf::Sprite &sprite;
		std::vector<sf::IntRect>::iterator frame_itr;
		std::map<States, std::vector<sf::IntRect>> frame_map;
		std::set<States> loop_map;

		sf::Time frame_rate;
		sf::Time elapsed;
		States   state;
		bool     playing;
		bool     iterator_initialized;
		bool     end_loop;
};



template <typename States>
Animation<States>::Animation(sf::Sprite &c_sprite, const sf::Time dt, const States init_state) :
sprite(c_sprite)     //link with the sprite of the entity
, frame_rate(dt)
, elapsed(frame_rate)
, state(init_state)
, iterator_initialized(false)
, playing(false)
, end_loop(false)
{
	
}


//If animation is playing then update depending on the frame rate of the animation
template <typename States>
void Animation<States>::update(const sf::Time dt)
{
	if (isPlaying())
	{
		elapsed -= dt;
		assert(iterator_initialized == true); //assert if the iterator it's not initialized

		if (elapsed <= sf::Time::Zero)
		{
			++frame_itr;

			//Restart the animation
			if (frame_itr == frame_map[state].end())
			{
				if (isUniqueLoop()) { stop(); end_loop = true; }
				frame_itr = frame_map[state].begin();	
			}

			elapsed = frame_rate;
			sprite.setTextureRect(*frame_itr);
		}
	}
}

//Find the animation type and check if exists, if it isn't then create
//Create and add the frame to the vector depending on the animation type
template <typename States>
void Animation<States>::addFrame(const States id, const sf::IntRect frame_rect)
{
	auto itr = frame_map.find(id);

	if (itr == frame_map.end())
	{
		sprite.setTextureRect(frame_rect);
		std::vector<sf::IntRect> animation_frames(0);
		animation_frames.push_back(frame_rect);
		frame_map.insert(std::make_pair(id, animation_frames));
		itr = frame_map.begin();
	}
	else itr->second.push_back(frame_rect);

	//Put iterator on the initial state
	if (id == state)
	{
		frame_itr = itr->second.begin();
		iterator_initialized = true;
	}		
}

//Change animation type and init the iterator starting in the first frame
template <typename States>
void Animation<States>::changeState(const States id)
{
	end_loop = false;

	state = id;
	frame_itr = frame_map[id].begin();
	assert(frame_itr != frame_map[id].end()); //Assert if the id doesn't exist
	sprite.setTextureRect(*frame_itr);
}

}