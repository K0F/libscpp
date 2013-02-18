/*
 *  Main.cpp
 *  SC++
 *
 *  Created by Chad McKinney on 2/17/12.
 *
 */
#include  <libgen.h>

#include "Main.h"
#include "Server.h"
#include "Platform.h"
#ifdef __APPLE__
#include "OSXPlatform.h"
#elif __LINUX__
#include "LinuxPlatform.h"
#endif

namespace sc {

	/*
	Main::Main() : Process()
	{
		
	}
	
	Main::~Main()
	{
		delete platform;
	}
	*/
#ifdef __APPLE__
	Platform* Main::platform = new OSXPlatform();
#elif __LINUX__
    Platform* Main::platform = new LinuxPlatform();
#endif

	void Main::startup()
	{
		// setup the platform first so that class initializers can call platform methods.
		// create the platform, then intialize it so that initPlatform can call methods
		// that depend on thisProcess.platform methods.
		
		//platform = new Platform();
		//Main::platform = new OSXPlatform(); // CURRENTLY WE ONLY WORK ON OSX
		//Main::platform->initPlatform();
		
        // Process::startup();
		Main::platform->startup();
	}
	
	void Main::shutdown()
	{
		Server::quitAll();
		Main::platform->shutdown();
		Process::shutdown();
	}
	
	void Main::run()
	{
		
	}
	
	void Main::stop()
	{
		
	}
	
	void Main::hardstop()
	{
		
	}
	
	void Main::recompile()
	{
		platform->recompile();
	}
	
	
	Platform* Main::getPlatform()
	{
		return platform;
	}
	
	int32 Main::pid()
	{
		return getpid();
	}

}
