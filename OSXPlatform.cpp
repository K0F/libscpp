/*
 *  OSXPlatform.cpp
 *  SC++
 *
 *  Created by Chad McKinney on 2/17/12.
 *
 */


#ifdef __APPLE__
#include <CoreServices/CoreServices.h>
#include "OSXPlatform.h"

using namespace std;

namespace sc {

	OSXPlatform::OSXPlatform() : UnixPlatform()
	{
		initPlatform();
	}
	
	void OSXPlatform::initPlatform()
	{
		UnixPlatform::initPlatform();
		
		FSRef ref;
		OSType folderType = kApplicationSupportFolderType;
		char path[PATH_MAX];
		char path2[PATH_MAX];
		 
		FSFindFolder(kUserDomain, folderType, kCreateFolder, &ref);
		FSRefMakePath(&ref, (UInt8*)&path, PATH_MAX);
		FSRefMakePath(&ref, (UInt8*)&path2, PATH_MAX);
		 
		string classPath = path;
		classPath.append("/SuperCollider/extensions");
		
		string recordPath = path2;
		size_t found = recordPath.find("/Library");
		recordPath.erase(recordPath.begin() + found, recordPath.end());
		recordPath.append("/Music/SuperCollider Recordings");
		
		classLibraryDir = classPath;
		recordingsDir = recordPath;
	}
	
	OperatingSystem OSXPlatform::name()
	{
		return OSXPlatform::os;
	}
	
	void OSXPlatform::recompile()
	{
		
	}
	void OSXPlatform::startup()
	{
		
	}
	void OSXPlatform::shutdown()
	{
		
	}
	
}

#endif
