/*
 *  Quant.cpp
 *  SC++
 *
 *  Created by Chad McKinney on 3/5/12.
 *
 */

#include "Quant.h"
#include "Clock.h"

namespace sc {
	
	Quant* Quant::defaultQuant = NULL;
	
	Quant* Quant::getDefault()
	{
		//if(Quant::defaultQuant == NULL)
		//	Quant::defaultQuant = new Quant();
		return Quant::defaultQuant;
	}
	
	void Quant::setDefault(Quant* _default)
	{
		Quant::defaultQuant = _default->asQuant();
	}
	
	Quant::Quant(float64 _quant, float64 _phase, float64 _timingOffset) :
		quant(_quant),
		phase(_phase),
		timingOffset(_timingOffset)
	{
		
	}
	
	float64 Quant::nextTimeOnGrid(TempoClock& clock)
	{
		return clock.nextTimeOnGrid(quant, phase - timingOffset);
	}
	
	Quant* Quant::asQuant()
	{
		return new Quant(*this);
	}
};
