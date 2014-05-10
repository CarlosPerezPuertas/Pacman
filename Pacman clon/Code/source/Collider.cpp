#include "..\header\Collider.h"

#include "..\header\World.h"
#include "..\header\Player.h"
#include "..\header\Pacman.h"
#include "..\header\Map.h"
#include "..\header\Ghost.h"
#include "..\header\PickupObject.h"
#include "..\header\Warp.h"
#include "..\header\Text.h"
#include "..\header\ImageContainer.h"



Collider::Collider(World &c_world, TMX::Map &c_map, CommandQueue &c_command_queue, Player &c_player)
: world(c_world)
, map(c_map)
, command_queue(c_command_queue)
, player(c_player)
{
	//REGISTER EVENT MAP
	events[CollisionEvent::PlayerGhost] = std::make_pair(Category::Pacman, Category::Ghost);
	events[CollisionEvent::PlayerSmallDot] = std::make_pair(Category::Pacman, Category::SmallDot);
	events[CollisionEvent::PlayerBigDot] = std::make_pair(Category::Pacman, Category::BigDot);
	events[CollisionEvent::PlayerBonus] = std::make_pair(Category::Pacman, Category::Bonus);
	events[CollisionEvent::PlayerWarp] = std::make_pair(Category::Pacman, Category::Warp);
	events[CollisionEvent::GhostWarp] = std::make_pair(Category::Ghost, Category::Warp);

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
	CommandCollision player_eats_bonus;
	CommandCollision player_warp;
	CommandCollision ghost_warp;

	//Register actions
	player_or_ghost_dies.action = get_function_collision<Pacman, Ghost>([this](Pacman &pacman, Ghost &ghost)
	{
		//if (pacman.getInteriorRect().intersects(ghost.getInteriorRect()) == true && !pacman.isDiying() )
		if (pacman.getInteriorRect().intersects(ghost.getInteriorRect()) == true && !pacman.isDiying())
		{
			if (ghost.getCurrentBehaviour() == GhostBehaviour::Frightened)
			{
				ghost.changeState(GhostBehaviour::CaveReturn);
				ghost.isEaten();

				Command update_score;
				update_score.category = Category::Text;
				
				std::stack<int> factor_v;
				for (int i = 0; i < ghost.getGhostsEaten(); ++i)
				{
					if (i == 0) factor_v.push(1);
					else factor_v.push(factor_v.top() * 2);
				}
				int score_factor = factor_v.top();
				int score = 200 * score_factor;
			
				pacman.addScore(score);
				world.refreshScore1up(score);

				update_score.action = get_function<Text>([&](Text &text, sf::Time dt)
				{
					if (text.getType() == TextType::Score)
						text.setText(std::to_string(pacman.getScore()));

					if (text.getType() == TextType::HighScore && pacman.isHigherScore())
					{
						text.setText(std::to_string(pacman.getScore()));

						std::ofstream record_file("record/record.txt");
						record_file << pacman.getScore();
						record_file.close();
					}
				});
				command_queue.push(update_score);

				std::unique_ptr<Text> points_text(new Text(std::to_string(score), "fonts/Press Start Regular.ttf", 12, TextType::Static, sf::Color(204, 73, 204, 255)));
				points_text->setPosition(sf::Vector2f(ghost.getPosition().x - 20, ghost.getPosition().y - 10));
				points_text->setLifeTime(3.f);
				world.addNodeToScene(std::move(points_text));

				//Play sound
				Command sound;
				sound.category = Category::SoundEatGhost;
				sound.action = get_function<SoundNode>([&](SoundNode &sound, sf::Time dt)
				{
					sound.play();
				});
				command_queue.push(sound);
			}
			else if (ghost.getCurrentBehaviour() != GhostBehaviour::Frightened &&
				     ghost.getCurrentBehaviour() != GhostBehaviour::CaveReturn)
			{
				Command remove_ghosts;
				remove_ghosts.category = Category::Ghost;

				remove_ghosts.action = get_function<Ghost>([&](Ghost &g, sf::Time dt)
				{
					g.desactivate();
				});

				command_queue.push(remove_ghosts);

				if (pacman.getLifes() <= 5)
				{
					Command pop_life_container;
					pop_life_container.category = Category::LifeContainer;

					pop_life_container.action = get_function<ImageContainer>([&](ImageContainer &ic, sf::Time dt)
					{
						ic.popBack();
					});
					command_queue.push(pop_life_container);
				}

				//Play sound
				Command sound;
				sound.category = Category::SoundPacmanDies;
				sound.action = get_function<SoundNode>([&](SoundNode &sound, sf::Time dt)
				{
					sound.play();
				});
				command_queue.push(sound);

				//Remove siren
				Command siren_off;
				siren_off.category = Category::SoundSiren;
				siren_off.action = get_function<SoundNode>([this](SoundNode &sound, sf::Time dt)
				{
					sound.stop();
				});
				command_queue.push(siren_off);

				pacman.die();
			}	
		}
	});

	player_eats_small_dot.action = get_function_collision<Pacman, PickupObject>([this](Pacman &pacman, PickupObject &pickup)
	{
		if (pacman.getInteriorRect().intersects(pickup.getInteriorRect()) == true && pickup.getCategory() == Category::SmallDot)
		{
			world.eatDot();

			Command update_score;
			update_score.category = Category::Text;
			pacman.addScore(pickup.getScore());
			world.refreshScore1up(pickup.getScore());
	
			update_score.action = get_function<Text>([&](Text &text, sf::Time dt)
			{
				if(text.getType() == TextType::Score)
				text.setText(std::to_string(pacman.getScore()));

				if (text.getType() == TextType::HighScore && pacman.isHigherScore())
				{
					text.setText(std::to_string(pacman.getScore()));
					std::ofstream record_file("record/record.txt");
					record_file << pacman.getScore();
					record_file.close();
				}
				
			});

			command_queue.push(update_score);

			Command sound;
			sound.category = Category::SoundWaka;
			sound.action = get_function<SoundNode>([&](SoundNode &sound, sf::Time dt)
			{
				if (pacman.getVelRatio() == 0.8f) sound.setPitch(1.0f);
				else if (pacman.getVelRatio() == 0.9f) sound.setPitch(1.024f);
				else if (pacman.getVelRatio() == 1.f) sound.setPitch(1.04f);

				sound.play();
				
			});
			command_queue.push(sound);

			pickup.removeFromScene();
		}
	});

	player_eats_big_dot.action = get_function_collision<Pacman, PickupObject>([this](Pacman &pacman, PickupObject &pickup)
	{
		if (pacman.getInteriorRect().intersects(pickup.getGlobalRect()) == true && pickup.getCategory() == Category::BigDot)
		{
			world.eatDot();

			Command update_score;
			update_score.category = Category::Text;
			pacman.addScore(pickup.getScore());
			world.refreshScore1up(pickup.getScore());

			update_score.action = get_function<Text>([&](Text &text, sf::Time dt)
			{
				if (text.getType() == TextType::Score)
					text.setText(std::to_string(pacman.getScore()));

				if (text.getType() == TextType::HighScore && pacman.isHigherScore())
				{
					text.setText(std::to_string(pacman.getScore()));

					std::ofstream record_file("record/record.txt");
					record_file << pacman.getScore();
					record_file.close();
				}
			});
			command_queue.push(update_score);

			Command ghost_escape;
			ghost_escape.category = Category::Ghost;
			ghost_escape.action = get_function<Ghost>([&](Ghost &ghost, sf::Time dt)
			{
				ghost.escape();
				ghost.ghostEatenToZero();

				Command siren2_on;
				siren2_on.category = Category::SoundBigDot;
				siren2_on.action = get_function<SoundNode>([&ghost](SoundNode &sound, sf::Time dt)
				{
					sound.playDuring(ghost.getFrightenedTime().asSeconds());
					sound.setLoop(true);
				});
				command_queue.push(siren2_on);

				Command siren_off;
				siren_off.category = Category::SoundSiren;
				siren_off.action = get_function<SoundNode>([&ghost](SoundNode &sound, sf::Time dt)
				{
					sound.stop();
					sound.playAfter(ghost.getFrightenedTime().asSeconds());
				});
				command_queue.push(siren_off);
			});
			command_queue.push(ghost_escape);

	
			pickup.removeFromScene();
		}
	});

	player_eats_bonus.action = get_function_collision<Pacman, PickupObject>([this](Pacman &pacman, PickupObject &pickup)
	{
		if (pacman.getInteriorRect().intersects(pickup.getGlobalRect()) == true && pickup.getCategory() == Category::Bonus)
		{
			//Update the score 
			Command update_score;
			update_score.category = Category::Text;
			pacman.addScore(pickup.getScore());
			world.refreshScore1up(pickup.getScore());

			update_score.action = get_function<Text>([&](Text &text, sf::Time dt)
			{
				if (text.getType() == TextType::Score)
					text.setText(std::to_string(pacman.getScore()));

				if (text.getType() == TextType::HighScore && pacman.isHigherScore())
				{
					text.setText(std::to_string(pacman.getScore()));

					std::ofstream record_file("record/record.txt");
					record_file << pacman.getScore();
					record_file.close();
				}
			});

			command_queue.push(update_score);

			//Create the text with acore adquired
			std::unique_ptr<Text> points_text(new Text(std::to_string(pickup.getScore()), "fonts/Press Start Regular.ttf", 14, TextType::Static, sf::Color(204, 73, 204, 255)));
			
			//Text Centered depending on the number of 0s
			if (pickup.getScore() / 1000 < 1) points_text->setPosition(sf::Vector2f(340.f, 433.f));
			else                              points_text->setPosition(sf::Vector2f(332.f, 433.f));

			points_text->setLifeTime(3.f);
			world.addNodeToScene(std::move(points_text));

			//Play sound
			Command sound;
			sound.category = Category::SoundEatPickup;
			sound.action = get_function<SoundNode>([&](SoundNode &sound, sf::Time dt)
			{
				sound.play();
			});
			command_queue.push(sound);

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

	ghost_warp.action = get_function_collision<Ghost, Warp>([this](Ghost &ghost, Warp &warp)
	{
		if (ghost.getInteriorRect().intersects(warp.getGlobalRect()) == true)
		{
			ghost.setPosition(warp.getDestiny());
		}
	});

	//REGISTER COMMANDS
	interactions[events[CollisionEvent::PlayerGhost]] = player_or_ghost_dies;
	interactions[events[CollisionEvent::PlayerSmallDot]] = player_eats_small_dot;
	interactions[events[CollisionEvent::PlayerBigDot]] = player_eats_big_dot;
	interactions[events[CollisionEvent::PlayerBonus]] = player_eats_bonus;
	interactions[events[CollisionEvent::PlayerWarp]] = player_warp;
	interactions[events[CollisionEvent::GhostWarp]]= ghost_warp;

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





