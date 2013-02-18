/*
 *  Platform.h
 *  SC++
 *
 *  Created by Chad McKinney on 2/17/12.
 *
 */
#pragma once

/* The classes below are exported */
#pragma GCC visibility push(default)

#include <string>

namespace sc {
	
	enum OperatingSystem {
		PLATFORM,
		UNIX,
        OSX,
        LINUX
	};
	
	class Platform {
		
	public:
		
		Platform();
		
		virtual void initPlatform();
		virtual OperatingSystem name() = 0;
		virtual void recompile() = 0;
		
		const char* getClassLibraryDir();
		void setClassLibraryDir(const char* _dir);
		const char* getRecordingsDir();
		void setRecordingsDir(const char* _dir);
		
		virtual void startup() = 0;
		virtual void shutdown() = 0;
		
		static OperatingSystem os;
		
	protected:
		
		std::string classLibraryDir;
		std::string recordingsDir;
	};
	
	class UnixPlatform : public Platform {
		
	public:
		
		UnixPlatform();
		
		virtual void initPlatform();
		virtual OperatingSystem name();
		virtual void recompile();
		
		const char* getClassLibraryDir();
		void setClassLibraryDir(const char* _dir);
		const char* getRecordingsDir();
		void setRecordingsDir(const char* _dir);
		
		virtual void startup();
		virtual void shutdown();
		
		const static OperatingSystem os = UNIX;
	};
	
};

#pragma GCC visibility pop
