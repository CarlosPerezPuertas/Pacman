#include "..\header\Ghost.h"
#include "..\header\Pacman.h"

//### SCATTER ###

GhostScatter::GhostScatter()
: elapsed(sf::Time::Zero)
{

}

void GhostScatter::enter(Ghost *entity)
{
	elapsed = sf::seconds(5.f);
	entity->setCurrentTarget(entity->getScatterTarget());
	entity->changeAnimation(GhostAnimations::GoLeft);
	entity->setIdTileMap(0); //Force to check direction again
}

void GhostScatter::execute(Ghost *entity, sf::Time dt)
{
	entity->exit_cave_elapsed -= dt;
	entity->updateAnimation(dt);

	if (entity->isElroy2()){ entity->changeState(GhostBehaviour::Chase); }
	else if (entity->isElroy1()) { entity->changeState(GhostBehaviour::Chase); }

	if (entity->exit_cave_elapsed <= sf::Time::Zero)
	{
		elapsed -= dt;
		if (elapsed > sf::Time::Zero) choosePath(dt, entity);
		else entity->changeState(GhostBehaviour::Chase);	
	}	
}

void GhostScatter::exit(Ghost *entity)
{

	
}

void GhostScatter::choosePath(sf::Time dt, Ghost *ghost)
{
	ghost->discardOppositeDirection();
	selectDirection(ghost);

	//Force to go up when we enter to the init cave
	if (ghost->getTileMap() + 1 == 465) ghost->setCurrentDirection(Direction::Up);

	moveByDirection(dt, ghost);

	
}

void GhostScatter::moveByDirection(sf::Time dt, Ghost *ghost)
{
	if (ghost->getCurrentDirection() == Direction::Left)
	{
		ghost->goLeft(dt);
		if (ghost->getAnimationState() != GhostAnimations::GoLeft) ghost->changeAnimation(GhostAnimations::GoLeft);
	}
	else if (ghost->getCurrentDirection() == Direction::Right)
	{
		ghost->goRight(dt);
		if (ghost->getAnimationState() != GhostAnimations::GoRight) ghost->changeAnimation(GhostAnimations::GoRight);
	}
	else if (ghost->getCurrentDirection() == Direction::Up)
	{
		ghost->goUp(dt);
		if (ghost->getAnimationState() != GhostAnimations::GoUp) ghost->changeAnimation(GhostAnimations::GoUp);
	}
	else if (ghost->getCurrentDirection() == Direction::Down)
	{
		ghost->goDown(dt);
		if (ghost->getAnimationState() != GhostAnimations::GoDown) ghost->changeAnimation(GhostAnimations::GoDown);
	}
}
void GhostScatter::selectDirection(Ghost *ghost)
{
		int row_ghost = ghost->getTileMap() / 30;
		int column_ghost = ghost->getTileMap() % 30;


		int row_ghost_left = ghost->getTileMap() / 30;
		int column_ghost_left = (ghost->getTileMap() - 1) % 30;
		int row_ghost_right = ghost->getTileMap() / 30;
		int column_ghost_right = (ghost->getTileMap() + 1) % 30;
		int row_ghost_up = (ghost->getTileMap() - 30) / 30;
		int column_ghost_up = ghost->getTileMap() % 30;
		int row_ghost_down = (ghost->getTileMap() + 30) / 30;
		int column_ghost_down = ghost->getTileMap() % 30;

		int row_target = ghost->getCurrentTarget() / 30;
		int column_target = ghost->getCurrentTarget() % 30;

		int row_diff = row_ghost - row_target;
		int col_diff = column_ghost - column_target;

		int row_diff_left = row_ghost_left - row_target;
		int col_diff_left = column_ghost_left - column_target;
		int row_diff_right = row_ghost_right - row_target;
		int col_diff_right = column_ghost_right - column_target;
		int row_diff_up = row_ghost_up - row_target;
		int col_diff_up = column_ghost_up - column_target;
		int row_diff_down = row_ghost_down - row_target;
		int col_diff_down = column_ghost_down - column_target;

		float left_magnitude = 1000.f;
		float right_magnitude = 1000.f;
		float up_magnitude = 1000.f;
		float down_magnitude = 1000.f;

		auto directions = ghost->getPossibleDirections();

		for (auto &itr : directions)
		{
			if (itr == Direction::Left)
				left_magnitude = sqrt(powf(static_cast<float>(row_diff_left), 2) + powf(static_cast<float>(col_diff_left), 2));
			else if (itr == Direction::Right)
				right_magnitude = sqrt(powf(static_cast<float>(row_diff_right), 2) + powf(static_cast<float>(col_diff_right), 2));
			else if (itr == Direction::Up)
				up_magnitude = sqrt(powf(static_cast<float>(row_diff_up), 2) + powf(static_cast<float>(col_diff_up), 2));
			else if (itr == Direction::Down)
				down_magnitude = sqrt(powf(static_cast<float>(row_diff_down), 2) + powf(static_cast<float>(col_diff_down), 2));
		}


		if (left_magnitude != 1000.f &&
			(left_magnitude < right_magnitude && left_magnitude < up_magnitude && left_magnitude < down_magnitude))
		{
			ghost->selectLeftDirection();
		}
		else if (right_magnitude != 1000.f &&
			(right_magnitude < left_magnitude && right_magnitude < up_magnitude && right_magnitude < down_magnitude))
		{
			ghost->selectRightDirection();
		}
		else if (up_magnitude != 1000.f &&
			(up_magnitude < right_magnitude && up_magnitude < left_magnitude && up_magnitude < down_magnitude))
		{
			ghost->selectUpDirection();
		}
		else if (down_magnitude != 1000.f &&
			(down_magnitude < right_magnitude && down_magnitude < up_magnitude && down_magnitude < left_magnitude))
		{
			ghost->selectDownDirection();
		}

		bool is_going_away = false;
		bool isHorizontal = false;
		bool isVertical = false;

		if (abs(row_diff) <= abs(col_diff)) isVertical = true;
		else isHorizontal = true;


		if (isHorizontal && row_diff >= 0 && ghost->getCurrentDirection() == Direction::Down) is_going_away = true;
		if (isHorizontal && row_diff < 0 && ghost->getCurrentDirection() == Direction::Up) is_going_away = true;
		if (isVertical && col_diff <= 0 && ghost->getCurrentDirection() == Direction::Left) is_going_away = true;
		if (isVertical && col_diff > 0 && ghost->getCurrentDirection() == Direction::Right) is_going_away = true;

		if (is_going_away && directions.size() > 1)
		{
			ghost->removeDirection(ghost->getCurrentDirection());
			selectDirection(ghost); //Recursion
		}	
}

