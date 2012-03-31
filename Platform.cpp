/*
 *  Platform.cpp
 *  SC++
 *
 *  Created by Chad McKinney on 2/17/12.
 *
 */

#include "Platform.h"

namespace sc {

	//////////////
	// Platform
	//////////////
	
	Platform::Platform()
	{
		
	}
	
	void Platform::initPlatform()
	{
		
	}
	
	const char* Platform::getClassLibraryDir()
	{
		return this->classLibraryDir.c_str();
	}
	
	void Platform::setClassLibraryDir(const char* _dir)
	{
		this->classLibraryDir = _dir;
	}
	
	const char* Platform::getRecordingsDir()
	{
		return this->recordingsDir.c_str();
	}
	void Platform::setRecordingsDir(const char* _dir)
	{
		this->recordingsDir = _dir;
	}
	
	OperatingSystem Platform::name()
	{
		return PLATFORM;
	}
	
	void Platform::recompile()
	{
		
	}
	
	void Platform::startup()
	{
		
	}
	
	void Platform::shutdown()
	{
		
	}
	
	//////////////////
	// Unix Platform
	/////////////////
	
	UnixPlatform::UnixPlatform() : Platform()
	{
		
	}
	
	void UnixPlatform::initPlatform()
	{
		Platform::initPlatform();
	}
	
	const char* UnixPlatform::getClassLibraryDir()
	{
		return Platform::getClassLibraryDir();
	}
	
	void UnixPlatform::setClassLibraryDir(const char* _dir)
	{
		Platform::setClassLibraryDir(_dir);
	}
	
	const char* UnixPlatform::getRecordingsDir()
	{
		return Platform::getRecordingsDir();
	}
	void UnixPlatform::setRecordingsDir(const char* _dir)
	{
		Platform::setRecordingsDir(_dir);
	}
	
	OperatingSystem UnixPlatform::name()
	{
		return UNIX;
	}
	
	void UnixPlatform::recompile()
	{
		Platform::recompile();
	}
	
	void UnixPlatform::startup()
	{
		Platform::startup();
	}
	
	void UnixPlatform::shutdown()
	{
		Platform::shutdown();
	}
}