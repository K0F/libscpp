/*
 *  Environment.h
 *  SC++
 *
 *  Created by Chad McKinney on 2/29/12.
 *
 */

#pragma once

#include "boost/unordered_map.hpp"
#include "boost/any.hpp"
#include "boost/function.hpp"
#include <deque>

#include "Function.h"

namespace sc {
	
	class Environment : public boost::unordered_map<const char*, boost::any> {
		
	public:
		
		typedef boost::function<void (Environment*)> EnvirMakeFunc;
		typedef boost::function<boost::any (Environment*)> EnvirUseFunc;
		
		static Environment* staticMake(EnvirMakeFunc function);
		Environment* staticUse(EnvirUseFunc function);
		
		void make(EnvirMakeFunc function);
		boost::any use(EnvirUseFunc function);
		
		boost::any eventAt(const char* key);
		
		// composeEvents
		
		static void staticPop();
		static void staticPush(Environment* envir);
		
		void pop();
		void push();
		
		/*
		NOT IMPLEMENTED
		linkDoc
		unlinkDoc
		*/
		
		static std::deque<Environment*> stack;
		
	private:
		
		static Environment* currentEnvironment;
	};
	
};