//### CHASE ###

GhostChase::GhostChase()
{

}

void GhostChase::enter(Ghost *entity)
{
	if (entity->getCurrentDirection() == Direction::Down) entity->setCurrentDirection(Direction::Up);
	if (entity->getCurrentDirection() == Direction::Up) entity->setCurrentDirection(Direction::Down);
	if (entity->getCurrentDirection() == Direction::Left) entity->setCurrentDirection(Direction::Right);
	if (entity->getCurrentDirection() == Direction::Right) entity->setCurrentDirection(Direction::Left);

	entity->setIdTileMap(0); //Force to check direction again
	entity->addDirection(entity->getCurrentDirection());	
}

void GhostChase::execute(Ghost *entity, sf::Time dt)
{
	entity->setCurrentTarget(entity->getChaseTarget());
	choosePath(dt, entity);
	entity->updateAnimation(dt);
}

void GhostChase::exit(Ghost *entity)
{

}

void GhostChase::choosePath(sf::Time dt, Ghost *ghost)
{
	ghost->discardOppositeDirection();

	last_direction = ghost->getCurrentDirection();
	selectDirection(ghost);

	// Force to go up when we enter to the init cave
	if (ghost->getTileMap() + 1 == 465) ghost->setCurrentDirection(Direction::Up);

	moveByDirection(dt, ghost);
}

void GhostChase::moveByDirection(sf::Time dt, Ghost *ghost)
{
	if (ghost->getCurrentDirection() == Direction::Left)
	{
		ghost->goLeft(dt);
		if (ghost->getAnimationState() != GhostAnimations::GoLeft) ghost->changeAnimation(GhostAnimations::GoLeft);
	}
	else if (ghost->getCurrentDirection() == Direction::Right)
	{
		ghost->goRight(dt);
		if (ghost->getAnimationState() != GhostAnimations::GoRight) ghost->changeAnimation(GhostAnimations::GoRight);
	}
	else if (ghost->getCurrentDirection() == Direction::Up)
	{
		ghost->goUp(dt);
		if (ghost->getAnimationState() != GhostAnimations::GoUp) ghost->changeAnimation(GhostAnimations::GoUp);
	}
	else if (ghost->getCurrentDirection() == Direction::Down)
	{
		ghost->goDown(dt);
		if (ghost->getAnimationState() != GhostAnimations::GoDown) ghost->changeAnimation(GhostAnimations::GoDown);
	}
}

