/*
 *  Stream.h
 *  SC++
 *
 *  Created by Chad McKinney on 2/29/12.
 *
 */

#pragma once

#include "AbstractFunction.h"
#include "List.h"
#include <vector>
#include "sc.h"
#include "Function.h"
#include "Event.h"
#include "Clock.h"
#include "Quant.h"

namespace sc {

	class Routine;
	class EventStreamPlayer;
		
	///////////
	// Stream
	///////////
	
	template<class T>
	class Stream : public AbstractFunction {
		
	public:
		
		Stream():
			streamSize(0)
		{
			
		}
	
		virtual T next(T inval);
		
		virtual T next();
		
		Stream* iter()
		{
			return this;
		}
		
		T value(T inval)
		{
			--streamSize;
			return this->next(inval);
		}
		
		T valueArray()
		{
			--streamSize;
			return this->next();
		}
		
		std::vector<T> nextN(uint32 n, T inval)
		{
			std::vector<T> nList;
			
			for(int i = 0; i < n; ++i)
			{
				nList.push_back(*(this->next(inval)));
			}
			
			streamSize -= n;
			return nList;
		}
		
		//template<class T>
		//std::vector<T> all();
		
		// OVERRIDE IN SUBCLASS FOR FUNCTIONALITY, BUT YOU MUST CALL THIS TO INCREMENT THE STREAM SIZE!!
		void put(T item)
		{
			++streamSize;
		}
		
		// OVERRIDE IN SUBCLASS FOR FUNCTIONALITY, BUT YOU MUST CALL THIS TO INCREMENT THE STREAM SIZE!!
		void putN(uint32 n, T item)
		{
			streamSize += n;
		}
		
		void putAll(std::vector<T> aCollection)
		{
			for(uint32 i = 0; i < aCollection.size(); ++i)
			{
				this->put(aCollection[i]);
			}
		}
		
		void doForEach(ScFunction function = 0)
		{
			T item = this->next();
			uint32 numStreamElements = streamSize;
			
			for(uint32 i = 0; i < numStreamElements; ++i)
			{
				function(2, item, i);
				item = this->next();
			}
		}
		
		/*
		subSample
		generate
		reject
		select
		dot
		interace
		*/
		
		// compiler won't let us use ++ ...booooo...
		//Routine operator+=(Stream& stream);
		//Routine appendStream(Stream& stream);
		
		/*
		collate
		<>
		composeUnaryOp
		comboseBinaryOp
		reverseCbomposeBinaryOp
		compseNAryOp
		*/
		
		virtual void yield(boost::any inval);
		
		// This seems....questionable...
		T embedInStream(T inval)
		{
			T outval;
			
			for(uint32 i = 0; i < streamSize; ++i)
			{
				outval = this->value(inval);
				inval = T(this->yield(outval));
			}
			
			return inval;
		}
		
		EventStreamPlayer* asEventStreamPlayer(Event* protoEvent)
		{
			return new EventStreamPlayer(this, protoEvent);
		}
		
		void play(TempoClock* clock = TempoClock::defaultClock, Quant quant = Quant(1.0))
		{
			clock->play((Task*) this, quant);
		}
		
		virtual void reset()
		{
			
		}
		
		//trace
		//repeat
		
	protected:
		
		// Keeps track of the size of the stream
		uint32 streamSize;
	};
	
	
	/////////////////
	// OneShotStream
	/////////////////
	
	template<class T>
	class OneShotStream : public Stream<T> {
		
	};
	
	/////////////////
	// EmbedOnce
	/////////////////
	
	template<class T>
	class EmbedOnce : public Stream<T> {
		
	};
	
	/////////////////
	// FuncStream
	/////////////////
	
	template<class T>
	class FuncStream : public Stream<T> {
		
	};
	
	/////////////////
	// StreamClutch
	/////////////////
	
	template<class T>
	class StreamClutch : public Stream<T> {
		
	};
	
	/////////////////
	// CleanupStream
	/////////////////
	
	template<class T>
	class CleanupStream : public Stream<T> {
		
	};
	
	/////////////////
	// PauseStream
	/////////////////
	
	// PauseStream is a stream wrapper that can be started and stopped.
	class PauseStream : public Stream<float64> {
		
	public:
		
		PauseStream(Stream<float64>* _stream = NULL, TempoClock* _clock = NULL);
		
		bool isPlaying();
		PauseStream* play(TempoClock* _clock = NULL, bool doReset = false, Quant quant = *Quant::getDefault());
		void reset();
		void stop();
		void removeFromScheduler();
		void streamError();
		bool wasStopped();
		bool canPause();
		bool pause();
		PauseStream* resume(TempoClock* _clock, Quant quant);
		void refresh();
		PauseStream* start(TempoClock* _clock, Quant quant);
		float64 next(float64 inval);
		float64 awake(uint32 beats, uint32 seconds, TempoClock* inClock);
		
		void setStream(Stream<float64>* _stream);
		Stream<float64>* getStream();
		Stream<float64>* getOriginalStream();
		TempoClock* getClock();
		void setStreamHasEnded(bool _streamHasEnded);
		bool getStreamHasEnded();
		
		
	private:
		
		void prStop();
		
		Stream<float64>* stream;
		Stream<float64>* originalStream;
		TempoClock* clock;
		uint32 nextBeat;
		bool streamHasEnded, isWaiting;
		uint32 era;
	};
	
	/////////
	// Task
	/////////
	
	// Task is a PauseStream for wrapping a Routine
	class Task : public PauseStream {
		
	};
	
	/////////////////////
	// EventStreamPlayer
	/////////////////////
	
	class EventStreamPlayer : public PauseStream {
	
	public:
		
		EventStreamPlayer(Stream<float64>* _stream, Event* event = Event::defaultEvent());
		
		Event* event;
	};
};