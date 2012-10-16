/*
 *  Engine.cpp
 *  SC++
 *
 *  Created by Chad McKinney on 2/18/12.
 *
 */

#include <iostream>

#include "Engine.h"
#include "scmath.h"
#include "sc.h"
//#define NULL 0

namespace sc {
	
	///////////////////
	// NodeIDAllocator
	//////////////////
	
	NodeIDAllocator::NodeIDAllocator(uint8 _user, uint16 _initTemp)
	{
		if(_user > 31)
		{
			std::cerr << "NodeIDAllocator user id > 31";
		}
		
		user = _user;
		initTemp = _initTemp;
		reset();
	}
		
	void NodeIDAllocator::reset()
	{
		mask = user << 26;
		temp = initTemp;
		perm = 2;
		permFreed.clear();
	}
	
	uint16 NodeIDAllocator::alloc()
	{
		uint16 x;
		x = temp;
		temp = wrapInt((x + 1), initTemp, 0x03FFFFFF);
		return x | mask;
	}
	
	uint16 NodeIDAllocator::allocPerm()
	{
		uint16 x, minItem;
		if(permFreed.size() > 0)
		{
			std::set<uint16>::iterator iter = permFreed.begin();
			minItem = *iter;
			while(iter != permFreed.end())
			{
				if(*iter < minItem)
					minItem = *iter;
				++iter;
			}
			x = minItem;
			permFreed.erase(minItem);
		}
		
		else 
		{
			x = perm;
			perm = std::min((x + 1), initTemp - 1);
		}
		return x | mask;
	}
	
	void NodeIDAllocator::freePerm(uint16 id)
	{
		// should not add a temp node id to the freed-permanent collection
		id = id & 0x03FFFFFF;
		if(id < initTemp)
			permFreed.insert(id);
	}
	
	uint8 NodeIDAllocator::getUser()
	{
		return user;
	}
	
	///////////////////////
	//StackNumberAllocator
	//////////////////////
	
	StackNumberAllocator::StackNumberAllocator(uint32 _lo, uint32 _hi) :
		lo(_lo),
		hi(_hi)
	{
		next = lo - 1;
	}
	
	int64 StackNumberAllocator::alloc()
	{
		if(freeList.size() > 0)
		{
			uint32 last = freeList.back();
			freeList.pop_back();
			return last;
		}
		
		if(next < hi)
		{
			next += 1;
			return next;
		}
		
		return NEGNULL;
	}
	
	void StackNumberAllocator::free(uint32 index)
	{
		freeList.push_back(index);
	}
	
	///////////////////
	// ContiguousBlock
	///////////////////
	
	ContiguousBlock::ContiguousBlock(uint32 _start, uint32 _size) :
		start(_start),
		size(_size),
		used(false) // assume free; owner must say otherwise
	{
		
	}
	
	uint32 ContiguousBlock::address()
	{
		return this->start;
	}
	
	bool ContiguousBlock::adjoins(ContiguousBlock* block)
	{
		return ((start < block->getStart()) && ((start + this->size) >= block->getStart()))
			|| ((start > block->getStart()) && (block->getStart() + block->getSize() >= start));
	}
	
	ContiguousBlock* ContiguousBlock::join(ContiguousBlock* block)
	{
		uint32 newStart, newSize;
		if(adjoins(block))
		{
			newStart = std::min(start, block->getStart());
			newSize = std::max(start + this->size, block->getStart() + block->getSize()) - newStart;
			return new ContiguousBlock(newStart, newSize);
		}
		
		else 
		{
			return NULL;
		}

	}
	
	ContiguousBlock** ContiguousBlock::split(uint32 span)
	{
		if(span < this->size)
		{
			ContiguousBlock** splitArray = new ContiguousBlock*[2];
			splitArray[0] = new ContiguousBlock(start, span);
			splitArray[1] = new ContiguousBlock(start + span, this->size - span);
			return splitArray;
		}
		
		else 
		{
			if(span == this->size)
			{
				ContiguousBlock** splitArray = new ContiguousBlock*[2];
				splitArray[0] = this;
				splitArray[1] = NULL;
				return splitArray;	
			}
			
			else 
			{
				return NULL;
			}
		}
	}
	
	uint32* ContiguousBlock::storeArgs()
	{
		uint32* args = new uint32[3];
		args[0] = start;
		args[1] = this->size;
		if(!used)
			args[2] = 0;
		else 
			args[2] = 1;

		return args;
	}
	
	uint32 ContiguousBlock::getStart()
	{
		return start;
	}
	
	uint32 ContiguousBlock::getSize()
	{
		return this->size;
	}
	