void GhostChase::selectDirection(Ghost *ghost)
{
	auto directions = ghost->getPossibleDirections();
	bool selected = false;

	if (directions.size() == 1)
	{
		for (auto &itr : directions) ghost->setCurrentDirection(itr);
	}
	else if (directions.size() > 1)
	{

	int row_ghost = ghost->getTileMap() / 30;
	int column_ghost = ghost->getTileMap() % 30;

	int row_ghost_left = ghost->getTileMap() / 30;
	int column_ghost_left = (ghost->getTileMap() - 1) % 30;
	int row_ghost_right = ghost->getTileMap() / 30;
	int column_ghost_right = (ghost->getTileMap() + 1) % 30;
	int row_ghost_up = (ghost->getTileMap() - 30) / 30;
	int column_ghost_up = ghost->getTileMap() % 30;
	int row_ghost_down = (ghost->getTileMap() + 30) / 30;
	int column_ghost_down = ghost->getTileMap() % 30;

	int row_target = ghost->getCurrentTarget() / 30;
	int column_target = ghost->getCurrentTarget() % 30;

	int row_diff = row_ghost - row_target;
	int col_diff = column_ghost - column_target;

	int row_diff_left = row_ghost_left - row_target;
	int col_diff_left = column_ghost_left - column_target;
	int row_diff_right = row_ghost_right - row_target;
	int col_diff_right = column_ghost_right - column_target;
	int row_diff_up = row_ghost_up - row_target;
	int col_diff_up = column_ghost_up - column_target;
	int row_diff_down = row_ghost_down - row_target;
	int col_diff_down = column_ghost_down - column_target;

	float left_magnitude = 1000.f;
	float right_magnitude = 1000.f;
	float up_magnitude = 1000.f;
	float down_magnitude = 1000.f;

	

	for (auto &itr : directions)
	{
		if (itr == Direction::Left)
			left_magnitude = sqrt(powf(static_cast<float>(row_diff_left), 2) + powf(static_cast<float>(col_diff_left), 2));
		else if (itr == Direction::Right)
			right_magnitude = sqrt(powf(static_cast<float>(row_diff_right), 2) + powf(static_cast<float>(col_diff_right), 2));
		else if (itr == Direction::Up)
			up_magnitude = sqrt(powf(static_cast<float>(row_diff_up), 2) + powf(static_cast<float>(col_diff_up), 2));
		else if (itr == Direction::Down)
			down_magnitude = sqrt(powf(static_cast<float>(row_diff_down), 2) + powf(static_cast<float>(col_diff_down), 2));
	}



	if (left_magnitude != 1000.f &&
		(left_magnitude <= right_magnitude && left_magnitude <= up_magnitude && left_magnitude <= down_magnitude) 
		&& ghost->isPossibleDirection(Direction::Left))
	{
		selected  = ghost->selectLeftDirection();
	}
	else if (right_magnitude != 1000.f &&
		(right_magnitude < left_magnitude && right_magnitude < up_magnitude && right_magnitude < down_magnitude)
		&& ghost->isPossibleDirection(Direction::Right))
	{
		selected  = ghost->selectRightDirection();
	}
	else if (up_magnitude != 1000.f &&
		(up_magnitude <= right_magnitude && up_magnitude <= left_magnitude && up_magnitude <= down_magnitude)
		&& ghost->isPossibleDirection(Direction::Up))
	{
		selected  = ghost->selectUpDirection();
	}
	else if (down_magnitude != 1000.f &&
		(down_magnitude < right_magnitude && down_magnitude < up_magnitude && down_magnitude < left_magnitude)
		&& ghost->isPossibleDirection(Direction::Down))
	{
		selected  = ghost->selectDownDirection();
	}

	bool is_going_away = false;
	bool isHorizontal = false;
	bool isVertical = false;

	if (abs(row_diff) <= abs(col_diff)) isVertical = true;
	else isHorizontal = true;


	if (isHorizontal && row_diff >= 0 && ghost->getCurrentDirection() == Direction::Down) is_going_away = true;
	if (isHorizontal && row_diff < 0 && ghost->getCurrentDirection() == Direction::Up) is_going_away = true;
	if (isVertical && col_diff <= 0 && ghost->getCurrentDirection() == Direction::Left) is_going_away = true;
	if (isVertical && col_diff > 0 && ghost->getCurrentDirection() == Direction::Right) is_going_away = true;

	if (ghost->getCurrentDirection() == Direction::None) ghost->setCurrentDirection(last_direction); //Solution to the lst problem. Ghost collides with pacman on the back

	else if (is_going_away == true && directions.size() > 1 && selected == true)
	{
		selected = false;
		is_going_away = false;
		ghost->removeDirection(ghost->getCurrentDirection());
		ghost->setCurrentDirection(Direction::None);
		selectDirection(ghost); //Recursion
		
	}
	}



}




