/*
 *  Quant.h
 *  SC++
 *
 *  Created by Chad McKinney on 3/5/12.
 *
 */

// This class is used to encapsulate quantization issues associated with EventStreamPlayer and TempoClock
// quant and phase determine the starting time of something scheduled by a TempoClock
// timingOffset is an additional timing factor that allows an EventStream to compute "ahead of time" enough to allow
// negative lags for strumming a chord, etc


#pragma once

#include "sc.h"

namespace sc {
	
	class TempoClock;
	
	class Quant {
		
	public:
		
		static Quant* getDefault();
		static void setDefault(Quant* _default);
		
		Quant(float64 _quant = 0, float64 _phase = 0, float64 _timingOffset = 0);
		
		float64 nextTimeOnGrid(TempoClock& clock);
		
		Quant* asQuant();
		
		//printOn
		//storeArgs
		
		static Quant* defaultQuant;
		
		float64 quant, phase, timingOffset;
	};
	
};
