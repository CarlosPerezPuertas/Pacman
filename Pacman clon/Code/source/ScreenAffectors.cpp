#include "..\header\ScreenAffectors.h"

#include "..\header\ScreenEffect.h"


FadeAffector::FadeAffector(double i_alpha, double e_alpha) 
: init_alpha(i_alpha)
, end_alpha(e_alpha)
{  
	affector = createAffector();
}


std::function<void(ScreenEffect&, sf::Time)>
FadeAffector::createAffector()
{
	return[this](ScreenEffect& effect, sf::Time df) mutable
	{
		effect.elapsed += df;
		double ratio = static_cast<double>(effect.elapsed.asMilliseconds()) / static_cast<double>(effect.life_time.asMilliseconds());
		double adapted_ratio = getInit() + ratio * (getEnd() - getInit());

		if (adapted_ratio > 0.0 && adapted_ratio < 1.0)
			effect.color.a = static_cast<sf::Uint8>(255 * adapted_ratio );
		else if (adapted_ratio <= 0.0) effect.color.a = static_cast<sf::Uint8>(0.0);
		else if (adapted_ratio >= 1.0) effect.color.a = static_cast<sf::Uint8>(1.0);
	};
}

void FadeAffector::init(ScreenEffect &effect)
{ 
	effect.setAlpha(static_cast<sf::Uint8>(255*init_alpha)); 
}
