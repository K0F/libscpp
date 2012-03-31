/*
 *  OSXPlatform.h
 *  SC++
 *
 *  Created by Chad McKinney on 2/17/12.
 *
 */

#pragma once

/* The classes below are exported */
#pragma GCC visibility push(default)

#include "Platform.h"

namespace sc {
	
	class OSXPlatform : public UnixPlatform {
		
	public:
		OSXPlatform();
		void initPlatform();
		OperatingSystem name();
		void recompile();
		void startup();
		void shutdown();
		
		const static OperatingSystem os = OSX;
		
	private:
		
		
	};
	
};

#pragma GCC visibility pop