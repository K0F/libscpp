/*
 *  Server.cpp
 *  SC++
 *
 *  Created by Chad McKinney on 2/17/12.
 *
 */
#include <stdexcept>
#include <iostream>
#include <unistd.h>

#include "Server.h"
#include "Node.h"
#include "scsynthsend.h"

#define GetCurrentDir getcwd

namespace sc {
	
	/////////////////
	// ServerOptions
	////////////////
	ServerOptions::ServerOptions() :
		WorldOptions(kDefaultWorldOptions)
	{
		//WorldOptions(kDefaultWorldOptions)
		mPassword = NULL;
		mMaxLogins = 64;
		mMaxNodes = 1024;
		mNumInputBusChannels = 8;
		mNumOutputBusChannels = 8;
		mNumBuffers = 1026;
		maxSynthDefs = 1024;
		protocol = "udp";
		mBufLength = 64;
		mNumRGens = 64;
		mMaxWireBufs = 64;
		mPreferredSampleRate = 44100;
		mLoadGraphDefs = true;
		mVerbosity = 0;
		mRendezvous = false;
		//mRestrictedPath = "~/";
		initialNodeID = 1000;
		remoteControlVolume = false;
		memoryLocking = false;
		mPreferredHardwareBufferFrameSize = 512;
		mRealTimeMemorySize = 81920; // Increased
		initialNodeID = 1000;
		blockSize = 512;
		portNum = 57110;
		mNumPrivateAudioBusChannels = 112;
		
		// Generate default paths
		FSRef ref;
		OSType folderType = kApplicationSupportFolderType;
		
		FSFindFolder(kUserDomain, folderType, kCreateFolder, &ref);
		FSRefMakePath(&ref, (UInt8*)&userPath, PATH_MAX);
		
		const char* synthdefs = "/SuperCollider/synthdefs";
		strcat(userPath, synthdefs);
		synthDefPath = userPath;
		pluginPath = "/Applications/SuperCollider/SuperCollider.app/Contents/Resources/plugins";
	}
	
	uint32 ServerOptions::firstPrivateBus()
	{
		return mNumOutputBusChannels + mNumInputBusChannels;
	}
	
	uint32 ServerOptions::numBuffers()
	{
		return mNumBuffers - 2;
	}
	
	void ServerOptions::setNumBuffers(uint32 _numBuffers)
	{
		mNumBuffers = _numBuffers + 2;
	}
	
	uint32 ServerOptions::numAudioBusChannels()
	{
		return mNumPrivateAudioBusChannels + mNumInputBusChannels + mNumOutputBusChannels;
	}
	
	uint32 ServerOptions::numControlBusChannels()
	{
		return mNumControlBusChannels;
	}
	
	void ServerOptions::setPortNum(uint32 _portNum)
	{
		portNum = _portNum;
	}
	
	uint32 ServerOptions::getPortNum()
	{
		return portNum;
	}
	
	///////////
	// Server
	//////////
	
	bool Server::sync_s = true;
	std::set<Server*> Server::serverSet = std::set<Server*>();
	std::map<const char*, Server*> Server::named = std::map<const char*, Server*>();
	Server* Server::internal = NULL;
	Group* Server::defaultGroup = NULL;
	
	Server::Server(ServerOptions _options) :
		name("internal"),
		clientID(0),
		serverRunning(false),
		serverBooting(false),
		bootNotifyFirst(false),
		options(_options),
		latency(0.2),
		dumpMode(0),
		notify(true),
		notified(false),
		numUGens(0),
		numSynths(0),
		numGroups(0),
		numSynthDefs(0),
		alive(false),
		booting(false),
		aliveThreadPeriod(0.7),
		nodeAllocator(NULL),
		controlBusAllocator(NULL),
		audioBusAllocator(NULL),
		mDefaultGroup(NULL),
		mRootNode(NULL),
		recordBuf(NULL),
		recordNode(NULL),
		recHeaderFormat("aiff"),
		recSampleFormat("float"),
		recChannels(2)
	{
		init();
	}
	
	Server::Server(const char* _name, NetAddr _addr, ServerOptions _options, uint16 _clientID) :
		name(_name),
		addr(_addr),
		clientID(_clientID),
		serverRunning(false),
		serverBooting(false),
		bootNotifyFirst(false),
		options(_options),
		latency(0.2),
		dumpMode(0),
		notify(true),
		notified(false),
		numUGens(0),
		numSynths(0),
		numGroups(0),
		numSynthDefs(0),
		alive(false),
		booting(false),
		aliveThreadPeriod(0.7),
		nodeAllocator(NULL),
		controlBusAllocator(NULL),
		audioBusAllocator(NULL),
		mDefaultGroup(NULL),
		mRootNode(NULL),
		recordBuf(NULL),
		recordNode(NULL),
		recHeaderFormat("aiff"),
		recSampleFormat("float"),
		recChannels(2)
	{
		init();
	}
	
