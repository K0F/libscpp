/*
 *  AbstractFunction.h
 *  SC++
 *
 *  Created by Chad McKinney on 2/29/12.
 *  Copyright 2012 __MyCompanyName__. All rights reserved.
 *
 */

#pragma once

namespace sc {
	
	class UnaryOpFunction;
	class BinraryOpFunction;
	
	class AbstractFunction {
		
	public:
		
	};
	
	class UnaryOpFunction : public AbstractFunction {
		
	};
	
	class BinraryOpFunction : public AbstractFunction {
		
	};
	
	class NAryOpFunction : AbstractFunction {
		
	};
	
};