#pragma once

#include <SFML/Graphics.hpp>
#include <iostream>
#include <vector>
#include <set>
#include <map>
#include <functional>
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
		Warp,
		Intersection,
		SmallDot,
		BigDot,
		Cherry,
		Strawerry,
		Peach,
		Apple,
		Grapes,
		Galaxian,
		Bell,
		Key,
		Text,
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
		void deleteChild(SceneNode &node);
		void recieveCommand(Command &command, const sf::Time df);

		void removeFromScene();
		void deleteRemovable(std::vector<SceneNode*> &scene_vector);
		inline bool isRemovable(){ return delete_from_scene == true; }
		inline void activate(){ active = true; }
		inline void desactivate(){ active = false; }
		inline const bool isActive() const { return active; }
		inline void activateAll(){ activate(); for (auto &itr : children) itr->activateAll(); }
			
		
		virtual unsigned int getCategory() const;
		sf::Transform getWorldTransform();

	 
	private:
		SceneNode* parent;
		std::vector<PtrNode> children;	
		bool delete_from_scene;
		bool active;
		
};

