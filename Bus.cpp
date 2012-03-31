/*
 *  Bus.cpp
 *  SC++
 *
 *  Created by Chad McKinney on 2/19/12.
 *
 */

#include <iostream>

#include "Bus.h"
#include "Server.h"

namespace sc {

	Bus::Bus(Rate _rate, uint32 _index, uint32 _numChannels, Server* _server) :
		rate(_rate),
		index(_index),
		numChannels(_numChannels)
	{
		if(_server == NULL)
			server = Server::internal;
		else
			server = _server;
		
		mapSymbol = NULL;
		
		
	}
		
	Bus* Bus::control(Server* _server, uint32 _numChannels)
	{
		int64 alloc = _server->getControlBusAllocator()->alloc(_numChannels);
		if(alloc == NEGNULL)
		{
			std::cerr << "Meta_Bus:control:failed to get a control bus allocated. "
				<< "numChannels: " << _numChannels << " server: " << _server->getName()
				<< std::endl;
			return NULL;
		}
		
		return new Bus(Control, alloc, _numChannels, _server);
	}
	
	Bus* Bus::audio(Server* _server, uint32 _numChannels)
	{
		int64 alloc = _server->getAudioBusAllocator()->alloc(_numChannels);
		if(alloc == NEGNULL)
		{
			std::cerr << "Meta_Bus:control:failed to get a control bus allocated. "
			<< "numChannels: " << _numChannels << " server: " << _server->getName()
			<< std::endl;
			return NULL;
		}
		
		return new Bus(Audio, alloc, _numChannels, _server);
	}
	
	Bus* Bus::alloc(Rate _rate, Server* _server, uint32 _numChannels)
	{
		if(_rate == Control)
			return Bus::control(_server, _numChannels);
		else if(_rate == Audio)
			return Bus::audio(_server, _numChannels);
		else
			return NULL;
	}	
	
	bool Bus::isSettable()
	{
		return rate != Audio;
	}
	
	void Bus::set(float64 _value)
	{
		big_scpacket* packet = setMsg(_value);
		server->sendMsg(packet);
		delete packet;
		packet = NULL;
	}
	
	big_scpacket* Bus::setMsg(float64 _value)
	{
		big_scpacket* packet = new big_scpacket();
		
		packet->BeginMsg();
		
		packet->addi(c_set);
		packet->maketags(3);
		packet->addtag(',');
		packet->addtag('i');
		packet->addi(index);
		packet->addtag('f');
		packet->addf(_value);
		
		packet->EndMsg();
		return packet;
	}
	
	void Bus::setn(std::list<float64> _values)
	{
		if(this->isSettable())
		{
			big_scpacket packet;
			//packet->OpenBundle(0.0f); // A bundle in SC source, not sure if we need to do it that way if we're rolling the packet ourselves
			
			packet.BeginMsg();
			
			packet.addi(c_setn);
			packet.maketags(3 + _values.size());
			packet.addtag(',');
			packet.addtag('i');
			packet.addi(index);
			packet.addtag('i');
			packet.addi(_values.size());
			
			std::list<float64>::iterator iter = _values.begin();
			while(iter != _values.end())
			{
				
				packet.addtag('f');
				packet.addf(*iter);
				
				
				++iter;
			}
			
			packet.EndMsg();
			server->sendMsg(&packet);
			//packet->CloseBundle();
		}
		
		else
		{
			std::cerr << "Cannot set an audio rate bus." << std::endl;
		}
	}
	
	big_scpacket* Bus::setnMsg(std::list<float64> _values)
	{
		if(this->isSettable())
		{
			big_scpacket* packet = new big_scpacket();
			//packet->OpenBundle(0.0f); // A bundle in SC source, not sure if we need to do it that way if we're rolling the packet ourselves
			
			packet->BeginMsg();
			
			packet->addi(c_setn);
			packet->maketags(3 + _values.size());
			packet->addtag(',');
			packet->addtag('i');
			packet->addi(index);
			packet->addtag('i');
			packet->addi(_values.size());
			
			std::list<float64>::iterator iter = _values.begin();
			while(iter != _values.end())
			{
				
				packet->addtag('f');
				packet->addf(*iter);
				
				
				++iter;
			}
			
			packet->EndMsg();
			return packet;
			//packet->CloseBundle();
		}
		
		else
		{
			std::cerr << "Cannot set an audio rate bus." << std::endl;
			return NULL;
		}
	}
	
