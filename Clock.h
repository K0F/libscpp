/*
 *  Clock.h
 *  SC++
 *
 *  Created by Chad McKinney on 3/5/12.
 *
 */

#pragma once

#include "sc.h"
#include "Quant.h"

namespace sc {

	class Task;
	
	// Clocks for timing threads
	class Clock {
	
	public:
		
		static void play();
		static float64 second();
		
	};
	
	class TempoClock : public Clock {
	
	public:
		
		void play(Task* task, Quant quant = Quant(1.0));
		
		float64 nextTimeOnGrid(float64 quant = 1, float64 phase = 0);
		
		static TempoClock* defaultClock;
	};
	
};