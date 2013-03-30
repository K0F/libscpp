/*
 *  Server.h
 *  SC++
 *
 *  Created by Chad McKinney on 2/17/12.
 *
 */
#pragma once
/* The classes below are exported */
#pragma GCC visibility push(default)

#include <set>
#include <map>
//#include "boost/unordered_map.hpp"
#include <vector>
#ifdef __APPLE__
#include <CoreServices/CoreServices.h>
#endif

#include "sc.h"
#include "NetAddr.h"
#include "SC_WorldOptions.h"
#include "Engine.h"
#include "SystemActions.h"
#include "Bus.h"
#include "Buffer.h"
#include "InternalSynthServer.h"

namespace sc {
	
	class Group;
	class RootNode;
	class Synth;
	
	struct ServerOptions : WorldOptions {

		ServerOptions();
		
		uint32 firstPrivateBus();
		uint32 numBuffers();
		void setNumBuffers(uint32 _numBuffers);
		uint32 numAudioBusChannels();
		uint32 numControlBusChannels();
		void setPortNum(uint32 _portNum);
		uint32 getPortNum();
		
		const char* protocol;
		uint32 mNumPrivateAudioBusChannels;
		uint32 maxSynthDefs;
		
		bool intputStreamsEnabled, outputStreamsEnabled;
		uint32 initialNodeID;
		bool remoteControlVolume, memoryLocking;
		//uint32 bufferSize;
		uint32 portNum;
		uint32 blockSize;
		const char* pluginPath;
		const char* synthDefPath;
		
	private:
		
		char userPath[PATH_MAX];
	};
	
	//////////////////////
	// ServerShmInterface
	//////////////////////
	
	// Not Implemented!
	/*
	class ServerShmInterface {
	
	public:
		
		ServerShmInterface(uint32 port);
		
		ServerShmInterface* copy(); // never ever copy! will cause duplicate calls to the finalizer!
		
		void connect();
		void disconnect();
		void getControlBusValue();
		void getControlBusValues();
		void setControlBusValue();
		void setControlBusValues();
		
	private:
		// ptr, finalizer // WHAT ARE THESE??
	};*/
	
	////////////
	// Server
	///////////
	
	class Server {
		
	public:
		
		Server(ServerOptions _options);
		Server(const char* _name = "internal", NetAddr _add = NetAddr(), ServerOptions _options = ServerOptions(), 
			   uint16 _clientID = 0);
			
	private:
		
		void init();
		
	public:
		
		~Server();
		
		//static std::set<uint16> all(); I DON'T KNOW WHAT THESE TYPES ARE YET
		//static setAll(std::set<uint16> dict);
		
		void newAllocators();
		void newNodeAllocators();
		void newBusAllocators();
		void newBufferAllocators();
		void newScopeBufferAllocators();
		uint16 nextNodeID();
		uint16 nextPermNodeID();
		void freePermNodeID(uint16 ID);
		
		static void initClass();
		static Server* fromName(const char* _name);
		
		// bundling support added
		void sendMsg(big_scpacket* packet);
		
		/*
		void sendRawArray(TYPE? rawArray);
		void sendMsgSync
		void sync
		void schedSync
		void listSendMsg
		void listSendBundle
		*/
		
		void sendSynthDef(const char* synthName, const char* dir);
		void loadSynthDef(const char* synthName, int completionMsg, const char* dir);
		void loadDirectory(const char* dir, int completionMsg = 0);
		
		/*
		serverRunning_
		wait
		waitForBoot
		doWhenBooted
		bootSync
		ping
		addStatusWatcher
		cachedBufferDo
		cachedBufferAt		
		*/
		
		Bus inputBus();
		Bus outputBus();
		
		/*
		startAliveThread
		stopAliveThread
		aliveThreadIsRunning
		resumeThreads
		*/
		
		typedef bool (*BootFailHandler)();
        // Removed bool startAliveThread, bool recover, Server::BootFailHandler* onFail,
        void boot(server_reply_func_t callback = nullCallback);
        void bootServerApp(server_reply_func_t callback);
		void reboot();
		void status();
		void setNotify(bool flag = true);
		void sendNotifyRequest(bool flag = true);
		// dumpOSC  This is broken in SC anyways...
		void quit();
		static void quitAll();
		static void killAll();
		void freeAll();
		static void staticFreeAll();
		static void hardFreeAll();
		static std::vector<Server*> allRunningServers();
		
		/*
		openBundle
		closeBundle
		makeBundle
		bind
		*/
		
		bool bootInProcess();
		bool quitInProcess();
		
		/*
		allocSharedControls
		setSharedControls
		getSharedControls
		*/
		
		void record(const char* path = NULL);
		void pauseRecording();
		void stopRecording();
		void prepareForRecord(const char* path = NULL);
		
		void cmdPeriod();
		void doOnServerTree();
		
		Group* getDefaultGroup();
		void queryAllNodes();
		
		/*
		printOn
		storeOn
		 */
		
		void setVolume(float64 volume);
		void mute();
		void unmute();
		bool hasShmInterface();
		void reorder();
		
