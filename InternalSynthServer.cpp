/*
 *  InternalSynthServer.cpp
 *  SC++
 *
 *  Created by Chad McKinney on 2/20/12.
 *	Big thanks to Gerard Roma and Chris Kiefer the for helpful code.
 */

#include "InternalSynthServer.h"
#include "SC_WorldOptions.h"
#include <iostream>
#include "SC_Errors.h"

namespace sc {

	InternalSynthServer::InternalSynthServer(WorldOptions _options, const char* _pluginsPath,  
											 const char* _synthdefsPath, int _preferredPort) :
		options(_options),
		preferredPort(_preferredPort)
	{
		const int kNumDefaultSharedControls = 1024;
		float gDefaultSharedControls[kNumDefaultSharedControls];
		//bool gUseDoubles = false;
		world = 0;
		gInternalSynthServer.mNumSharedControls = kNumDefaultSharedControls;
		gInternalSynthServer.mSharedControls = gDefaultSharedControls;
		gInternalSynthServer.mPluginsPath = _pluginsPath;
		gInternalSynthServer.mSynthDefsPath = _synthdefsPath;
		
		pluginsPath = CFStringCreateWithCString(NULL, _pluginsPath, kCFStringEncodingASCII);
		synthdefsPath = CFStringCreateWithCString(NULL, _synthdefsPath, kCFStringEncodingASCII);
	}
	
	InternalSynthServer::~InternalSynthServer()
	{
		
	}
	
	void* scThreadFunc(void* arg);
	
	void* scThreadFunc(void* arg)
	{
		World* world  = (World*)arg;
		World_WaitForQuit(world);
		return 0;
	}
	
	void null_reply_func(struct ReplyAddress* /*addr*/, char* /*msg*/, int /*size*/) {
		
	}
	
	void InternalSynthServer::boot()
	{
		
		if (!world) {
			//SetPrintFunc(&vpost);
			
			
			//options.mNumSharedControls = gInternalSynthServer.mNumSharedControls;
			//options.mSharedControls = gInternalSynthServer.mSharedControls;
			/*
			// internal servers use the PID to identify the shared memory region
#if defined(SC_IPHONE)
			options.mSharedMemoryID = 0;
#elif !defined(_WIN32)
			options.mSharedMemoryID = getpid();
#else
			options.mSharedMemoryID = GetCurrentProcessId();
#endif
			*/
			
			/*
			options = kDefaultWorldOptions;
			options.mPreferredSampleRate=44100;
			options.mBufLength=64;
			options.mPreferredHardwareBufferFrameSize = 512;
			options.mMaxWireBufs=64;
			options.mRealTimeMemorySize=8192;*/
			options.mUGensPluginPath = gInternalSynthServer.mPluginsPath;
			//gInternalSynthServer.mWorld = World_New(&options);
			pthread_t scThread;
			char stringBuffer[PATH_MAX];
			OSCMessages messages;
			
			CFStringGetCString(pluginsPath, stringBuffer, sizeof(stringBuffer), kCFStringEncodingUTF8);
			setenv("SC_PLUGIN_PATH", stringBuffer, 1);
			
			CFStringGetCString(synthdefsPath, stringBuffer, sizeof(stringBuffer), kCFStringEncodingUTF8);
			setenv("SC_SYNTHDEF_PATH", stringBuffer, 1);
			
			this->portNum = findNextFreeUdpPort(preferredPort);
			//this->portNum = preferredPort;
			
			world = World_New(&options);
			world->mVerbosity=true;
			//world->mDumpOSC=2;
			if (world) {
				if (this->portNum >= 0) World_OpenUDP(world, this->portNum);
				pthread_create (&scThread, NULL, scThreadFunc, (void*)world);
			}
			if (world->mRunning){
				small_scpacket packet = messages.initTreeMessage();
				World_SendPacket(world, 16, (char*)packet.buf, null_reply_func);
			}
		}
		
	}
	
	void InternalSynthServer::quit()
	{
		OSCMessages messages;
		if (world && world->mRunning){
			small_scpacket packet = messages.quitMessage();
			World_SendPacket(world, 8,(char*)packet.buf, null_reply_func);
		}
	}
	
	void InternalSynthServer::sendMsg(big_scpacket* packet)
	{
		if(world != NULL)
			World_SendPacket(world, packet->size(), (char*)packet->buf, null_reply_func);
		else
			scprintf("ERROR. InternalSynthServer: world declared null.");
	}
	
	int InternalSynthServer::findNextFreeUdpPort(int startNum)
	{
		int server_socket = -1;
		struct sockaddr_in mBindSockAddr;
		int numberOfTries=50;
		int port = startNum;
		if ((server_socket = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
			scprintf("failed to create udp socket\n");
			return -1;
		}
		
		bzero((char *)&mBindSockAddr, sizeof(mBindSockAddr));
		mBindSockAddr.sin_family = AF_INET;
		mBindSockAddr.sin_addr.s_addr = htonl(INADDR_ANY);
		mBindSockAddr.sin_port = htons(port);
		const int on = 1;
		setsockopt( server_socket, SOL_SOCKET, SO_REUSEADDR, &on, sizeof(on));
		
		while (bind(server_socket, (struct sockaddr *)&mBindSockAddr, sizeof(mBindSockAddr)) < 0) {
			if(--numberOfTries <0 || (errno != EADDRINUSE)) {
				scprintf("unable to bind udp socket\n");
				return -1;
			}
			port++;
			mBindSockAddr.sin_port = htons(port);
		}
		close(server_socket);
		return port;
	}
	
	bool InternalSynthServer::getScopeBuf(uint32 index, SndBuf *buf)
	{
		/*
		bool didChange = false;
		
		if(world)
		{
			int serverErr = World_CopySndBuf(world, index, buf, true, &didChange);
			
			if(serverErr != kSCErr_None)
			{
				if(serverErr == kSCErr_IndexOutOfRange)
				{
					std::cout << "Error: InternalSynthServer World_CopySndBuf failed. Index out of range." 
						<< std::endl;
				}
				
				else
				{
					std::cout << "Error: InternalSynthServer World_CopySndBuf failed." << std::endl;
				}
				
				return false;
			}
		} 
		
		else 
		{
			didChange = false;
		}

		
		*/
		
		world->mNRTLock->Lock();
		/*
		for(int i = 0; i < world->mBufLength; ++i)
		{
			try {
				buf->data[i] = ((float*)world->mAudioBus)[(index * world->mBufLength) + i];
			}
			catch (...) {
				buf->data[i] = 0;
			}
			
		}*/
		
		uint32 bufSize = buf->samples * sizeof(float);
		memcpy(buf->data, world->mAudioBus + (index * world->mBufLength), bufSize);
		
		world->mNRTLock->Unlock();
		
		return true;
	}
	
	World* InternalSynthServer::getWorld()
	{
		return world;
	}
	
	void InternalSynthServer::setWorldOptions(WorldOptions _options)
	{
		options = _options;
	}
}