	bool ContiguousBlock::getUsed()
	{
		return used;
	}
	
	void ContiguousBlock::setUsed(bool _used)
	{
		this->used = _used;
	}
	
	////////////////////////////
	// ContiguousBlockAllocator
	////////////////////////////
	
	ContiguousBlockAllocator::ContiguousBlockAllocator(uint32 _size, uint32 _pos) :
		size(_size),
		pos(_pos),
		top(_pos)	
	{
		this->array = new ContiguousBlock*[this->size];
		
		for(int i = 0; i < this->size; ++i)
		{
			array[i] = NULL;
		}
		
		array[pos] = new ContiguousBlock(pos, size - pos);
	}
	
	ContiguousBlockAllocator::~ContiguousBlockAllocator()
	{
		for(int i = 0; i < this->size; ++i)
		{
			if(array[i]!=NULL)
			{
				delete array[i];
				array[i] = NULL;
			}
		}
		
		delete[] array;
		array = NULL;
		
		std::map<uint32, std::set<ContiguousBlock*>* >::iterator freedIter = freed.begin();
		while(freedIter != freed.end())
		{
			std::set<ContiguousBlock*>::iterator setIter = freedIter->second->begin();
			while(setIter != freedIter->second->end())
			{
				if(*setIter != NULL)
					delete *setIter;
				++setIter;
			}
			
			freedIter->second->clear();
			delete freedIter->second;
			++freedIter;
		}
		
		freed.clear();
	}
	
	
	int64 ContiguousBlockAllocator::alloc(uint32 n)
	{
		ContiguousBlock* block = findAvailable(n);
		
		if(block != NULL)
		{
			return prReserve(block->getStart(), n, block)->getStart();
		}
		
		else 
		{
			return NEGNULL;
		}

	}
	
	ContiguousBlock* ContiguousBlockAllocator::reserve(uint32 address, uint32 _size, bool warn)
	{
		ContiguousBlock* block = array[address];
		ContiguousBlock* newBlock;
		
		if(block == NULL)
			block = findNext(address);
		
		if(block != NULL)
		{
			if(block->getUsed() && (address + _size > block->getStart()))
			{
				std::cout << "The block at (" << address << ", " << size << ") is already in used and cannot be reserved.";
			}
			
			else 
			{
				if(block->getStart() == address)
				{
					newBlock = prReserve(address, _size, block);
					return newBlock;
				}
				
				else 
				{
					block = findPrevious(address);
					if(block != NULL)
					{
						if(block->getUsed() && (block->getStart() + block->getSize() > address))
						{
							std::cout << "The block at (" << address << ", " 
								<< _size << ") is already in used and cannot be reserved.";
						}
						
						else 
						{
							newBlock = prReserve(address, _size, NULL, block);
							return newBlock;
						}
					}
				}
			}
		}
		
		return NULL;
	}
	
	void ContiguousBlockAllocator::free(uint32 address)
	{
		if(address < size - 1)
		{
			
			if(array[address] != NULL)
			{
				ContiguousBlock* block = array[address];
				ContiguousBlock* prev;
				ContiguousBlock* next;
				ContiguousBlock* temp;
				
				prev = next = temp = NULL;
								
				if(block->getUsed())
				{
					block->setUsed(false);
					addToFreed(block);
					prev = findPrevious(address);
					
					
					if(prev != NULL)
					{
						if(!prev->getUsed())
						{
							temp = prev->join(block);
							if(temp != NULL)
							{
								if(block->getStart() == top)
								{
									top = temp->getStart();
								}
								
								array[temp->getStart()] = temp;
								array[block->getStart()] = NULL;
								removeFromFreed(prev);
								removeFromFreed(block);
								
								if(top > temp->getStart())
								{
									addToFreed(temp);
								}
								
								block = temp;
							}
						}
					}
					
					next = findNext(block->getStart());
					if(next != NULL)
					{
						if(!next->getUsed())
						{
							temp = next->join(block);
							
							if(temp != NULL)
							{
								if(next->getStart() == top)
								{
									top = temp->getStart();
								}
								
								array[temp->getStart()] = temp;
								array[next->getStart()] = NULL;
								removeFromFreed(next);
								removeFromFreed(block);
								
								if(top > temp->getStart())
								{
									addToFreed(temp);
								}
							}
						}
					}
				}
			}
		}
	}
	
	std::vector<ContiguousBlock*> ContiguousBlockAllocator::blocks()
	{
		std::vector<ContiguousBlock*> blocks;
		
		for(int i = 0; i < this->size; ++i)
		{
			if(array[i] != NULL)
			{
				if(array[i]->getUsed())
					blocks.push_back(array[i]);
			}
		}
		
		return blocks;
	}
	
