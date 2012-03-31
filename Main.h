/*
 *  Main.h
 *  SC++
 *
 *  Created by Chad McKinney on 2/17/12.
 *
 */
#pragma once

/* The classes below are exported */
#pragma GCC visibility push(default)

#include <vector>
#include "Platform.h"
#include "Kernel.h"

namespace sc {

	class Main : public Process {
		
	public:
		
		//Main();
		//~Main();
		
		static void startup();
		static void shutdown();
		static void run();
		static void stop();
		static void hardstop();
		static void recompile();
		
		static int32 pid();
		
		static Platform* getPlatform();
		
	//private:
		
		static Platform* platform;
	};

};

#pragma GCC visibility pop