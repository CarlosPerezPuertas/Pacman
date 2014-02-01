#include "..\header\Player.h"

#include "..\header\Pacman.h"
#include "..\header\CommandQueue.h"


Player::Player() : current_action(GoRight)
{
	key_to_action[sf::Keyboard::Left]  = GoLeft;
	key_to_action[sf::Keyboard::Right] = GoRight;
	key_to_action[sf::Keyboard::Up]    = GoUp;
	key_to_action[sf::Keyboard::Down]  = GoDown;

	Command go_left; Command go_right; Command go_up; Command go_down;

	go_left.category = Category::Pacman;
	go_right.category = Category::Pacman;
	go_up.category = Category::Pacman;
	go_down.category = Category::Pacman;

	
	go_left.action = get_function<Pacman>([](Pacman &pacman, sf::Time dt)
	{
		if(pacman.isPossibleDirection(Direction::Left))
		{
			pacman.setCurrentDirection(Direction::Left);
			pacman.move(sf::Vector2f(-pacman.getSpeed() * dt.asSeconds(), 0));
			if (pacman.isOtherAnimation(Animations::GoLeft)) pacman.changeAnimation(Animations::GoLeft);
		}
		else if (pacman.isPossibleDirection(pacman.getCurrentDirection()))
		{
			pacman.moveByMap(pacman.getCurrentDirection(), dt);
		}
		else
		{
			pacman.stopAnimation();
			pacman.setCurrentDirection(Direction::None);	
		}	

		
	});

	go_right.action = get_function<Pacman>([](Pacman &pacman, sf::Time dt)
	{
		if (pacman.isPossibleDirection(Direction::Right))
		{
			pacman.setCurrentDirection(Direction::Right);
			pacman.move(sf::Vector2f(+pacman.getSpeed() * dt.asSeconds(), 0));
			if (pacman.isOtherAnimation(Animations::GoRight)) pacman.changeAnimation(Animations::GoRight);
		}
		else if (pacman.isPossibleDirection(pacman.getCurrentDirection()))
		{
			pacman.moveByMap(pacman.getCurrentDirection(), dt);
		}
		else
		{
			pacman.stopAnimation();
			pacman.setCurrentDirection(Direction::None);
		}

		
	});
	
	go_up.action = get_function<Pacman>([](Pacman &pacman, sf::Time dt)
	{ 
		if (pacman.isPossibleDirection(Direction::Up))
		{
			pacman.setCurrentDirection(Direction::Up);
			pacman.move(sf::Vector2f(0, -pacman.getSpeed() * dt.asSeconds()));
			if (pacman.isOtherAnimation(Animations::GoUp)) pacman.changeAnimation(Animations::GoUp);
		}
		else if (pacman.isPossibleDirection(pacman.getCurrentDirection()))
		{
			pacman.moveByMap(pacman.getCurrentDirection(), dt);
		}
		else
		{
			pacman.stopAnimation();
			pacman.setCurrentDirection(Direction::None);
		}

		
	});
	
	go_down.action = get_function<Pacman>([](Pacman &pacman, sf::Time dt)
	{ 
		if (pacman.isPossibleDirection(Direction::Down))
		{
			pacman.setCurrentDirection(Direction::Down);
			pacman.move(sf::Vector2f(0, pacman.getSpeed() * dt.asSeconds()));
			if (pacman.isOtherAnimation(Animations::GoDown)) pacman.changeAnimation(Animations::GoDown);
		}

		else if (pacman.isPossibleDirection(pacman.getCurrentDirection()))
		{
			pacman.moveByMap(pacman.getCurrentDirection(), dt);
		}
		else
		{
			pacman.stopAnimation();
			pacman.setCurrentDirection(Direction::None);
		}
	});

	action_to_command[GoLeft]  = go_left;
	action_to_command[GoRight] = go_right;
	action_to_command[GoUp]    = go_up;
	action_to_command[GoDown]  = go_down;
}



void Player::updateInput(CommandQueue &command_queue)
{
	
	for (auto &itr : key_to_action)
	{
		//If the key is pressed add to the set. If not then delete from the set.
		//If it's a new key then refresh the last_key_pressed and current_action
		if (sf::Keyboard::isKeyPressed(itr.first)) 
		{
			auto key = keys_pressed.find(itr.first);
			if (key == keys_pressed.end())
			{
				current_action = key_to_action[itr.first];
				last_key_pressed = itr.first;
			}
			keys_pressed.insert(itr.first);
		}
		else
		{
			auto key = keys_pressed.find(itr.first);
			if (key != keys_pressed.end()) keys_pressed.erase(itr.first);
		}
	}

	if(last_key_pressed != 0)
	command_queue.push(action_to_command[key_to_action[last_key_pressed]]);
}

