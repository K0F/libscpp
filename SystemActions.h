/*
 *  SystemActions.h
 *  SC++
 *
 *  Created by Chad McKinney on 2/19/12.
 *
 */

#pragma once

/* The classes below are exported */
#pragma GCC visibility push(default)

#include "sc.h"

namespace sc {

	class AbstractSystemAction {

	public:
		
		static void init();
	};
	
	class AbstractServerAction : public AbstractSystemAction {
		
	public:
		
		static void init();
	};
	
	class ServerTree : public AbstractServerAction {
	
	public:
		
		static void init();
		void** objects;
	};
	
};

#pragma GCC visibility pop