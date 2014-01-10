
#include <SFML/Graphics.hpp>
#include <iostream>
#include <ctime>

#include "header\GameApp.h"


int main()
{
	srand(static_cast<unsigned int>(time(0)));
	GameApp game_app;
	game_app.run();
}