	void Server::init()
	{
		std::cout << "Server created with plugin path: " << options.pluginPath <<std::endl
		<< "Server created with synth def path: " << options.synthDefPath <<std::endl;
		
		sampleRate = options.mPreferredSampleRate;
		scsynth = new InternalSynthServer(options, options.pluginPath, options.synthDefPath, options.portNum);
		//inProcess = addr.getAddr() == 0;
		inProcess = true; // We're making an internal server
		isLocal = false;
		remoteControlled = isLocal;
		named[name] = this;
		serverSet.insert(this);
		if(Server::internal == NULL)
			Server::internal = this;
		newAllocators();
		// Server.changed(\serverAdded, this); Not implemented
		//volume Not Implemented Yet
	}
	
	Server::~Server()
	{
		
		if(nodeAllocator != NULL)
		{
			nodeAllocator->reset();
			delete nodeAllocator;
			nodeAllocator = NULL;
		}
		
		if(controlBusAllocator != NULL)
		{
			delete controlBusAllocator;
			controlBusAllocator = NULL;
		}
		
		if(audioBusAllocator != NULL)
		{
			delete audioBusAllocator;
			audioBusAllocator = NULL;
		}
		
		if(bufferAllocator != NULL)
		{
			delete bufferAllocator;
			bufferAllocator = NULL;
		}
		
		if(mDefaultGroup != NULL)
		{
			delete mDefaultGroup;
			mDefaultGroup = NULL;
		}
		
		if(mRootNode != NULL)
		{
			delete mRootNode;
			mRootNode = NULL;
		}
		
		if(recordBuf != NULL)
		{
			delete recordBuf;
			recordBuf = NULL;
		}
		
		if(recordNode != NULL)
		{
			delete recordNode;
			recordNode = NULL;
		}
		
		if(scsynth != NULL)
		{
			delete scsynth;
			scsynth = NULL;
		}
	}
	
	void Server::initTree()
	{
		newNodeAllocators();
		//sendMsg(g_new, 1, 0, 0);
		this->mRootNode = new RootNode(this);
		this->mDefaultGroup = new Group(mRootNode, addToHead, 1);
		//sendMsg("/g_new", 1, 0, 0); We Will Create the root group directly ourselves
		//tree.value(this); ...???!?
		//ServerTree::run(this); Not Implemented Yet
	}
	
	void Server::newAllocators()
	{
		newNodeAllocators();
		newBusAllocators();
		newBufferAllocators();
		newScopeBufferAllocators();
		// NotificationCenter.notify(this, \newAllocators); Not Implemented Yet
	}
	
	void Server::newNodeAllocators()
	{
		if(nodeAllocator != NULL)
		{
			nodeAllocator->reset();
			delete nodeAllocator;
		}
		
		nodeAllocator = new NodeIDAllocator(clientID, options.initialNodeID);
	}
	
	void Server::newBusAllocators()
	{
		controlBusAllocator = new ContiguousBlockAllocator(options.numControlBusChannels());
		audioBusAllocator = new ContiguousBlockAllocator(options.numAudioBusChannels(), options.firstPrivateBus());
	}
	
	void Server::newBufferAllocators()
	{
		bufferAllocator = new ContiguousBlockAllocator(options.numBuffers());
	}
	
	void Server::newScopeBufferAllocators()
	{
		scopeBufferAllocator = new StackNumberAllocator(0, 127);
	}
	
	uint16 Server::nextNodeID()
	{
		return nodeAllocator->alloc();
	}
	
	uint16 Server::nextPermNodeID()
	{
		return nodeAllocator->allocPerm();
	}
	
	void Server::freePermNodeID(uint16 ID)
	{
		nodeAllocator->freePerm(ID);
	}
	
	
	void Server::initClass()
	{
		Server::named = std::map<const char*, Server*>();
		Server::serverSet = std::set<Server*>();
		Server::internal = NULL;
		// Not implementing this right now. We don't need a local/internal dichotomy, everything is internal,
		// and there is no SuperCollider coding environment proper.
		//default = local = Server.new(\localhost, NetAddr("127.0.0.1", 57110));
		//internal = Server.new(\internal, NetAddr.new);
	}
	
