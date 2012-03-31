/*
 *  Bus.h
 *  SC++
 *
 *  Created by Chad McKinney on 2/19/12.
 *
 */
#pragma once

/* The classes below are exported */
#pragma GCC visibility push(default)

#include <list>
#include "sc.h"
#include "scsynthsend.h"
#include "boost/function.hpp"

namespace sc {
	
	class Server;

	class Bus {
		
	public:
		
		typedef boost::function<void (Bus* _bus)> Action;
		
		inline void defaultAction(Bus* _bus = NULL)
		{
			
		};
		
		enum Rate {
			Control,
			Audio
		};
		
		Bus(Rate _rate = Audio, uint32 _index = 0, uint32 _numChannels = 2, Server* _server = NULL);
		
		static Bus* control(Server* _server = NULL, uint32 _numChannels = 1);
		static Bus* audio(Server* _server = NULL, uint32 _numChannels = 1);
		static Bus* alloc(Rate _rate, Server* _server = NULL, uint32 _numChannels = 1);
		
		bool isSettable();
		void set(float64 _value); // was a bundle, however in C++ setn is about identical without some macro foolery
		big_scpacket* setMsg(float64 _value);
		void setn(std::list<float64> _values); // shouldn't be larger than this->numChannels
		big_scpacket* setnMsg(std::list<float64> _values);
		//void setAt(uint32 _offset, std::list<float64> _values); // shouldn't be larger than this.numChannels - offset
		void setnAt(uint32 _offset, std::list<float64> _values);
		
		//setPairs
		
		/*
		get(Bus::Action _action = &Bus::defaultAction);
		getn
		getMsg
		getnMsg
		getnSynchronous
		setSynchronous
		setnSynchronous
		*/
		
		void fill(double _value, uint32 _numChannels);
		big_scpacket* fillMsg(double _value, uint32 _numChannels);
		
		void free();
		void alloc();
		void realloc();
		void setAll(double _value);
		void setValue(double _value);
		
		/*
		printOn
		storeOn
		==
		hash
		*/
		
		bool isAudioOut(); //Audio Interface
		
		/*
		ar
		kr
		play
		asUGenInput
		asControlInput
		asMap
		*/
		
		static Bus* newFrom(Bus* _bus, uint32 _offset, uint32 _numChannels = 1);
		Bus* subBus(uint32 _offset, uint32 _numChannels = 1);
		
		Bus::Rate getRate();
		uint32 getIndex();
		uint32 getNumChannels();
		Server* getServer();
		
	private:
		
		Server* server;
		int64 numChannels, index;
		Rate rate;
		const char* mapSymbol;
	};
	
};

#pragma GCC visibility pop