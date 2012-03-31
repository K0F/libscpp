/*
 *  Engine.h
 *  SC++
 *
 *  Created by Chad McKinney on 2/18/12.
 *
 */

#pragma once

/* The classes below are exported */
#pragma GCC visibility push(default)

#include <set>
#include <vector>
#include <deque>
#include <map>

#include "sc.h"

namespace sc {

	class NodeIDAllocator {
	
	public:
		
		NodeIDAllocator(uint8 _user = 0, uint16 _initTemp = 1000);
		
		void reset();
		uint16 alloc();
		uint16 allocPerm();
		void freePerm(uint16 id);
		
		uint8 getUser();
		
	private:
		uint8 user;
		uint16 initTemp, temp, perm, mask;
		std::set<uint16> permFreed;
	};
	
	class StackNumberAllocator {
			
	public:
		
		StackNumberAllocator(uint32 _lo, uint32 _hi);
		
		int64 alloc();
		void free(uint32 index);
		
	private:
		
		uint16 lo, hi, next;
		std::deque<int32> freeList;
	};
	
	class ContiguousBlock {
	
	public:
		
		ContiguousBlock(uint32 _start, uint32 _size);
		
		uint32 address();
		bool adjoins(ContiguousBlock* block);
		ContiguousBlock* join(ContiguousBlock* block);
		ContiguousBlock** split(uint32 span);
		uint32* storeArgs(); // 3 index array [start, size, used(bool as 0/1)]
		//void printOn(Stream stream); // NOT IMPLEMENTED YET
		
		uint32 getStart();
		uint32 getSize();
		bool getUsed();
		void setUsed(bool _used);
		
	private:
		uint32 start, size;
		bool used; // assume free; owner must say otherwise
	};
	
	class ContiguousBlockAllocator {
	
	public:
		
		ContiguousBlockAllocator(uint32 _size, uint32 _pos = 0);
		~ContiguousBlockAllocator();
		
		int64 alloc(uint32 n = 1);
		ContiguousBlock* reserve(uint32 address, uint32 _size = 1, bool warn = true);
		void free(uint32 address);
		std::vector<ContiguousBlock*> blocks();
		ContiguousBlock* findAvailable(uint32 n);
		void addToFreed(ContiguousBlock* block);
		void removeFromFreed(ContiguousBlock* block);
		ContiguousBlock* findPrevious(uint32 address);
		ContiguousBlock* findNext(uint32 address);
		ContiguousBlock* prReserve(uint32 address, uint32 _size, ContiguousBlock* availBlock, 
								   ContiguousBlock* prevBlock = 0);
		ContiguousBlock** prSplit(ContiguousBlock* availBlock, uint32 n, bool used = true);
		void debug(const char* text);
		
	private:
		
		uint32 size, pos, top;
		//std::vector<ContiguousBlock*> array;
		ContiguousBlock** array;
		std::map< uint32, std::set<ContiguousBlock*>* > freed;
	};
};

#pragma GCC visibility pop