//### ESCAPE ###
GhostFrightened::GhostFrightened()
: frightened_time(sf::Time::Zero)
, init_flash_time(sf::Time::Zero)
, flashing(false)
{

}

void GhostFrightened::enter(Ghost *entity)
{
	entity->changeAnimation(GhostAnimations::Escape);
	frightened_time = entity->frightened_time;
	init_flash_time = frightened_time / 3.f;
	entity->setSpeed(entity->getSpeed(), entity->getFrightenedRatio());
	entity->setIdTileMap(0); //Force to check direction again
}

void GhostFrightened::execute(Ghost *entity, sf::Time dt)
{
	entity->exit_cave_elapsed -= dt;

	frightened_time -= dt;
	entity->updateAnimation(dt);

	if (entity->exit_cave_elapsed <= sf::Time::Zero) randomPath(dt, entity);

	if (frightened_time <= init_flash_time && flashing == false)
	{
		flashing = true;
		entity->changeAnimation(GhostAnimations::EscapeFlashing);
	}

	if (frightened_time <= sf::Time::Zero) entity->changeState(GhostBehaviour::Scatter);
}


void GhostFrightened::exit(Ghost *entity)
{
	entity->changeAnimation(GhostAnimations::GoRight);
}

void GhostFrightened::randomPath(sf::Time dt, Ghost *ghost)
{

	ghost->discardOppositeDirection();

	auto directions = ghost->getPossibleDirections();

	if (directions.size() == 1) for (auto &itr : directions) ghost->setCurrentDirection(itr);
	else if (!directions.empty())
	{
		std::vector<Direction> random_directions;
		for (auto &itr : directions) random_directions.push_back(itr);
		int num_directions = random_directions.size();
		int random_direction = rand() % num_directions;
		ghost->setCurrentDirection(random_directions[random_direction]);
	}

	moveByDirection(dt, ghost);

	if (ghost->getTileMap() + 1 == 465) ghost->setCurrentDirection(Direction::Up);
	
}
void GhostFrightened::moveByDirection(sf::Time dt, Ghost *ghost)
{
	if (ghost->getCurrentDirection() == Direction::Left) ghost->goLeft(dt);
	else if (ghost->getCurrentDirection() == Direction::Right) ghost->goRight(dt);
	else if (ghost->getCurrentDirection() == Direction::Up) ghost->goUp(dt);
	else if (ghost->getCurrentDirection() == Direction::Down) ghost->goDown(dt);
}


//### RETURN TO CAVE ###
void GhostCaveReturn::enter(Ghost *entity)
{
	entity->setCurrentTarget(entity->getCaveReturnTarget());
	entity->setSpeed(entity->getSpeed(), entity->getVelRatio());
}

void GhostCaveReturn::execute(Ghost *entity, sf::Time dt)
{
	entity->updateAnimation(dt);
	choosePath(dt, entity);

	if (entity->getTileMap() + 1 == entity->getCaveReturnTarget())
	{
		entity->changeState(GhostBehaviour::Scatter);
	}
}

void GhostCaveReturn::exit(Ghost *entity)
{

}

void GhostCaveReturn::choosePath(sf::Time dt, Ghost *ghost)
{
	ghost->discardOppositeDirection();
	selectDirection(ghost);
	moveByDirection(dt, ghost);

}

void GhostCaveReturn::moveByDirection(sf::Time dt, Ghost *ghost)
{
	if (ghost->getCurrentDirection() == Direction::Left)
	{
		ghost->goLeft(dt);
		if (ghost->getAnimationState() != GhostAnimations::EyeLeft) ghost->changeAnimation(GhostAnimations::EyeLeft);
	}
	else if (ghost->getCurrentDirection() == Direction::Right)
	{
		ghost->goRight(dt);
		if (ghost->getAnimationState() != GhostAnimations::EyeRight) ghost->changeAnimation(GhostAnimations::EyeRight);
	}
	else if (ghost->getCurrentDirection() == Direction::Up)
	{
		ghost->goUp(dt);
		if (ghost->getAnimationState() != GhostAnimations::EyeUp) ghost->changeAnimation(GhostAnimations::EyeUp);
	}
	else if (ghost->getCurrentDirection() == Direction::Down)
	{
		ghost->goDown(dt);
		if (ghost->getAnimationState() != GhostAnimations::EyeDown) ghost->changeAnimation(GhostAnimations::EyeDown);
	}

}