	Server* Server::fromName(const char* _name)
	{
		std::map<const char*, Server*>::iterator nameIter = Server::named.find(_name);
		if(nameIter == Server::named.end())
		{
			Server::named[_name] = new Server(_name, NetAddr("127.0.0.1", 57110), ServerOptions(), 0);
		}
		
		return Server::named[_name];
	}
	
	void Server::sendMsg(big_scpacket* packet)
	{
		if(scsynth != NULL)
			scsynth->sendMsg(packet);
		else
			scprintf("ERROR. Server: scsynth declared null.");
	}
	
	void Server::sendSynthDef(const char* synthName, const char* dir)
	{
		
	}
	
	void Server::loadSynthDef(const char* synthName, int completionMsg, const char* dir)
	{

	}
	
	void Server::loadDirectory(const char* dir, int completionMsg)
	{
		big_scpacket packet;
		packet.BeginMsg();
		
		packet.addi(d_loadDir);
		packet.maketags(2);
		packet.addtag(',');
		packet.addtag('s');
		packet.adds(dir);
		
		packet.EndMsg();
		sendMsg(&packet);
	}
	
	void Server::boot(bool startAliveThread, bool recover, Server::BootFailHandler* onFail)
	{
		// serverBooting = true;
		if(!remoteControlled)
		{
			bootServerApp();
		}
	}
	
	void Server::bootServerApp()
	{
		scsynth->boot();
		initTree();
	}
	
	void Server::reboot()
	{
		
	}
	
	void Server::status()
	{
		
	}
	
	void Server::setNotify(bool flag)
	{
		
	}
	
	void Server::sendNotifyRequest(bool flag)
	{
		
	}
	
	// dumpOSC  This is broken in SC anyways...
	void Server::quit()
	{
		scsynth->quit();
	}
	
	void Server::quitAll()
	{
		
	}
	
	void Server::killAll()
	{
		
	}
	
	void Server::freeAll()
	{
		
	}
	
	void Server::staticFreeAll()
	{
		
	}
	
	void Server::hardFreeAll()
	{
		
	}
	
	std::vector<Server*> Server::allRunningServers()
	{
		
	}
	
	const char* Server::getName()
	{
		return this->name;
	}
	
	Group* Server::getDefaultGroup()
	{
		return this->mDefaultGroup;
	}
	
	NetAddr Server::getAddr()
	{
		return this->addr;
	}
	
	void Server::setAddr(NetAddr _addr)
	{
		this->addr = _addr;
	}
	
	
	uint16 Server::getClientID()
	{
		return this->clientID;
	}
	
	bool Server::getInProcess()
	{
		return this->inProcess;
	}
	
	bool Server::getServerRunning()
	{
		return this->serverRunning;
	}
	
	bool Server::getServerBooting()
	{
		return this->serverBooting;
	}
	
	bool Server::getIsLocal()
	{
		return this->isLocal;
	}
	
	bool Server::getRemoteControlled()
	{
		return this->remoteControlled;
	}
	
	void Server::setRemoteControlled(bool _remoteControlled)
	{
		this->remoteControlled = _remoteControlled;
	}
	
	ServerOptions* Server::getOptions()
	{
		return &options;
	}
	
	void Server::setOptions(ServerOptions _options)
	{
		this->options = _options;
	}
	
	
	float64 Server::getLatency()
	{
		return this->latency;
	}
	
	void Server::setLatency(float64 _latency)
	{
		this->latency = _latency;
	}
	
	
	bool Server::getNotify()
	{
		return this->notify;
	}
	
	bool Server::getGetNotified()
	{
		return this->notified;
	}
	
	
	NodeIDAllocator* Server::getNodeAllocator()
	{
		return this->nodeAllocator;
	}
	
	ContiguousBlockAllocator* Server::getControlBusAllocator()
	{
		return this->controlBusAllocator;
	}
	
	ContiguousBlockAllocator* Server::getAudioBusAllocator()
	{
		return this->audioBusAllocator;
	}
	
	ContiguousBlockAllocator* Server::getBufferAllocator()
	{
		return this->bufferAllocator;
	}
	
	StackNumberAllocator* Server::getScopeBufferAllocator()
	{
		return this->scopeBufferAllocator;
	}
	
	uint32 Server::getNumUGens()
	{
		return this->numUGens;
	}
	
	uint32 Server::getNumSynths()
	{
		return this->numSynths;
	}
	
	uint32 Server::getNumGroups()
	{
		return this->numGroups;
	}
	
	uint32 Server::getNumSynthDefs()
	{
		return this->numSynthDefs;
	}
	
	float64 Server::getAvgCPU()
	{
		return this->avgCPU;
	}
	
