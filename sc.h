/*
 *  SC++.h
 *  SC++
 *
 *  Created by Chad McKinney on 2/16/12.
 *
 */

#pragma once

/* The classes below are exported */
#pragma GCC visibility push(default)


//#define NULL 0;
#include "SC_Types.h"
#include <list>
#include "Main.h"

// SuperCollider namespace
namespace sc {
	
	/////////////////////////////////////////////
	// REMEMBER TO CALL startup() AND shutdown()
	/////////////////////////////////////////////
	
	//Main thisProcess;
	
	inline void startup()
	{
		Main::startup();
		//std::cout << "SuperCollider Class Path" << Main::getPlatform()->getClassLibraryDir() << std::endl;
		//std::cout << "SuperCollider Recording Path" << Main::getPlatform()->getRecordingsDir() << std::endl;
	}
	
	inline void shutdown()
	{
		Main::shutdown();
	}
	
	inline const char* recordingsDir()
	{
		//const char* path = Main::platform->getRecordingsDir();
		//std::cout << "recordingsDir() From sc.h: " << path << std::endl;
		return Main::getPlatform()->getRecordingsDir();
	}
	
	///////////////////////////////
	// Conveniance Functions/Types
	//////////////////////////////
	
	
	// Struct for holding arg name/val pairs
	struct arg_pair {
		const char* arg_name;
		float64 arg_val;
		
		arg_pair(const char* _arg_name, float64 _arg_val) : arg_name(_arg_name), arg_val(_arg_val){}
	};
	
	// Typefef for argument pair lists
	typedef std::list<arg_pair> node_arg_list;
	
	inline int binaryValue(bool flag)
	{
		if(flag)
			return 1;
		else
			return 0;
	}
	
	// NODE ADD ACTION
	enum AddAction {
		addToHead = 0,
		addToTail = 1,
		addBefore = 2,
		addAfter = 3,
		addReplace = 4
	};
	
	/* For classes that expect an uint index (and does not expect negative valies), but where 0
	may be a useful and necessary value, but needs to be able against NULL. EXAMPLES buffer index alloc*/
	extern const int NEGNULL;
	//extern const int NULL;
};

#pragma GCC visibility pop
