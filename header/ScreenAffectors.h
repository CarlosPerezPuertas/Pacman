#pragma once

#include "ScreenAffector.h"

class ScreenEffect;

class FadeAffector : public ScreenAffector
{
	public:
		explicit FadeAffector(double i_alpha, double e_alpha);
		
		virtual void init(ScreenEffect &effect);
		Affector createAffector();

		const double getInit() const { return init_alpha; }
		const double getEnd()  const { return end_alpha; }

	private:
		double init_alpha;
		double end_alpha;	
};

