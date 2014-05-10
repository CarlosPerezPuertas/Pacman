#include "..\header\SceneNode.h"


SceneNode::SceneNode() 
: parent(nullptr)
, children()
, delete_from_scene(false)
, active(true)
, time_disabled(sf::Time::Zero)
, isTemporalDisabled(false)
, lifetime(sf::Time::Zero)
, lifetime_elapsed(sf::Time::Zero)
, hasLifetime(false)
{
}


SceneNode::~SceneNode()
{
}

void SceneNode::draw(sf::RenderTarget &target, sf::RenderStates states) const
{
	if (isActive())
	{
		states.transform *= getTransform();
		drawCurrent(target, states);
		drawChildren(target, states);	
	}	
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
	if (isActive())
	{
		updateCurrent(command_queue, df);
		updateChildren(command_queue, df);
	}	
	else if (isTemporalDisabled == true)
	{
		time_disabled -= df;
		if (time_disabled <= sf::Time::Zero) { activate(); isTemporalDisabled = false; }
	}
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

// Find the node and compare it with scenetree. if exist then get the iterator
// We move the object from the iterator and we set the parent to null
// We use the itr to delete the object from children.
// When PTRNODE RESULT is out of the scope it's deleted automatically
void SceneNode::deleteChild(SceneNode &node)
{
	auto itr = std::find_if(children.begin(), children.end(), [&](PtrNode &ptr) ->bool {return ptr.get() == &node; });
	assert(itr != children.end());

	/*PtrNode result = std::move(*itr);
	result->parent = nullptr;
	children.erase(itr);*/

	SceneNode *result = itr->release();
	result->parent = nullptr;
	children.erase(itr);
}

//Mark as removable using recursion
void SceneNode::removeFromScene()
{ 
	delete_from_scene = true;

	for (auto &itr : children)
	{
		itr->removeFromScene();
	}
}

//We delete the scene tree and the scene vector using recursion
void SceneNode::deleteRemovable(std::vector<SceneNode*> &scene_vector)
{
	auto delete_begin = std::remove_if(children.begin(), children.end(), std::mem_fn(&SceneNode::isRemovable));
	children.erase(delete_begin, children.end());

	auto node_to_remove_begin = std::remove_if(scene_vector.begin(), scene_vector.end(), [&](SceneNode *node) {return node->isRemovable(); });
	scene_vector.erase(node_to_remove_begin, scene_vector.end());
	
	std::for_each(children.begin(), children.end(), [&](PtrNode &node) 
	{
		node->deleteRemovable(scene_vector);
	});

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
sf::Transform SceneNode::getWorldTransform()
{
	sf::Transform transform = sf::Transform::Identity;

	for (auto *itr = this; itr != nullptr; itr = itr->parent)
		transform = itr->getTransform() * transform;

	return transform;
}
