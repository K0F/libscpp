/*
 *  Stream.cpp
 *  SC++
 *
 *  Created by Chad McKinney on 2/29/12.
 *  Copyright 2012 __MyCompanyName__. All rights reserved.
 *
 */
#include <iostream>

#include "Stream.h"
#include "Clock.h"
#include "boost/foreach.hpp"

namespace sc {
	
	/////////////////
	// PauseStream
	////////////////
	
	PauseStream::PauseStream(Stream<float64>* _stream, TempoClock* _clock) :
		stream(NULL),
		originalStream(_stream),
		clock(_clock)
	{
		streamHasEnded = false;
		isWaiting = false;
		era = 0;
	}
		
	bool PauseStream::isPlaying()
	{
		if(stream != NULL)
			return true;
		else
			return false;
	}
	
	PauseStream* PauseStream::play(TempoClock* _clock, bool doReset, Quant quant)
	{
		if(stream != NULL)
		{
			std::cout << "Pause Stream: already playing";
			return this;
		}
		
		if(doReset)
			reset();
		
		if(_clock == NULL)
		{
			if(clock == NULL)
			{
				clock = TempoClock::defaultClock;
			}
		}
		
		else
		{
			clock = _clock;
		}
		
		streamHasEnded = false;
		stream = originalStream;
		isWaiting = true; // make sure that accidental play/stop/play sequences
						  // don't cause memory leaks
		
		//era = CmdPeriod.era Number of times CmdPeriod has been called...not implemented
		
		//clock->play();
		//changed(\userPlayed)
		return this;
	}
	
	void PauseStream::reset()
	{
		originalStream->reset();
	}
	
	void PauseStream::stop()
	{
		
	}
	
	void PauseStream::removeFromScheduler()
	{
		
	}
	
	void PauseStream::streamError()
	{
		
	}
	
	bool PauseStream::wasStopped()
	{
		
	}
	
	bool PauseStream::canPause()
	{
		
	}
	
	bool PauseStream::pause()
	{
		
	}
	
	PauseStream* PauseStream::resume(TempoClock* _clock, Quant quant)
	{
		
	}
	
	void PauseStream::refresh()
	{
		
	}
	
	PauseStream* PauseStream::start(TempoClock* _clock, Quant quant)
	{
		
	}
	
	float64 PauseStream::next(float64 inval)
	{
		Stream<float64>::next(inval);
	}
	
	float64 PauseStream::awake(uint32 beats, uint32 seconds, TempoClock* inClock)
	{
		
	}
	
	void PauseStream::setStream(Stream<float64>* _stream)
	{
		
	}
	
	Stream<float64>* PauseStream::getStream()
	{
		
	}
	
	Stream<float64>* PauseStream::getOriginalStream()
	{
		
	}
	
	TempoClock* PauseStream::getClock()
	{
		
	}
	
	void PauseStream::setStreamHasEnded(bool _streamHasEnded)
	{
		
	}
	
	bool PauseStream::getStreamHasEnded()
	{
		
	}
	
	void PauseStream::prStop()
	{
		
	}
	
	/////////////////////
	// EventStreamPlayer
	////////////////////
	EventStreamPlayer::EventStreamPlayer(Stream<float64>* _stream, Event* event) :
		PauseStream(_stream, TempoClock::defaultClock)
	{
		
	}
};
