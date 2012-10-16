/*
 *  Buffer.cpp
 *  SC++
 *
 *  Created by Chad McKinney on 3/18/12.
 *
 */
#include <cmath>
#include <iostream>
#include <string>

#include "Buffer.h"
#include "Server.h"
#include "boost/date_time/posix_time/posix_time.hpp"
#include "boost/date_time/gregorian/gregorian.hpp"

namespace sc {
	
	Buffer::Buffer(Server* _server, int _numFrames, int _numChannels, int _bufnum) :
		server(_server), numFrames(_numFrames), numChannels(_numChannels), bufnum(_bufnum)
	{
		if(bufnum == NEGNULL)
		{
			bufnum = server->getBufferAllocator()->alloc(1);
		}
		
		if(bufnum == NEGNULL)
			std::cerr << "No more buffer numbers -- free some buffers before allocating more" << std::endl;
		
		sampleRate = server->getSampleRate();

	}
	
	Buffer::~Buffer()
	{
		free();
	}
	
	Buffer* Buffer::alloc(Server* _server, int _numFrames, int _numChannels, big_scpacket* _completionMessage, 
						  int _bufnum)
	{
		if(_bufnum == NEGNULL)
		{
			_bufnum = _server->getBufferAllocator()->alloc(1);
		}
		
		if(_bufnum == NEGNULL)
			std::cerr << "No more buffer numbers -- free some buffers before allocating more" << std::endl;
		
		Buffer* buffer = new Buffer(_server, _numFrames, _numChannels, _bufnum);
		buffer->alloc(_completionMessage);
		buffer->setSampleRate(_server->getSampleRate());

		return buffer;
	}
	
	std::vector<Buffer*> Buffer::allocConsecutive(int _numBufs, Server* _server, int _numFrames, int _numChannels, 
												  big_scpacket* _completionMessage, int _bufnum)
	{
		int bufBase;
		std::vector<Buffer*> bufArray;
		
		if(_bufnum == NEGNULL)
			bufBase =  _server->getBufferAllocator()->alloc(_numBufs);
		else
			bufBase = _bufnum;
		
		if(bufBase == NEGNULL)
			std::cerr << "No more buffer numbers -- free some buffers before allocating more" << std::endl;
		
		for(int i = 0; i < _numBufs; ++i)
		{
			Buffer* newBuf = new Buffer(_server, _numFrames, _numChannels, i + bufBase);
						
			big_scpacket packet;
			packet.BeginMsg();
			
			packet.addi(b_alloc);
			packet.maketags(4);
			packet.addtag(',');
			packet.addtag('i');
			packet.addi(i + bufBase);
			packet.addtag('i');
			packet.addi(_numFrames);
			packet.addtag('i');
			packet.addi(_numChannels);
			
			packet.EndMsg();
			
			_server->sendMsg(&packet);
			bufArray.push_back(newBuf);
			//newBuf->cache;
		}
		
		return bufArray;
	}
	
	void Buffer::alloc(big_scpacket* _completionMessage)
	{
		big_scpacket* msg = allocMsg(_completionMessage);
		server->sendMsg(msg);
		delete msg;
		msg = NULL;
	}
	
	void Buffer::allocRead(const char* _argpath, int _startFrame, int _numFrames, 
						   big_scpacket* _completionMessage)
	{
		path = _argpath;		
		big_scpacket* packet = allocReadMsg(path, _startFrame, _numFrames, _completionMessage);
		server->sendMsg(packet);
		delete packet;
		packet = NULL;
	}
	
	big_scpacket* Buffer::allocReadMsg(const char* _argpath, int _startFrame, int _numFrames, 
						   big_scpacket* _completionMessage)
	{
		big_scpacket* packet = new big_scpacket();
		packet->BeginMsg();
		
		packet->addi(b_allocRead);
		packet->maketags(5);
		packet->addtag(',');
		packet->addtag('i');
		packet->addi(bufnum);
		packet->addtag('s');
		packet->adds(_argpath);
		packet->addtag('i');
		packet->addi(_startFrame);
		packet->addtag('i');
		packet->addi(_numFrames);
		
		packet->EndMsg();
		return packet;
	}
	
