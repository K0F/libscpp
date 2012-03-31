/*
 *  Kernel.cpp
 *  SC++
 *
 *  Created by Chad McKinney on 2/17/12.
 *
 */

#include "PyrSched.h"
#include "Kernel.h"

namespace sc {

	////////////
	// Process
	///////////
	/*
	Process::Process()
	{
		
	}*/
	
	void Process::startup()
	{
		float64 time;
		time = Process::elapsedTime();
	}
	
	void Process::run()
	{
		// This method is called when 'Run Main' is chosen from the menu.
		// Override in class 'Main' to do whatever you want.
	}
	
	void Process::stop()
	{
		// This method is called when 'Stop Main' is chosen from the menu.
		// Override in class 'Main' to do whatever you want.
	}
	
	void Process::shutdown()
	{
		// This method is called before recompiling or quitting.
		// Override in class 'Main' to do whatever you want.
	}
	
	double Process::elapsedTime()
	{
		return elapsedTime();
	}
	
	///////////////
	// Interpreter
	//////////////
	
	Interpreter::Interpreter()
	{
		
	}
	
	void Interpreter::setCmdLine(const char* _line)
	{
		this->cmdLine = _line;
	}
	
	const char* Interpreter::getCmdLine()
	{
		return this->cmdLine;
	}
	
}