		float64 getControlBusValue(uint32 busIndex);
		std::vector<float64> getControlBusValues(uint32 busIndex, uint32 busChannels);
		void setControlBusValue(uint32 busIndex);
		void setControlBusValues(uint32 busIndex, uint32 busChannels);
		
		const char* getName();
		
		NetAddr getAddr();
		void setAddr(NetAddr _addr);
		
		uint16 getClientID();
		bool getInProcess();
		bool getServerRunning();
		bool getServerBooting();
		bool getIsLocal();
		bool getRemoteControlled();
		void setRemoteControlled(bool _remoteControlled);
		
		ServerOptions* getOptions();
		void setOptions(ServerOptions _options);
		
		float64 getLatency();
		void setLatency(float64 _latency);
		
		bool getNotify();
		bool getGetNotified();
		
		NodeIDAllocator* getNodeAllocator();
		ContiguousBlockAllocator* getControlBusAllocator();
		ContiguousBlockAllocator* getAudioBusAllocator();
		ContiguousBlockAllocator* getBufferAllocator();
		StackNumberAllocator* getScopeBufferAllocator();
		uint32 getNumUGens();
		uint32 getNumSynths(); 
		uint32 getNumGroups(); 
		uint32 getNumSynthDefs();
		float64 getAvgCPU();
		float64 getPeakCPU();
		uint64 getSampleRate();
		uint64 getActualSampleRate();
		float64 getAliveThreadPeriod();
		void setAliveThreadPeriod(float64 _aliveThreadPeriod);
		ServerTree getTree();
		void setTree(ServerTree _tree);
		Synth* getRecordNode();
		const char* getRecHeaderFormat();
		void setRecHeaderFormat(const char* _recHeaderFormat);
		const char* getRecSampleFormat();
		void setRecSampleFormat(const char* _recSampleFormat);
        uint32 getRecChannels();
		void setRecChannels(uint32 _recChannels);
		int32 getPid();
		
		bool getScopeBuf(uint32 index, SndBuf* buf);
		
		
		static std::map<const char*, Server*> named;
		static std::set<Server*> serverSet;
		static Server* internal;
		static Group* defaultGroup;
		static bool sync_s;
        static const char* program;
		
		uint32 getPortNum();
		void setPortNum(uint32 _portNum);
		
	private:
		
		void initTree();
		
		const char* name;
		NetAddr addr;
		uint16 clientID;
		bool isLocal, inProcess, serverRunning, serverBooting, bootNotifyFirst, remoteControlled;
		ServerOptions options;
		float64 latency;
		uint16 dumpMode;
		bool notify, notified;
		
		NodeIDAllocator* nodeAllocator;
		ContiguousBlockAllocator* controlBusAllocator; 
		ContiguousBlockAllocator* audioBusAllocator;
		ContiguousBlockAllocator* bufferAllocator;
		StackNumberAllocator* scopeBufferAllocator;
		
		uint32 numUGens, numSynths, numGroups, numSynthDefs;
		float64 avgCPU, peakCPU;
		uint64 sampleRate, actualSampleRate;
		bool alive, booting;
		float64 aliveThreadPeriod;
		ServerTree tree;
		int32 pid;
		InternalSynthServer* scsynth;
		Group* mDefaultGroup;
		RootNode* mRootNode;
		
		// Recording Vars
		Buffer* recordBuf; 
		Synth* recordNode;
		const char* recHeaderFormat;
		const char* recSampleFormat;
		uint32 recChannels;
	};
	
	// Server Commands
	enum Command {
		none = 0,
		
		notify = 1,
		status = 2,
		quit = 3,
		cmd = 4,
		
		d_recv = 5,
		d_load = 6,
		d_loadDir = 7,
		d_freeAll = 8,
		
		s_new = 9,
		
		n_trace = 10,
		n_free = 11,
		n_run = 12,
		n_cmd = 13,
		n_map = 14,
		n_set = 15,
		n_setn = 16,
		n_fill = 17,
		n_before = 18,
		n_after = 19,
		
		u_cmd = 20,
		
		g_new = 21,
		g_head = 22,
		g_tail = 23,
		g_freeAll = 24,
		
		c_set = 25,
		c_setn = 26,
		c_fill = 27,
		
		b_alloc = 28,
		b_allocRead = 29,
		b_read = 30,
		b_write = 31,
		b_free = 32,
		b_close = 33,
		b_zero = 34,
		b_set = 35,
		b_setn = 36,
		b_fill = 37,
		b_gen = 38,
		
		dumpOSC = 39,
		
		c_get = 40,
		c_getn = 41,
		b_get = 42,
		b_getn = 43,
		s_get = 44,
		s_getn = 45,
		
		n_query = 46,
		b_query = 47,
		
		n_mapn = 48,
		s_noid = 49,
		
		g_deepFree = 50,
		clearSched = 51,
		
		sync = 52,
		
		d_free = 53,
		
		b_allocReadChannel = 54,
		b_readChannel = 55,
		
		g_dumpTree = 56,
		g_queryTree = 57,
		
		
		error = 58,
		
		s_newargs = 59,
		
		n_mapa = 60,
		n_mapan = 61,
		n_order = 62,
		
		NUMBER_OF_COMMANDS = 63
	};
	
};

#pragma GCC visibility pop
