/*
 *  Buffer.h
 *  SC++
 *
 *  Created by Chad McKinney on 3/18/12.
 *
 */

#pragma once

#include <vector>
#include <list>
#include "boost/function.hpp"
#include "boost/tuple/tuple.hpp"
#include "scsynthsend.h"
#include "sc.h"

namespace sc {
	
	class Server;
	
	class Buffer {
	
	public:
		
		/////////////////////////////////////////////////
		// COMPLETION MESSAGES NOT IMPLEMENTED CURRENTLY
		/////////////////////////////////////////////////
		
		// Buffer Completion Message typedef and default
		/*
		typedef boost::function<big_scpacket* (Buffer* _buffer, int _bufnum)> CompletionMessage;
		
		inline big_scpacket* defaultCompletionMessage(Buffer* _buffer = NULL, int _bufnum = 0)
		{
			big_scpacket* nullPacket = NULL;
			return nullPacket;
		};*/
		
		typedef boost::function<void (Buffer* _buffer)> Action;
		
        void defaultAction(Buffer* _buffer) {} // Nothing
		
		// typedefs For setting multiple indexes
		typedef std::list< std::pair<int, float64> > set_list;
		typedef std::list< std::pair< int, std::list<float64> > > setn_list;
		typedef std::list< boost::tuple<int, int, float64> > fill_list;
		
		Buffer(Server* _serve, int _numFrames = 0, 
			   int _numChannels = 1, int _bufnum = NEGNULL);
		
		~Buffer();
		
		static Buffer* alloc(Server* _server, int _numFrames, int _numChannels = 1, 
							 big_scpacket* _completionMessage = NULL, int _bufnum = NEGNULL);
		
		static std::vector<Buffer*> allocConsecutive(int _numBufs, Server* _server, int _numFrames, int _numChannels = 1,
										big_scpacket* _completionMessage = NULL, int _bufnum = NEGNULL);
		
		void alloc(big_scpacket* _completionMessage = NULL);
		
		void allocRead(const char* _argpath, int _startFrame, int _numFrames = NEGNULL,
					   big_scpacket* _completionMessage = NULL);
		
		big_scpacket* allocReadMsg(const char* _argpath, int _startFrame, int _numFrames = NEGNULL,
								   big_scpacket* _completionMessage = NULL);
		
		void allocReadChannel(const char* _argpath, int _startFrame, int _numFrames = 0, int _channels = NEGNULL,
							  big_scpacket* _completionMessage = NULL);
		
		big_scpacket* allocReadChannelMsg(const char* _argpath, int _startFrame, int _numFrames = 0, 
										  int _channels = NEGNULL);
		
		big_scpacket* allocMsg(big_scpacket* _completionMessage = NULL);
		//big_scpacket* allocReadMsg
		
		static Buffer* read(Server* _server, const char* _path, int _startFrame = 0, int _numFrames = NEGNULL, 
							Action _action = &Buffer::defaultAction, int _bufnum = NEGNULL);
		
		void read(const char* _path, int _fileStartFrame, int _numFrames = NEGNULL, int _bufStartFrame = 0,
				  bool _leaveOpen = false, Action action = &Buffer::defaultAction);
		
		big_scpacket* readMsg(const char* _path, int _fileStartFrame, int _numFrames = NEGNULL, int _bufStartFrame = 0,
				  bool _leaveOpen = false);
		
		static Buffer* readChannel(Server* _server, const char* _path, int _startFrame = 0, int _numFrames = NEGNULL, 
								   int _channels = NEGNULL, Action _action = &Buffer::defaultAction, 
								   int _bufnum = NEGNULL);
		
		void readChannel(const char* _path, int _fileStartFrame = 0, int _numFrames = NEGNULL, int _bufStartFrame = 0,
						 bool _leaveOpen = false, int _channels = NEGNULL, Action _action = &Buffer::defaultAction);
		
		big_scpacket* readChannelMsg(const char* _path, int _fileStartFrame = 0, int _numFrames = NEGNULL, 
									 int _bufStartFrame = 0, bool _leaveOpen = false, int _channels = NEGNULL);
		
		// NOTE: Wait time NOT implemented currently
		void sendCollection(std::vector<float64>& collection, int startFrame = 0, float64 wait = 0);		
				
		/*
		readNoUpdate
		readMsg
		readChannelMsg
		cueSoundFile
		cueSoundFileMsg
		static Buffer* loadCollection
		*sendCollection
		*loadCollection
		loadCollection
		streamCollection
		loadToFloatArray
		getToFloatArray
		*/
		
		void write(const char* _path = NULL, const char* _headerFormat = "aiff", const char* _sampleFormat = "int24", 
				   int _numFrames = NEGNULL, int _startFrame = 0, bool _leaveOpen = false, 
				   big_scpacket* _completionMessage = NULL);
		
		big_scpacket* writeMsg(const char* _path, const char* headerFormat, const char* sampleFormat, 
							   uint32 numFrames, uint32 startFrame, uint32 leaveOpen, 
							   big_scpacket* _completionMessage = NULL);
		
		static big_scpacket* staticWriteMsg(uint32 bufnum, const char* _path, const char* headerFormat, 
											const char* sampleFormat, uint32 numFrames, uint32 startFrame, 
											uint32 leaveOpen, big_scpacket* _completionMessage = NULL);
		
		void free(big_scpacket* _completionMessage = NULL);
		big_scpacket* freeMsg(big_scpacket* _completionMessage = NULL);
		static void freeAll(Server* _server = NULL);
		void zero(big_scpacket* _completionMessage = NULL);
		//big_scpacket zeroMsg(big_scpacket* _completionMessage = NULL);
		void set(int _index, float64 _float);
		//big_scpacket setMsg
		void set(set_list _pairs);
		void setn(int startAt, std::list<float64> _values);
		void setn(setn_list _pairs);
		//setnMsgArgs
		//setnMsg
		
		//void get(int index, Action action = defaultAction);
		//getMsg
		//getn
		//getnMsg
		
		void fill(int _startAt, int _numFrames, std::list<float64> _values);
		void fill(fill_list fillList);
		//fillMsg
		/*
		normalize
		normalizeMsg
		
		gen
		genMsg
		sine1
		sine2
		sine3
		cheby
		sine1Msg
		sine2Msg
		sine3Msg
		chebyMsg
		
		copyData
		copyMsg
		*/
		// close a file, write header, after DiskOut usage
		void close(big_scpacket* _completionMessage = NULL);
		big_scpacket* closeMsg(big_scpacket* _completionMessage = NULL);
		/*
		query
		
		updateInfo
		
		// cache Buffers for easy info updating
		cache
		uncache 
		*initServerCache 
		*clearServerCaches 
		*cachedBuffersDo 
		*cachedBufferAt 
		
		// called from Server when b_info is received
		queryDone 
		
		printOn 
			
		*loadDialog 
			
		play 
			
		duration 
		 */
		
		Server* getServer();
		int getBufNum();
		int getNumFrames();
		int getNumChannesl();
		int getSampleRate();
		const char* getPath();
		void setPath(const char* _path);
		
		//static IdentityDictionary serverCaches;
		
	private:
		
		static const char* createDefaultRecordingPath(const char* _headerFormat);
		void setNumFrames(int _numFrames);
		void setNumChannels(int _numChannels);
		void setSampleRate(int _sampleRate);
		
		Server* server;
		int bufnum;
		int numFrames;
		int numChannels;
		int sampleRate;
		const char* path;
		//doOnInfo
	};
};
