#pragma once

#include "SceneNode.h"

class SoundNode : public SceneNode
{
	public:
		explicit SoundNode(SoundEffect::Type c_type, SoundBufferFactory &buffer_factory);
		explicit SoundNode(SoundEffect::Type c_type1, SoundEffect::Type c_type2, SoundBufferFactory &buffer_factory);
		virtual ~SoundNode();

		virtual unsigned int getCategory() const;
		void play();
		void playAfter(const float seconds);
		void playDuring(const float seconds);
		void updateCurrent(CommandQueue &command_queue, const sf::Time dt);
		void stop();
		void setLoop(bool x);
		void setPitch(float pitch);



	private:
		sf::Sound sound;	
		sf::Sound second_sound;
		sf::SoundBuffer buffer;
		SoundEffect::Type type;
		static bool first_waka;

		sf::Time time_after;

};

