/*
 *  InternalSynthServer.h
 *  SC++
 *
 *  Created by Chad McKinney on 2/20/12.
 *	Big thanks to Chris Kiefer and Gerard Roma for helpful code.
 */

#pragma once

/* The classes below are exported */
#pragma GCC visibility push(default)

#include "sc.h"
#include "SC_WorldOptions.h"
#include "SC_World.h"

//#include "Resources.h"
#include "OSCMessages.h"

#include <sys/socket.h>
#include <stdio.h>
#include <unistd.h>

#include "SC_CoreAudio.h"
#include "SC_AU.h"
#include "SC_SndBuf.h"

#include <boost/function.hpp>

namespace sc {

	
	extern void localServerReplyFunc(struct ReplyAddress *inReplyAddr, char* inBuf, int inSize);
    typedef boost::function<void ()> server_reply_func_t;
	
	class InternalSynthServer {
	
	public:
		InternalSynthServer(WorldOptions _options = kDefaultWorldOptions, 
						const char* _pluginsPath = NULL, 
						const char* _synthdefsPath = NULL, 
						int _preferredPort = 57110);
		
		~InternalSynthServer();
		
        void boot();
		void quit();
		void sendMsg(big_scpacket* packet);
		bool getScopeBuf(uint32 index, SndBuf* buf);
		
		World* getWorld();
		void setWorldOptions(WorldOptions _options);

#ifdef __APPLE__
		CFStringRef synthName;
#else
        std::string synthName;
#endif
		int portNum;
		
	private:
		
		struct InternalSynthServerGlobals
		{
			int mNumSharedControls;
			float *mSharedControls;
			const char* mPluginsPath;
			const char* mSynthDefsPath;
		};
		
		WorldOptions options;
#ifdef __APPLE__
        CFStringRef pluginsPath, synthdefsPath;
#else
        std::string pluginsPath, synthdefsPath;
#endif
		int preferredPort;
		World* world;
		InternalSynthServerGlobals gInternalSynthServer;
		int findNextFreeUdpPort(int startNum);
	};
	
};

#pragma GCC visibility pop