	void Bus::setnAt(uint32 _offset, std::list<float64> _values)
	{
		if(this->isSettable())
		{
			big_scpacket packet;
			packet.OpenBundle(0.0f);
			
			
			uint i = 0;
			std::list<float64>::iterator iter = _values.begin();
			while(iter != _values.end())
			{
				packet.BeginMsg();
				
				packet.addi(c_set);
				packet.maketags(3);
				packet.addtag(',');
				packet.addtag('i');
				packet.addi(index + _offset + i);
				
				packet.addtag('f');
				packet.addf(*iter);
				
				packet.EndMsg();
				++iter;
				++i;
			}
			
			packet.CloseBundle();
			server->sendMsg(&packet);
		}
		
		else
		{
			std::cerr << "Cannot set an audio rate bus." << std::endl;
		}
	}
	
	void Bus::fill(double _value, uint32 _numChannels)
	{
		big_scpacket* packet = fillMsg(_value, _numChannels);
		server->sendMsg(packet);
		delete packet;
		packet = NULL;
	}
	
	big_scpacket* Bus::fillMsg(double _value, uint32 _numChannels)
	{
		big_scpacket* packet = new big_scpacket();
		
		packet->BeginMsg();
		
		packet->addi(c_fill);
		packet->maketags(4);
		packet->addtag(',');
		packet->addtag('i');
		packet->addi(index);
		packet->addtag('i');
		packet->addi(_numChannels);
		packet->addtag('f');
		packet->addf(_value);
		
		packet->EndMsg();
		return packet;
	}
	
	void Bus::free()
	{
		if(rate == Audio)
		{
			server->getAudioBusAllocator()->free(index);
		}
		
		else
		{
			server->getControlBusAllocator()->free(index);
		}
		
		index = NULL;
		numChannels = NULL;
		mapSymbol = NULL;
	}
	
	void Bus::alloc()
	{
		if(rate == Audio)
		{
			index = server->getAudioBusAllocator()->alloc(numChannels);
		}
		
		else
		{
			index = server->getControlBusAllocator()->alloc(numChannels);
		}
		
		mapSymbol = NULL;
	}
	
	void Bus::realloc()
	{
		Rate r;
		uint32 n;
		
		if(index != NEGNULL)
		{
			r = rate;
			n = numChannels;
			this->free();
			rate = r;
			numChannels = n;
			this->alloc();
		}
	}
	
	void Bus::setAll(double _value)
	{
		fill(_value, numChannels);
	}
	
	void Bus::setValue(double _value)
	{
		fill(_value, numChannels);
	}
	
	bool Bus::isAudioOut()
	{
		return (rate == Audio) && (index > server->getOptions()->firstPrivateBus());
	}
	
	Bus* Bus::newFrom(Bus* _bus, uint32 _offset, uint32 _numChannels)
	{
		if((_offset > _bus->getNumChannels()) || (_numChannels + _offset > _bus->getNumChannels()))
		{
			std::cerr << "Bus:newFrom tried to reach outside the channel range of bus" << std::endl;
			return NULL;
		}
		
		return new Bus(_bus->getRate(), _bus->getIndex(), _bus->getNumChannels());
	}
	
	Bus* Bus::subBus(uint32 _offset, uint32 _numChannels)
	{
		return newFrom(this, _offset, _numChannels);
	}
	
	Bus::Rate Bus::getRate()
	{
		return rate;
	}
	
	uint32 Bus::getIndex()
	{
		return index;
	}
	
	uint32 Bus::getNumChannels()
	{
		return numChannels;
	}
	
	Server* Bus::getServer()
	{
		return server;
	}
}