void GhostCaveReturn::selectDirection(Ghost *ghost)
{
	int row_ghost = ghost->getTileMap() / 30;
	int column_ghost = ghost->getTileMap() % 30;

	int row_target = ghost->getCurrentTarget() / 30;
	int column_target = ghost->getCurrentTarget() % 30;

	int row_diff = row_ghost - row_target;
	int col_diff = column_ghost - column_target;

	bool isHorizontal = false;
	bool isVertical = false;
	bool is_going_away = false;

	auto directions = ghost->getPossibleDirections();

	if (directions.size() == 1) for (auto &itr : directions) ghost->setCurrentDirection(itr);
	else if (!directions.empty())
	{
		int vertical_selected = false;
		int horizontal_selected = false;

		if (abs(row_diff) <= abs(col_diff))
		{
			if (row_diff > 0) vertical_selected = ghost->selectUpDirection();
			else if (row_diff <= 0) vertical_selected = ghost->selectDownDirection();
			isVertical = true;
		}
		else
		{
			if (col_diff > 0) horizontal_selected = ghost->selectLeftDirection();
			else if (col_diff <= 0) horizontal_selected = ghost->selectRightDirection();
			isHorizontal = true;
		}

		if (isVertical && !vertical_selected)
		{
			if (col_diff > 0) horizontal_selected = ghost->selectLeftDirection();
			else if (col_diff <= 0) horizontal_selected = ghost->selectRightDirection();
		}
		else if (isHorizontal && !horizontal_selected)
		{
			if (row_diff > 0) vertical_selected = ghost->selectUpDirection();
			else if (row_diff <= 0) vertical_selected = ghost->selectDownDirection();
		}

		// If is going away from the target we change the axis
		if (isHorizontal && row_diff >= 0 && ghost->getCurrentDirection() == Direction::Down) is_going_away = true;
		if (isHorizontal && row_diff < 0 && ghost->getCurrentDirection() == Direction::Up) is_going_away = true;
		if (isVertical && col_diff <= 0 && ghost->getCurrentDirection() == Direction::Left) is_going_away = true;
		if (isVertical && col_diff > 0 && ghost->getCurrentDirection() == Direction::Right) is_going_away = true;

		if (is_going_away)
		{
			ghost->removeDirection(ghost->getCurrentDirection());
			selectDirection(ghost); //Recursion
		}
	}
}


int Ghost::ghosts_eaten = 0;