	void Buffer::allocReadChannel(const char* _argpath, int _startFrame, int _numFrames, int _channels, 
								  big_scpacket* _completionMessage)
	{		
		path = _argpath;		
		big_scpacket* packet = allocReadChannelMsg(path, _startFrame, _numFrames, _channels);
		server->sendMsg(packet);
		delete packet;
		packet = NULL;
	}
	
	big_scpacket* Buffer::allocReadChannelMsg(const char* _argpath, int _startFrame, int _numFrames, int _channels)
	{
		big_scpacket* packet = new big_scpacket();
		packet->BeginMsg();
		
		packet->addi(b_allocReadChannel);
		packet->maketags(6);
		packet->addtag(',');
		packet->addtag('i');
		packet->addi(bufnum);
		packet->addtag('s');
		packet->adds(path);
		packet->addtag('i');
		packet->addi(_startFrame);
		packet->addtag('i');
		packet->addi(_numFrames);
		packet->addtag('i');
		packet->addi(_channels);
		
		packet->EndMsg();
		return packet;
	}
	
	big_scpacket* Buffer::allocMsg(big_scpacket* _completionMessage)
	{		
		uint8* data;
		int len;
		uint32 packetLength;
		
		if(_completionMessage != NULL)
		{
			data = (uint8*) _completionMessage->data();
			len = _completionMessage->size();
			packetLength = 5;
		}
		
		else
			packetLength = 4;
		
		
		big_scpacket* packet = new big_scpacket();
		packet->BeginMsg();
		
		packet->addi(b_alloc);
		packet->maketags(packetLength);
		packet->addtag(',');
		packet->addtag('i');
		packet->addi(bufnum);
		packet->addtag('i');
		packet->addi(numFrames);
		packet->addtag('i');
		packet->addi(numChannels);
		
		if(_completionMessage != NULL)
		{
			packet->addtag('b');
			packet->addb(data, len);
		}
		
		packet->EndMsg();
		/*
		packet->BeginMsg();
		
		packet->addi(b_write);
		packet->maketags(8);
		packet->addtag(',');
		packet->addtag('i');
		packet->addi(bufnum);
		packet->addtag('s');
		packet->adds("/Users/curtismckinney/Music/SuperCollider Recordings/TEST_RECORDING.aiff");
		packet->addtag('s');
		packet->adds("aiff");
		packet->addtag('s');
		packet->adds("int24");
		packet->addtag('i');
		packet->addi(0);
		packet->addtag('i');
		packet->addi(0);
		packet->addtag('i');
		packet->addi(1);
		
		packet->EndMsg();*/
		return packet;
	}
	
	big_scpacket* queryCommand(Buffer* buffer)
	{
		big_scpacket* packet = new big_scpacket();
		packet->BeginMsg();
		
		packet->addi(b_query);
		packet->maketags(2);
		packet->addtag(',');
		packet->addtag('i');
		packet->addi(buffer->getBufNum());
		
		packet->EndMsg();
		
		return packet;
	}
	
	Buffer* Buffer::read(Server* _server, const char* _path, int _startFrame, int _numFrames, Action _action, 
						 int _bufnum)
	{
		if(_bufnum == NEGNULL)
			_bufnum = _server->getBufferAllocator()->alloc(1);
		
		if(_bufnum == NEGNULL)
			std::cerr << "No more buffer numbers -- free some buffers before allocating more" << std::endl;
		
		Buffer* buffer = new Buffer(_server, _numFrames, 1, _bufnum);
		big_scpacket* msg = queryCommand(buffer);
		buffer->allocRead(_path, _startFrame, _numFrames, msg);
		delete msg;
		msg = NULL;
		return buffer;
	}
	
	void Buffer::read(const char* _path, int _fileStartFrame, int _numFrames, int _bufStartFrame, bool _leaveOpen, 
					  Action action)
	{
		path = _path;
		//big_scpacket* msg = queryCommand(this);
		//delete msg;
		//msg = NULL;
		
		big_scpacket* packet = readMsg(path, _fileStartFrame, _numFrames, _bufStartFrame, _leaveOpen);
		server->sendMsg(packet);
		delete packet;
		packet = NULL;
	}
	