	ContiguousBlock* ContiguousBlockAllocator::findAvailable(uint32 n)
	{
		if(freed.find(n) != freed.end())
		{
			if(freed[n]->size() > 0)
			{
				for(std::set<ContiguousBlock*>::iterator iter = freed[n]->begin();
					iter != freed[n]->end(); ++iter)
				{
					if(*iter != NULL)
						return *iter;
				}
			}
		}
		
		std::map<uint32, std::set<ContiguousBlock*>* >::iterator freedIter = freed.begin();
		while(freedIter != freed.end())
		{
			if(freedIter->first >= n && freedIter->second->size() > 0)
			{
				for(std::set<ContiguousBlock*>::iterator iter = freed[n]->begin();
					iter != freed[n]->end(); ++iter)
				{
					if(*iter != NULL)
						return *iter;
				}
			}
			
			++freedIter;
		}
		
		if((top + n > this->size) || array[top]->getUsed())
		{
			return NULL;
		}
		
		else
		{
			return array[top];
		}
	}
	
	void ContiguousBlockAllocator::addToFreed(ContiguousBlock* block)
	{
		if(freed[block->getSize()] == NULL)
		{
			freed[block->getSize()] = new std::set<ContiguousBlock*>();
		}
		
		else 
		{
			freed[block->getSize()]->insert(block);
		}
	}
	
	void ContiguousBlockAllocator::removeFromFreed(ContiguousBlock* block)
	{
		if(freed.find(block->getSize()) != freed.end())
		{
			std::set<ContiguousBlock*>::iterator setIter = freed[block->getSize()]->begin();
			
			while(setIter != freed[block->getSize()]->end())
			{
				delete *setIter;
				++setIter;
			}
			
			freed[block->getSize()]->clear();
			delete freed[block->getSize()];
			freed[block->getSize()] = NULL;
			freed.erase(block->getSize());
		}
	}
	
	ContiguousBlock* ContiguousBlockAllocator::findPrevious(uint32 address)
	{
		if(address == pos)
		{
			return NULL;
		}
		
		else {
			
			for(int i = address - 1; i >= this->pos; --i)
			{
				if(array[i] != NULL)
					return array[i];
			}
			
			return NULL;
			
		}
	}
	
	ContiguousBlock* ContiguousBlockAllocator::findNext(uint32 address)
	{
		ContiguousBlock* temp = array[address];
		
		if(temp != NULL)
		{
			return array[temp->getStart() + temp->getSize()];
		}
		
		else 
		{
			for(int i = address + 1; i <= this->top; ++i)
			{
				if(array[i] != NULL)
				{
					return array[i];
				}
			}
		}
		
		return NULL;
	}
	
	ContiguousBlock* ContiguousBlockAllocator::prReserve(uint32 address, uint32 _size, ContiguousBlock* availBlock, 
														 ContiguousBlock* prevBlock)
	{
		ContiguousBlock* leftover;
		
		if(availBlock == NULL && prevBlock == NULL)
		{
			prevBlock = findPrevious(address);
		}
		
		if(availBlock == NULL)
			availBlock = prevBlock;
		
		if(availBlock->getStart() < address)
		{
			ContiguousBlock** split = prSplit(availBlock, address - availBlock->getStart(), false);
			leftover = split[0];
			availBlock = split[1];
		}
		
		return prSplit(availBlock, _size, true)[0];	
	}
	
	ContiguousBlock** ContiguousBlockAllocator::prSplit(ContiguousBlock* availBlock, uint32 n, bool used)
	{
		ContiguousBlock** split = availBlock->split(n);
		ContiguousBlock* newBlock = split[0];
		ContiguousBlock* leftover = split[1];
		
		newBlock->setUsed(used);
		removeFromFreed(availBlock);
		
		if(!used)
		{
			addToFreed(newBlock);
		}
		
		array[newBlock->getStart()] = newBlock;
		
		if(leftover != NULL)
		{
			array[leftover->getStart()] = leftover;
			top = std::max(top, leftover->getStart());
			
			if(top > leftover->getStart())
			{
				addToFreed(leftover);
			}
		}
		
		ContiguousBlock** newSplit = new ContiguousBlock*[2];
		newSplit[0] = newBlock;
		newSplit[1] = leftover;
		
		return newSplit;
	}
	
	void ContiguousBlockAllocator::debug(const char* text)
	{
		std::cout << text << ":\n\nArray:\n";
	}
};
