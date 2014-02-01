#include "..\header\Collider.h"

#include "..\header\World.h"
#include "..\header\Pacman.h"
#include "..\header\Map.h"
#include "..\header\Ghost.h"
#include "..\header\PickupObject.h"
#include "..\header\Warp.h"
#include "..\header\Text.h"



Collider::Collider(World &c_world, TMX::Map &c_map, CommandQueue &c_command_queue)
: world(c_world)
, map(c_map)
, command_queue(c_command_queue)
{
	//REGISTER EVENT MAP
	events[CollisionEvent::PlayerGhost] = std::make_pair(Category::Pacman, Category::Ghost);
	events[CollisionEvent::PlayerSmallDot] = std::make_pair(Category::Pacman, Category::SmallDot);
	events[CollisionEvent::PlayerBigDot] = std::make_pair(Category::Pacman, Category::BigDot);
	events[CollisionEvent::PlayerWarp] = std::make_pair(Category::Pacman, Category::Warp);

	registerCollisions();
}


Collider::~Collider()
{
}

void Collider::registerCollisions()
{
	//CREATE FUNCTION OBJECTS WITH LAMBDAS THAT ENCAPSULATES THE BEHAVIOURS OF EACH COLLISION DEPENDING ON THE PAIRS
	CommandCollision player_or_ghost_dies;
	CommandCollision player_eats_small_dot;
	CommandCollision player_eats_big_dot;
	CommandCollision player_warp;

	//Register actions
	player_or_ghost_dies.action = get_function_collision<Pacman, Ghost>([](Pacman &pacman, Ghost &ghost)
	{
		if (pacman.getGlobalRect().intersects(ghost.getGlobalRect()) == true)
		{
			//std::cout << "Collision" << std::endl;
			/*if (pacman.getState() == Behaviour::Chase)
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
			}*/
		}
	});

	player_eats_small_dot.action = get_function_collision<Pacman, PickupObject>([this](Pacman &pacman, PickupObject &pickup)
	{
		if (pacman.getInteriorRect().intersects(pickup.getGlobalRect()) == true)
		{
			Command update_score;
			update_score.category = Category::Text;
			pacman.addScore(pickup.getScore());
	
			update_score.action = get_function<Text>([&](Text &text, sf::Time dt)
			{
				if(text.getType() == TextType::Score)
				text.setText(std::to_string(pacman.getScore()));

				if (text.getType() == TextType::HighScore && pacman.isHigherScore())
				text.setText(std::to_string(pacman.getScore()));

			});

			command_queue.push(update_score);

			pickup.removeFromScene();
		}
	});

	player_eats_big_dot.action = get_function_collision<Pacman, PickupObject>([this](Pacman &pacman, PickupObject &pickup)
	{
		if (pacman.getInteriorRect().intersects(pickup.getGlobalRect()) == true)
		{
			Command ghost_escape;
			ghost_escape.category = Category::Ghost;
			ghost_escape.action = get_function<Ghost>([&](Ghost &ghost, sf::Time dt)
			{
				ghost.escape();
			});
			
			command_queue.push(ghost_escape);
			pickup.removeFromScene();
		}
	});

	player_warp.action = get_function_collision<Pacman, Warp>([this](Pacman &pacman, Warp &warp)
	{
		if (pacman.getInteriorRect().intersects(warp.getGlobalRect()) == true)
		{
			pacman.setPosition(warp.getDestiny());
		}

	});

	//REGISTER COMMANDS
	interactions[events[CollisionEvent::PlayerGhost]] = player_or_ghost_dies;
	interactions[events[CollisionEvent::PlayerSmallDot]] = player_eats_small_dot;
	interactions[events[CollisionEvent::PlayerBigDot]] = player_eats_big_dot;
	interactions[events[CollisionEvent::PlayerWarp]] = player_warp;

}

//Iterate trough all pairs avoiding repeated pairs
void Collider::checkSceneCollision(std::vector<SceneNode*> &nodes)
{
	std::vector<SceneNode*>::iterator first_itr = nodes.begin();
	std::vector<SceneNode*>::iterator second_itr = first_itr;
	second_itr++;

	while (second_itr != nodes.end())
	{
		for (std::vector<SceneNode*>::iterator itr = second_itr; itr != nodes.end(); ++itr)
		{
			//std::cout << (*first_itr)->getCategory() << " " << (*itr)->getCategory() << std::endl;
			checkCollision(**first_itr, **itr);
		}
		second_itr++;
		first_itr++;
	}	

	//std::cout << std::endl;
}