Ghost::Ghost(GhostType c_type, Textures::Type type, TextureGenerator &tg)
: MoveableObject(tg.get(type))
, state_machine(*this)
, animation(Entity::sprite, sf::seconds(0.1f), GhostAnimations::GoLeft)
, type(c_type)
, num_scatter_target(0)
, num_chase_target(0)
, num_cave_return_target(0)
, current_target(0)
, exit_cave_time(sf::Time::Zero)
, exit_cave_elapsed(sf::Time::Zero)
, frightened_time(sf::Time::Zero)
{
	//Set the center in order to apply transformations
	const float num_frames_width = 3.f;
	const float num_frames_height = 6.f;
	sf::FloatRect local_rect = getGlobalRect();
	sf::FloatRect frame_rect = sf::FloatRect(0.f,
											 0.f,
											 local_rect.width / num_frames_width,
											 local_rect.height / num_frames_height);

	Entity::sprite.setOrigin(frame_rect.width / 2.f , frame_rect.height / 2.f );


	animation.addFrame(GhostAnimations::Escape, sf::IntRect(0, 80, 20, 20));
	animation.addFrame(GhostAnimations::Escape, sf::IntRect(20, 80, 20, 20));

	animation.addFrame(GhostAnimations::EscapeFlashing, sf::IntRect(0, 80, 20, 20));
	animation.addFrame(GhostAnimations::EscapeFlashing, sf::IntRect(20, 80, 20, 20));
	animation.addFrame(GhostAnimations::EscapeFlashing, sf::IntRect(0, 100, 20, 20));
	animation.addFrame(GhostAnimations::EscapeFlashing, sf::IntRect(20, 100, 20, 20));

	animation.addFrame(GhostAnimations::GoRight, sf::IntRect(0, 60, 20, 20));
	animation.addFrame(GhostAnimations::GoRight, sf::IntRect(20, 60, 20, 20));
	animation.addFrame(GhostAnimations::EyeRight, sf::IntRect(40, 60, 20, 20));

	animation.addFrame(GhostAnimations::GoLeft, sf::IntRect(0, 40, 20, 20));
	animation.addFrame(GhostAnimations::GoLeft, sf::IntRect(20, 40, 20, 20));
	animation.addFrame(GhostAnimations::EyeLeft, sf::IntRect(40, 40, 20, 20));

	animation.addFrame(GhostAnimations::GoUp, sf::IntRect(0, 0, 20, 20));
	animation.addFrame(GhostAnimations::GoUp, sf::IntRect(20, 0, 20, 20));
	animation.addFrame(GhostAnimations::EyeUp, sf::IntRect(40, 0, 20, 20));
	
	animation.addFrame(GhostAnimations::GoDown, sf::IntRect(0, 20, 20, 20));
	animation.addFrame(GhostAnimations::GoDown, sf::IntRect(20, 20, 20, 20));
	animation.addFrame(GhostAnimations::EyeDown, sf::IntRect(40, 20, 20, 20));

	animation.play();

	state_machine.registerState<GhostFrightened>(GhostBehaviour::Frightened);
	state_machine.registerState<GhostChase>(GhostBehaviour::Chase);
	state_machine.registerState<GhostScatter>(GhostBehaviour::Scatter);
	state_machine.registerState<GhostCaveReturn>(GhostBehaviour::CaveReturn);
	state_machine.changeState(GhostBehaviour::Scatter);
}


Ghost::~Ghost()
{

}

void Ghost::drawCurrent(sf::RenderTarget &target, sf::RenderStates states) const
{
	if (isActive()) target.draw(Entity::sprite, states);
}

void Ghost::updateCurrent(CommandQueue &command_queue, const sf::Time dt)
{
	if (isActive())
	{
		state_machine.update(dt);

	}
}

bool Ghost::selectRightDirection()
{
	bool is_selected = false;

	auto directions = getPossibleDirections();

	auto itr2 = directions.find(Direction::Right);
	if (itr2 != directions.end())
	{
		setCurrentDirection(Direction::Right);
		is_selected = true;
	}

	return is_selected;
}

bool Ghost::selectLeftDirection()
{
	bool is_selected = false;

	auto directions = getPossibleDirections();

	auto itr2 = directions.find(Direction::Left);
	if (itr2 != directions.end())
	{
		setCurrentDirection(Direction::Left);
		is_selected = true;
	}

	return is_selected;
}

bool Ghost::selectUpDirection()
{
	bool is_selected = false;

	auto directions = getPossibleDirections();

	auto itr2 = directions.find(Direction::Up);
	if (itr2 != directions.end())
	{
		setCurrentDirection(Direction::Up);
		is_selected = true;
	}

	return is_selected;
}

bool Ghost::selectDownDirection()
{
	bool is_selected = false;

	auto directions = getPossibleDirections();

	auto itr2 = directions.find(Direction::Down);
	if (itr2 != directions.end())
	{
		setCurrentDirection(Direction::Down);
		is_selected = true;
	}

	return is_selected;
}


//The ghosts cannot go to the opposite site
void Ghost::discardOppositeDirection()
{
	if (getCurrentDirection() == Direction::Up) removeDirection(Direction::Down);
	else if (getCurrentDirection() == Direction::Down) removeDirection(Direction::Up);
	else if (getCurrentDirection() == Direction::Left) removeDirection(Direction::Right);
	else if (getCurrentDirection() == Direction::Right) removeDirection(Direction::Left);
}


unsigned int Ghost::getCategory() const
{
	return Category::Ghost;
}


GhostBehaviour Ghost::getCurrentBehaviour()
{
	return state_machine.getCurrentState();
}

void Ghost::restart()
{
	restartPosition(); 

	if (getType() == GhostType::Blinky) setCurrentDirection(Direction::Right);
	else if (getType() == GhostType::Pinky) setCurrentDirection(Direction::Left);
	else if (getType() == GhostType::Inky) setCurrentDirection(Direction::Right);
	else if (getType() == GhostType::Clyde) setCurrentDirection(Direction::Left);

	state_machine.changeState(GhostBehaviour::Scatter);
	desactivate(3.35f);
}
