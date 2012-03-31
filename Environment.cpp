/*
 *  Environment.cpp
 *  SC++
 *
 *  Created by Chad McKinney on 2/29/12.
 *
 */


#include "Environment.h"

namespace sc {
	
	Environment* Environment::currentEnvironment = NULL;

	Environment* Environment::staticMake(EnvirMakeFunc function)
	{
		Environment* envir = new Environment();
		envir->make(function);
		return envir;
	}
	
	Environment* Environment::staticUse(EnvirUseFunc function)
	{
		Environment* envir = new Environment();
		envir->use(function);
		return envir;
	}
	
	void Environment::make(EnvirMakeFunc function)
	{
		// pushes the Envir, executes function, returns the Envir
		// usually used to create an environment by adding new variables to it.
		Environment* saveEnvir;
		
		saveEnvir = currentEnvironment;
		currentEnvironment = this;
		
		try {
			function(this);
		}
		catch (...) {
			currentEnvironment = saveEnvir;
		}
	}

	boost::any Environment::use(EnvirUseFunc function)
	{
		boost::any result;
		Environment* saveEnvir;
		
		saveEnvir = currentEnvironment;
		currentEnvironment = this;
		
		try {
			result = function(this);
		}
		catch (...) {
			currentEnvironment = saveEnvir;
		}
		
		return result;
	}
	
	boost::any Environment::eventAt(const char* key)
	{
		return this->at(key);
	}
		
	void Environment::staticPop()
	{
		if(stack.size() > 0)
		{
			currentEnvironment = stack.back();
			stack.pop_back();
		}
	}
	
	void Environment::staticPush(Environment* envir)
	{
		stack.push_back(envir);
		currentEnvironment = envir;
	}
	
	void Environment::pop()
	{
		Environment::staticPop();
	}
	
	void Environment::push()
	{
		Environment::staticPush(this);
	}
};