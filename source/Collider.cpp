#include "..\header\Collider.h"

#include "..\header\Pacman.h"
#include "..\header\Map.h"
#include "..\header\Entity.h"



Collider::Collider(SceneNode &c_scene_graph) : scene_graph(c_scene_graph)
{
	CommandCollision player_or_ghost_dies;

	//Register action
	player_or_ghost_dies.action = get_function_collision<Pacman, Entity>([](Pacman &pacman, Entity &entity, sf::Time df)
	{
		/*if (pacman.getGlobalRect().intersects(ghost.getGlobalRect()) == true)
		{
			if (pacman.getState() == Behaviour::Chase)
			{
				pacman.addScore(ghost.getPoints());
				ghost.die();
			}
			else if (pacman.getState() == Behaviour::Escape))
			{
				pacman.die();
				
				if (pacman.isDead())
				{
					world.changeState(Transitions::GameOver)
				}
				else
				{
					world.restartPositions();
				}
			}
		}*/
	});

	interactions[std::make_pair(Category::Pacman, Category::Ghost)] = player_or_ghost_dies;
}


Collider::~Collider()
{
}

void Collider::checkCollisions(CommandQueue &command_queue)
{
	
}




