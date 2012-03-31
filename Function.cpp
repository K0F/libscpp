/*
 *  Function.cpp
 *  SC++
 *
 *  Created by Chad McKinney on 2/29/12.
 *
 */

#include "sc.h"
#include "Function.h"

namespace sc {
	
	Function::Function(ScFunction2 _def) :
		def(_def)
	{
		
	}
	
	// Here is the template for how to create a correct function with return
	// Evaluate the function with the given arguments
	boost::any Function::value(int numArgs, ...)
	{
		boost::any returnValue;
		va_list vl;
		va_start(vl, numArgs);
		returnValue = def(numArgs, vl);
		va_end(vl);
		//vaList = vl;
		return returnValue;
	}
	
	ScFunction2 Function::getDef()
	{
		return def;
	}
	
};

