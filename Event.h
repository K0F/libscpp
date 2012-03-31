/*
 *  Event.h
 *  SC++
 *
 *  Created by Chad McKinney on 2/29/12.
 *
 */
#pragma once

#include <boost/unordered_map.hpp>

#include "sc.h"
#include "Environment.h"

namespace sc {
	
	class Event : public Environment {
		
	public:
		
		Event(uint32 _n = 8, Event* _proto = NULL, Event* _parent = NULL, bool _know = true);
		
		static Event* defaultEvent();
		static Event* silent(float64 dur = 1.0f, Event* inEvent = NULL);
		//static void addEventType NOT IMPLEMENTED YET
		
		//next 
		
		float64 delta();
		void play();
		bool isRest();
		
		// cleanup is...?
		float64 playAndDelta(boost::any cleanup, bool mute);
		//Event* syncWithQuant(float64 quant);
		//asControlInput
		//asUGenInput
		//printOn
		//storeOn
		
		
		static void initClass();
		//static void makeDefaultSynthDef
		static void makeParentEvents();
		
		static boost::unordered_map<const char*, boost::any> parentEvents;
		static boost::unordered_map<const char*, boost::any> partialEvents;
		
	private:
		
		static Event* defaultParentEvent;
		uint32 n; 
		Event* proto;
		Event* parent;
		bool know;
	};
	
};