	big_scpacket* Buffer::readMsg(const char* _path, int _fileStartFrame, int _numFrames, int _bufStartFrame, 
								  bool _leaveOpen)
	{
		big_scpacket* packet = new big_scpacket();
		packet->BeginMsg();
		
		packet->addi(b_read);
		packet->maketags(7);
		packet->addtag(',');
		packet->addtag('i');
		packet->addi(bufnum);
		packet->addtag('s');
		packet->adds(_path);
		packet->addtag('i');
		packet->addi(_fileStartFrame);
		packet->addtag('i');
		packet->addi(_numFrames);
		packet->addtag('i');
		packet->addi(_bufStartFrame);
		packet->addtag('i');
		packet->addi(binaryValue(_leaveOpen));
		
		packet->EndMsg();
		return packet;
	}
	
	Buffer* Buffer::readChannel(Server* _server, const char* _path, int _startFrame, int _numFrames, int _channels, 
								Action _action, int _bufnum)
	{
		if(_bufnum == NEGNULL)
			_bufnum = _server->getBufferAllocator()->alloc(1);
		
		if(_bufnum == NEGNULL)
			std::cerr << "No more buffer numbers -- free some buffers before allocating more" << std::endl;
		
		Buffer* buffer = new Buffer(_server, _numFrames, _channels, _bufnum);
		big_scpacket* msg = queryCommand(buffer);
		buffer->allocReadChannel(_path, _startFrame, _numFrames, _channels, msg);
		
		delete msg;
		msg = NULL;
		
		return buffer;
	}
	
	void Buffer::readChannel(const char* _path, int _fileStartFrame, int _numFrames, int _bufStartFrame, bool _leaveOpen, 
							 int _channels, Action _action)
	{
		path = _path;
		
		//big_scpacket* msg = queryCommand(this);
		//delete msg;
		//msg = NULL;
		
		big_scpacket* packet = readChannelMsg(path, _fileStartFrame, _numFrames, _bufStartFrame, binaryValue(_leaveOpen),
											  _channels);
		server->sendMsg(packet);
		delete packet;
		packet = NULL;
	}
	
	big_scpacket* Buffer::readChannelMsg(const char* _path, int _fileStartFrame, int _numFrames, int _bufStartFrame, 
										 bool _leaveOpen, int _channels)
	{
		big_scpacket* packet = new big_scpacket();
		packet->BeginMsg();
		
		packet->addi(b_readChannel);
		packet->maketags(8);
		packet->addtag(',');
		packet->addtag('i');
		packet->addi(bufnum);
		packet->addtag('s');
		packet->adds(_path);
		packet->addtag('i');
		packet->addi(_fileStartFrame);
		packet->addtag('i');
		packet->addi(_numFrames);
		packet->addtag('i');
		packet->addi(_bufStartFrame);
		packet->addtag('i');
		packet->addi(binaryValue(_leaveOpen));
		packet->addtag('i');
		packet->addi(_channels);
		
		packet->EndMsg();
		return packet;
	}
	
	void Buffer::write(const char* _path, const char* _headerFormat, const char* _sampleFormat, int _numFrames, 
					   int _startFrame, bool _leaveOpen, 
					   big_scpacket* _completionMessage)
	{
		// Yes I know, this is cheating...
		std::string dirString(recordingsDir());
		
		if(_path == NULL)
		{
			boost::posix_time::ptime pt(boost::posix_time::second_clock::local_time());
			
			dirString.append("/SC_");
			dirString.append(boost::gregorian::to_simple_string(pt.date()));
			dirString.append("-");
			dirString.append(boost::posix_time::to_simple_string(pt.time_of_day()));
			
			size_t found = dirString.find(":");
			dirString.replace(dirString.begin() + found, dirString.begin() + found + 1, "-");
			found = dirString.find(":");
			dirString.replace(dirString.begin() + found, dirString.begin() + found + 1, "-");
			
			dirString.append(".");
			dirString.append(_headerFormat);
			path = dirString.c_str();
		}
		
		else
		{
			path = _path;
		}
				
		big_scpacket* msg = writeMsg(path, _headerFormat, _sampleFormat, _numFrames, _startFrame, 
						binaryValue(_leaveOpen), _completionMessage);
		server->sendMsg(msg);
		
		delete msg;
		msg = NULL;
	}
	
