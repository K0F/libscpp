/*
 *  Kernel.h
 *  SC++
 *
 *  Created by Chad McKinney on 2/17/12.
 *
 */

#pragma once

/* The classes below are exported */
#pragma GCC visibility push(default)

#include "SC_Types.h"

namespace sc {
	
	class Class {
		
	};
	
	class  Process {
		
	public:
		
		//Process();
		
		static void startup();
		static void run();
		static void stop();
		static void shutdown();
		
		static float64 elapsedTime();
	};
	
	class Interpreter {
		
		// The interpreter defines a context in which interactive commands
		// are compiled.
		
	public:
		
		Interpreter();
		
		void setCmdLine(const char* _line);
		const char* getCmdLine();
		
	private:
		
		const char* cmdLine;
	};	
	
};

#pragma GCC visibility pop