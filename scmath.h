/*
 *  scmath.h
 *  SC++
 *
 *  Created by Chad McKinney on 2/18/12.
 *
 */
#pragma once

/* The classes below are exported */
#pragma GCC visibility push(default)

#include <math.h>
#include <stdlib.h>

#include "sc.h"

namespace sc {
	
	const float32 PI = 3.14159265;
	const float32 TWOPI = 6.28318531;
	
	template<class T>
	inline T mod(T x, T y)
	{
		if(0 == y)
		{
			return x;
		}
		
		return x - y * floor(x / y);
	}
	
	///////////////
	// Wrap Angle
	///////////////
	
	template<class T>
	inline T wrapPosNegPI(T angle)
	{
		return mod(angle + PI, TWOPI) - PI;
	}
	
	template<class T>
	inline T wrapTwoPI(T angle)
	{
		return mod(angle, TWOPI);
	}
	
	template<class T>
	inline T wrap360(T angle)
	{
		return mod(angle, 360);
	}
	
	/////////
	// Wrap
	/////////
	
	template<class T>
	inline T wrap2(T val, T outMax)
	{
		T outVal;
		
		if(val > outMax)
			outVal = -outMax + val;
		else if(val < -outMax)
			outVal = outMax - val;
		else
			outVal = val;
		
		if(outVal > outMax || outVal < -outMax)
			return wrap2(outVal);
		else
			return outVal;

	}
	
	inline int32 wrapInt(int32 val, int32 lo, int32 hi)
	{
		return ((val - lo) % (hi - lo +1)) + lo;
	}
	
	/////////
	// Range
	/////////
	
	// Map a number from a linear range to an exponential range
	template<class T>
	inline T linexp(T val, T inMin, T inMax, T outMin, T outMax)
	{
		if(val <= outMin)
			return val;
		else if(val >= outMax)
			return val;
		else
			return pow(outMax / outMin, (val - inMin) / (inMax - inMin)) * outMin;
	}
	
	//////////
	// Random
	//////////
	
	template<class T>
	inline T randomRange(T minValue, T maxValue)
	{
        return minValue + (rand() % (T)(maxValue - minValue + 1));
	}
}

#pragma GCC visibility pop