	void Buffer::sendCollection(std::vector<float64>& collection, int startFrame, float64 wait)
	{
		if(collection.size() > ((numFrames - startFrame) * numChannels))
			std::cerr << "Collection larger than available number of frames" << std::endl;
		
		int pos = startFrame;
		
		while(pos < collection.size() - 1)
		{
			// std::cout << "Buffer::sendCollection() pos: " << pos << std::endl;
			// 1026 is the max size for setn under udp
			int bundSize = std::min(1026 / 2, (int) collection.size() - pos);
			
			big_scpacket packet;
			packet.BeginMsg();
			packet.addi(b_set);
			packet.maketags(2 + (bundSize * 2));
			packet.addtag(',');
			packet.addtag('i');
			packet.addi(bufnum);
			// packet.addtag('i');
			// packet.addi(pos);
			// packet.addtag('i');
			// packet.addi(bundSize);
			// packet.addtag('b');
			
			// big_scpacket packet2; // Bundle for our array
			// packet2.OpenBundle(1); // 1 = immediate
			// packet2.BeginMsg();
			// packet2.maketags(bundSize + 1);
			// packet2.addtag(',');
			
			for(int i = 0; i < bundSize; ++i)
			{
				packet.addtag('i');
				packet.addi(i + pos);
				packet.addtag('f');
				packet.addf((float) collection.at(i));
			}
			
			// packet2.EndMsg();
			// packet2.CloseBundle();
			// packet.addb((uint8*) packet2.data(), packet2.size());
			packet.EndMsg();
			server->sendMsg(&packet);
			pos += bundSize;
		}
	}
	
	big_scpacket* Buffer::writeMsg(const char* path, const char* headerFormat, const char* sampleFormat, 
						   uint32 numFrames, uint32 startFrame, uint32 leaveOpen, 
						   big_scpacket* _completionMessage)
	{
		uint8* data;
		size_t len;
		uint32 packetLength;
		
		if(_completionMessage != NULL)
		{
			data = (uint8*) _completionMessage->data();
			len = _completionMessage->size();
			packetLength = 9;
		}
		
		else
			packetLength = 8;
		
		big_scpacket* packet = new big_scpacket();
		packet->BeginMsg();
		
		packet->addi(b_write);
		packet->maketags(packetLength);
		packet->addtag(',');
		packet->addtag('i');
		packet->addi(bufnum);
		packet->addtag('s');
		packet->adds(path);
		packet->addtag('s');
		packet->adds(headerFormat);
		packet->addtag('s');
		packet->adds(sampleFormat);
		packet->addtag('i');
		packet->addi(numFrames);
		packet->addtag('i');
		packet->addi(startFrame);
		packet->addtag('i');
		packet->addi(leaveOpen);
		
		if(_completionMessage != NULL)
		{
			packet->addtag('b');
			packet->addb(data, len);
		}
		
		packet->EndMsg();
		
		return packet;
	}
	
	big_scpacket* Buffer::staticWriteMsg(uint32 bufnum, const char* path, const char* headerFormat, 
												const char* sampleFormat, uint32 numFrames, uint32 startFrame, 
												uint32 leaveOpen, big_scpacket* _completionMessage)
	{
		uint8* data;
		size_t len;
		uint32 packetLength;
		
		if(_completionMessage != NULL)
		{
			data = (uint8*) _completionMessage->data();
			len = _completionMessage->size();
			packetLength = 9;
		}
		
		else
			packetLength = 8;
		
		big_scpacket* packet = new big_scpacket();
		packet->BeginMsg();
		
		packet->addi(b_write);
		packet->maketags(packetLength);
		packet->addtag(',');
		packet->addtag('i');
		packet->addi(bufnum);
		packet->addtag('s');
		packet->adds(path);
		packet->addtag('s');
		packet->adds(headerFormat);
		packet->addtag('s');
		packet->adds(sampleFormat);
		packet->addtag('i');
		packet->addi(numFrames);
		packet->addtag('i');
		packet->addi(startFrame);
		packet->addtag('i');
		packet->addi(leaveOpen);
		
		if(_completionMessage != NULL)
		{
			packet->addtag('b');
			packet->addb(data, len);
		}
		
		packet->EndMsg();
		
		return packet;
	}
	
