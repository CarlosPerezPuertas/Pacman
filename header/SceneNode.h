#pragma once

#include <SFML/Graphics.hpp>
#include <iostream>
#include <vector>
#include <chrono>
#include <memory>

#include "Command.h"
#include "CommandQueue.h"

namespace Category
{
	enum Type
	{
		Pacman,
		Ghost,
		Fruit,
		PacDot,
		Other
		
	};
}



class SceneNode : public sf::Drawable, public sf::Transformable, private sf::NonCopyable
{
	public:
		typedef std::unique_ptr<SceneNode> PtrNode;

	public:
		SceneNode();
		virtual ~SceneNode();

		virtual void draw(sf::RenderTarget &target, sf::RenderStates states) const;
		virtual void drawCurrent(sf::RenderTarget &target, sf::RenderStates states) const;
		void drawChildren(sf::RenderTarget &target, sf::RenderStates states) const;

		void update(CommandQueue &command_queue, const sf::Time df);
		virtual void updateCurrent(CommandQueue &command_queue, const sf::Time df);
		void updateChildren(CommandQueue &command_queue, const sf::Time df);

		void addChild(PtrNode child);

		void recieveCommand(Command &command, const sf::Time df);

		virtual unsigned int getCategory() const;
		sf::Transform getWorlTransform();

	private:
		SceneNode* parent;
		std::vector<PtrNode> children;
		
};

