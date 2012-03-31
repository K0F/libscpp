/*
 *  Function.h
 *  SC++
 *
 *  Created by Chad McKinney on 2/29/12.
 *
 */

#pragma once
#include "AbstractFunction.h"
#include "boost/any.hpp"
#include "boost/function.hpp"
#include <stdarg.h>

namespace sc {

	// an implementation of this is supplied to sc::Function as the initial argument
	//typedef boost::any (*ScFunction2) (int numArgs, va_list vl);
	
	// you can use this directly if you want to avoid sc::Function altogether
	typedef boost::any (*ScFunction) (int numArgs, ...);
	
	//typedef boost::function<boost::any (int numArgs, ...)> ScFunction;
	typedef boost::function<boost::any (int numArgs, va_list)> ScFunction2;
	
	class Function : public AbstractFunction {
		
	public:
		
		// You will need to make your own function and supply it for the argument of the sc::Function 
		static boost::any exampleScFunction(int numArgs, va_list vl)
		{
			boost::any returnValue;
			for(int i = 0; i < numArgs; ++i)
			{
				// Really you need to pick specific types, float, int, and use them accordingly
				returnValue = va_arg(vl, void*);
			}
			return returnValue;
		}
		
		Function(ScFunction2 _def = &Function::exampleScFunction);
		
		// Implemented very different here, so we can allow for any number of arguments of any type
		// It is up to the user to define the behaviour within the function to properly use the va_list
		//Function(int numArgs, ...);
		
		// Evaluate the function with the given arguments
		boost::any value(int numArgs, ...);
		
		ScFunction2 getDef();
		
	private:
		
		va_list vaList;
		ScFunction2 def;
	};

};