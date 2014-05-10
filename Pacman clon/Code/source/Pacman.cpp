#include "..\header\Pacman.h"




Pacman::Pacman(Textures::Type type, TextureGenerator &tg, Player &c_player)
: MoveableObject(tg.get(type))
, animation(Entity::sprite, sf::seconds(0.13f), Animations::GoLeft)
, lifes(3)
, score(0)
, high_score(0)
, player(c_player)
{
	//Set the center in order to apply transformations
	const float num_frames_width = 11.f;
	const float num_frames_height = 5.f;
	sf::FloatRect local_rect = Entity::sprite.getLocalBounds();
	sf::FloatRect frame_rect = sf::FloatRect(0.f, 
		                                     0.f, 
											 local_rect.width / num_frames_width,
											 local_rect.height / num_frames_height);

	Entity::sprite.setOrigin(frame_rect.width / 2.f, frame_rect.height / 2.f);
	

	//Allowed directions
	possible_directions.insert(Direction::Right);
	possible_directions.insert(Direction::Left);
	
	//Add animations frame x frame
	animation.addFrame(Animations::GoLeft, sf::IntRect(0, 0, 20, 20));
	animation.addFrame(Animations::GoLeft, sf::IntRect(20, 0, 20, 20));
	animation.addFrame(Animations::GoLeft, sf::IntRect(40, 0, 20, 20));
	animation.addFrame(Animations::GoLeft, sf::IntRect(60, 0, 20, 20));

	animation.addFrame(Animations::GoRight, sf::IntRect(0, 20, 20, 20));
	animation.addFrame(Animations::GoRight, sf::IntRect(20, 20, 20, 20));
	animation.addFrame(Animations::GoRight, sf::IntRect(40, 20, 20, 20));
	animation.addFrame(Animations::GoRight, sf::IntRect(60, 20, 20, 20));

	animation.addFrame(Animations::GoUp, sf::IntRect(0, 40, 20, 20));
	animation.addFrame(Animations::GoUp, sf::IntRect(20, 40, 20, 20));
	animation.addFrame(Animations::GoUp, sf::IntRect(40, 40, 20, 20));
	animation.addFrame(Animations::GoUp, sf::IntRect(60, 40, 20, 20));

	animation.addFrame(Animations::GoDown, sf::IntRect(0, 60, 20, 20));
	animation.addFrame(Animations::GoDown, sf::IntRect(20, 60, 20, 20));
	animation.addFrame(Animations::GoDown, sf::IntRect(40, 60, 20, 20));
	animation.addFrame(Animations::GoDown, sf::IntRect(60, 60, 20, 20));

	animation.addFrame(Animations::Die, sf::IntRect(0,   80, 20, 20));
	animation.addFrame(Animations::Die, sf::IntRect(20,  80, 20, 20));
	animation.addFrame(Animations::Die, sf::IntRect(40,  80, 20, 20));
	animation.addFrame(Animations::Die, sf::IntRect(60,  80, 20, 20));
	animation.addFrame(Animations::Die, sf::IntRect(80,  80, 20, 20));
	animation.addFrame(Animations::Die, sf::IntRect(100, 80, 20, 20));
	animation.addFrame(Animations::Die, sf::IntRect(120, 80, 20, 20));
	animation.addFrame(Animations::Die, sf::IntRect(140, 80, 20, 20));
	animation.addFrame(Animations::Die, sf::IntRect(160, 80, 20, 20));
	animation.addFrame(Animations::Die, sf::IntRect(180, 80, 20, 20));
	animation.addFrame(Animations::Die, sf::IntRect(200, 80, 20, 20));
	animation.setUniqueLoop(Animations::Die);

	changeAnimation(Animations::GoLeft);
	setCurrentDirection(Direction::Left);

	std::ifstream record_file("record/record.txt");
	std::string best_score;

	if (record_file.is_open())
	{
		getline(record_file, best_score);
	}
	
	high_score = std::stoi(best_score);

	record_file.close();
}


void Pacman::drawCurrent(sf::RenderTarget &target, sf::RenderStates states) const
{
	if(isActive())
		target.draw(Entity::sprite, states);
}

void Pacman::updateCurrent(CommandQueue &command_queue, const sf::Time df)
{
	if (isActive()) animation.update(df);
	if (isDeadAnimationEnd()) desactivate();
}

void Pacman::changeAnimation(const Animations state)
{
	animation.changeState(state);
	animation.play();
}

void Pacman::restart()
{
	//activate();
	restartPosition();

	possible_directions.clear();
	possible_directions.insert(Direction::Right);
	possible_directions.insert(Direction::Left);

	setCurrentDirection(Direction::Left);
	changeAnimation(Animations::GoLeft);
	desactivate(3.35f);
}


unsigned int Pacman::getCategory() const
{ 
	return Category::Pacman;
}


void Pacman::die()
{
	if (!isDiying())
	{
		player.disable();
		lifes--;
		setCurrentDirection(Direction::None);
		changeAnimation(Animations::Die);
	}	
}

bool Pacman::hasLifes()
{
	return lifes > 0;
}