	float64 Server::getPeakCPU()
	{
		return this->peakCPU;
	}
	
	uint64 Server::getSampleRate()
	{
		return this->sampleRate;
	}
	
	uint64 Server::getActualSampleRate()
	{
		return this->actualSampleRate;
	}
	
	float64 Server::getAliveThreadPeriod()
	{
		return this->aliveThreadPeriod;
	}
	
	void Server::setAliveThreadPeriod(float64 _aliveThreadPeriod)
	{
		this->aliveThreadPeriod = _aliveThreadPeriod;
	}
	
	ServerTree Server::getTree()
	{
		return this->tree;
	}
	
	void Server::setTree(ServerTree _tree)
	{
		this->tree = _tree;
	}
	
	Synth* Server::getRecordNode()
	{
		return recordNode;
	}
	
	const char* Server::getRecHeaderFormat()
	{
		return recHeaderFormat;
	}
	
	void Server::setRecHeaderFormat(const char* _recHeaderFormat)
	{
		recHeaderFormat = _recHeaderFormat;
	}
	
	const char* Server::getRecSampleFormat()
	{
		return recSampleFormat;
	}
	
	void Server::setRecSampleFormat(const char* _recSampleFormat)
	{
		recSampleFormat = _recSampleFormat;
	}
	
	uint32 Server::getRecChannels()
	{
		return recChannels;
	}
	
	void Server::setRecChannels(uint32 _recChannels)
	{
		recChannels = _recChannels;
	}
	
	int32 Server::getPid()
	{
		return this->pid;
	}
	
	bool Server::getScopeBuf(uint32 index, SndBuf* buf)
	{
		return scsynth->getScopeBuf(index, buf);
	}
	
	void Server::record(const char* path)
	{
		if(recordBuf == NULL)
		{
			prepareForRecord(path);
			/*
			sync NOT IMPLEMENTED
			Routine({
				this.sync;
				this.record;
			}).play;
			*/
		}
		
		if(recordNode == NULL)
		{
			recordNode = Synth::tail(mRootNode, "libsc++-server-record", ARGS("bufnum", (double) recordBuf->getBufNum()));
			
		}
		
		else
		{
			recordNode->run(true);
		}
		
		std::cout << "Recording: " << recordBuf->getPath() << std::endl;
	}
	
	void Server::pauseRecording()
	{
		if(recordNode != NULL)
		{
			recordNode->run(false);
			std::cout << "Paused." << std::endl;
		}
		
		else 
		{
			std::cout << "Warning: Not Recording." << std::endl;
		}

	}
	
	void Server::stopRecording()
	{
		if(recordNode != NULL)
		{
			recordNode->free();
			delete recordNode;
			recordNode = NULL;
			
			big_scpacket* msg = recordBuf->freeMsg();
			recordBuf->close(msg);
			delete msg;
			msg = NULL;
			
			std::cout << "Recording Stopped:" << recordBuf->getPath() << std::endl;
			delete recordBuf;
			recordBuf = NULL;
		}
		
		else 
		{
			std::cout << "Warning: Not Recording." << std::endl;
		}
	}
	
	void Server::prepareForRecord(const char* path)
	{
		if(path == NULL)
		{
			/*
			// NOT IMPLEMENTED
			if(File.exists(thisProcess.platform.recordingsDir).not) {
				thisProcess.platform.recordingsDir.mkdir
			};
			*/
			
			//path = thisProcess.getPlatform()->getRecordingsDir();
			
		}
		
		//big_scpacket* recordMsg = Buffer::staticWriteMsg(this->options.numBuffers() + 1, 
		//												 path, recHeaderFormat, recSampleFormat, 0, 0, true);
		
		recordBuf = Buffer::alloc(this, 65536, recChannels, 0, this->options.numBuffers() + 1);
		
		recordBuf->write(path, recHeaderFormat, recSampleFormat, 0, 0, true);
		//recordBuf->setPath(path);
		
		
		/* NOT IMPLEMENTED
		 SynthDef("libsc++-server-record", { arg bufnum;
		 DiskOut.ar(bufnum, In.ar(0, recChannels))
		 }).send(this);
		 */
		
	}
	
	uint32 Server::getPortNum()
	{
		return this->options.portNum;
	}
	
	void Server::setPortNum(uint32 _portNum)
	{
		if(serverRunning || serverBooting)
		{
			std::cout << "Warning: You can't change the port of a running server." << std::endl;
		}
		
		else
		{
			this->options.portNum = _portNum;
			scsynth->portNum = _portNum;
		}
	}
}