	void Buffer::free(big_scpacket* _completionMessage)
	{
		big_scpacket* msg = freeMsg(_completionMessage);
		server->sendMsg(msg);
		delete msg;
	}
	
	big_scpacket* Buffer::freeMsg(big_scpacket* _completionMessage)
	{
		server->getBufferAllocator()->free(bufnum);
		
		uint8* data;
		size_t len;
		uint32 packetLength;
		
		if(_completionMessage != NULL)
		{
			data = (uint8*) _completionMessage->data();
			len = _completionMessage->size();
			packetLength = 3;
		}
		
		else
			packetLength = 2;
		
		
		big_scpacket* packet = new big_scpacket();
		packet->BeginMsg();
		
		packet->addi(b_free);
		packet->maketags(packetLength);
		packet->addtag(',');
		packet->addtag('i');
		packet->addi(bufnum);
		
		if(_completionMessage != NULL)
		{
			packet->addtag('b');
			packet->addb(data, len);
		}
		
		packet->EndMsg();
		
		return packet;
	}
	
	void Buffer::freeAll(Server* _server)
	{
		if(_server == NULL)
		{
			_server = Server::internal;
		}
	}
	
	void Buffer::zero(big_scpacket* _completionMessage)
	{
		
	}
	
	void Buffer::set(int _index, float64 _float)
	{
		
	}
	
	void Buffer::set(set_list _pairs)
	{
		
	}
	
	void Buffer::setn(int startAt, std::list<float64> _values)
	{
		
	}
	
	void Buffer::setn(setn_list _pairs)
	{
		
	}
	
	void Buffer::fill(int _startAt, int _numFrames, std::list<float64> _values)
	{
		
	}
	
	void Buffer::fill(fill_list fillList)
	{
		
	}

	Server* Buffer::getServer()
	{
		return server;
	}
	
	void Buffer::close(big_scpacket* _completionMessage)
	{
		big_scpacket* msg = closeMsg(_completionMessage);
		server->sendMsg(msg);
		delete msg;
	}
	
	big_scpacket* Buffer::closeMsg(big_scpacket* _completionMessage)
	{
		uint8* data;
		size_t len;
		uint32 packetLength;
		
		if(_completionMessage != NULL)
		{
			data = (uint8*) _completionMessage->data();
			len = _completionMessage->size();
			packetLength = 3;
		}
		
		else
			packetLength = 2;
			
		
		big_scpacket* packet = new big_scpacket();
		packet->BeginMsg();
		
		packet->addi(b_close);
		packet->maketags(packetLength);
		packet->addtag(',');
		packet->addtag('i');
		packet->addi(bufnum);
		
		if(_completionMessage != NULL)
		{
			packet->addtag('b');
			packet->addb(data, len);
		}
		
		packet->EndMsg();
		
		return packet;
	}
	
	int Buffer::getBufNum()
	{
		return bufnum;
	}
	
	int Buffer::getNumFrames()
	{
		return numFrames;
	}
	
	void Buffer::setNumFrames(int _numFrames)
	{
		numFrames = _numFrames;
	}
	
	int Buffer::getNumChannesl()
	{
		return numChannels;
	}
	
	void Buffer::setNumChannels(int _numChannels)
	{
		numChannels = numChannels;
	}
	
	int Buffer::getSampleRate()
	{
		return sampleRate;
	}
	
	void Buffer::setSampleRate(int _sampleRate)
	{
		sampleRate = _sampleRate;
	}
	
	const char* Buffer::getPath()
	{
		return path;
	}
	
	void Buffer::setPath(const char* _path)
	{
		path = _path;
	}
};

