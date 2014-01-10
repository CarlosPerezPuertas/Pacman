#include "..\header\SceneNode.h"


SceneNode::SceneNode() : parent(nullptr), children()
{
}


SceneNode::~SceneNode()
{
}

void SceneNode::draw(sf::RenderTarget &target, sf::RenderStates states) const
{
	
	states.transform *= getTransform();

	drawCurrent(target, states);
	drawChildren(target, states);

}
void SceneNode::drawCurrent(sf::RenderTarget &target, sf::RenderStates states) const
{

}
void SceneNode::drawChildren(sf::RenderTarget &target, sf::RenderStates states) const
{
	for (auto &itr : children)
	{
		itr->draw(target, states);
	}
}

void SceneNode::update(CommandQueue &command_queue, const sf::Time df)
{
	updateCurrent(command_queue, df);
	updateChildren(command_queue, df);
}

void SceneNode::updateCurrent(CommandQueue &command_queue, const sf::Time df)
{
	

}

void SceneNode::updateChildren(CommandQueue &command_queue, const sf::Time df)
{
	for (auto &itr : children)
	{
		itr->update(command_queue, df);
	}
}

void SceneNode::addChild(PtrNode child)
{
	child->parent = this;
	children.push_back(std::move(child));
}

void SceneNode::recieveCommand(Command &command, const sf::Time df)
{
	if (getCategory() == command.category)
	{
 		command.action(*this, df);
	}

	for (auto &itr : children)
	{
		itr->recieveCommand(command, df);
	}
}

unsigned int SceneNode::getCategory() const
{ 
	return Category::Other;
}

//We iterate trough the nodes above of the actual node to collect all the transforms
sf::Transform SceneNode::getWorlTransform()
{
	sf::Transform transform = sf::Transform::Identity;

	for (auto *itr = this; itr != nullptr; itr = itr->parent)
		transform = itr->getTransform() * transform;

	return transform;

}
