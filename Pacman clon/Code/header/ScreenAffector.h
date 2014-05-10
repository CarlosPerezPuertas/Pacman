#pragma once

#include <SFML/Graphics.hpp>
#include <iostream>
#include <vector>
#include <map>
#include <cassert>
#include <functional>
#include "ResourceTypes.h"
#include "Resources.h"

class ScreenEffect;


class ScreenAffector
{
	public:
		typedef std::function<void(ScreenEffect&, sf::Time)> Affector;

	public:
		virtual ~ScreenAffector(){}
		std::function<void(ScreenEffect&, sf::Time)> get(){ return ScreenAffector::affector; }
		virtual void init(ScreenEffect &effect) = 0;
		

	protected:
		Affector affector;	
};



