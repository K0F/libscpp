/*
 *  Thread.h
 *  SC++
 *
 *  Created by Chad McKinney on 2/29/12.
 *  Copyright 2012 __MyCompanyName__. All rights reserved.
 *
 */

#pragma once

#include "boost/thread.hpp"
#include "Stream.h"

namespace sc {

	class Thread : public Stream<float64>, public boost::thread {
		
	};
	
	class Routine : public Thread {
	
	public:
		
	};
};