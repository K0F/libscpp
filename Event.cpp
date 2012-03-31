/*
 *  Event.cpp
 *  SC++
 *
 *
 */

#include "Event.h"

namespace sc {
	
	//Event* Event::defaultParentEvent = new Event(8, NULL, NULL, true);
	
	Event::Event(uint32 _n, Event* _proto, Event* _parent, bool _know) :
		n(_n),
		proto(_proto),
		parent(_parent),
		know(_know)
	{
		
	}
	
	Event* Event::defaultEvent()
	{
		return new Event(8, NULL, defaultParentEvent, true);
	}
	
	Event* Event::silent(float64 dur, Event* inEvent)
	{
		if(inEvent == NULL)
		{
			inEvent = new Event();
		}
		
		else
		{
			inEvent = new Event(*inEvent);
		}
		
		inEvent->insert(std::make_pair("isRest", true));
		inEvent->insert(std::make_pair("dur", dur));
		inEvent->insert(std::make_pair("parent", defaultParentEvent));
		float64 delta;
		Environment::iterator iter = inEvent->find("stretch");
		if(iter != inEvent->end())
			delta = boost::any_cast<float64>(inEvent->at("stretch"));
		else
			delta = 1.0f;
		
		inEvent->insert(std::make_pair("delta", delta));
		return inEvent;
	}
	
	float64 Event::delta()
	{
		float64 delta;
		Environment::iterator iter = this->find("delta");
		
		if(iter != this->end())
			delta = boost::any_cast<float64>(*iter);
		else
			delta = boost::any_cast<float64>(this->at("dur")) * boost::any_cast<float64>(this->at("stretch"));
		
		return delta;
	}
	
	void Event::play()
	{
		if(this->parent == NULL)
			parent = Event::defaultParentEvent;
		use(boost::any_cast<EnvirUseFunc>(this->at("play")));
	}
	
	bool Event::isRest()
	{
		bool resting = false;
		Environment::iterator iter = this->find("isRest");
		
		if(iter != this->end())
		{
			return boost::any_cast<bool>(*iter);
		}
		
		else
		{
			iter = this->find("type");
			if(iter != this->end())
			{
				return strcmp(boost::any_cast<const char*>(*iter), "rest") == 0;
			}
			
			else
			{
				//use(<#EnvirUseFunc function#>)
			}
		}
		
		return resting;
	}
	
	float64 Event::playAndDelta(boost::any cleanup, bool mute)
	{
		if(mute)
			this->insert(std::make_pair("type", "rest"));
		//cleanup.update(this);
		play();
		return delta();
	}
	
	// A Quant specifies the quant and phase at which a TempoClock starts an EventStreamPlayer
	// Its offset specifies how far ahead of time events are actually computed by the EventStream.
	// offset allows ~strum to be negative, so strummed chords complete on the beat
	// it also makes it possible for one pattern to run a little ahead of another to set values
	// This method keeps ~timingOffset and Quant.offset the same.
	/*
	Event* Event::syncWithQuant(float64 quant)
	{
		
	}*/
	
	void Event::initClass()
	{
		
	}
	
	void Event::makeParentEvents()
	{
		
	}
};
