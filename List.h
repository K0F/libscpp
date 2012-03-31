/*
 *  List.h
 *  SC++
 *
 *  Created by Chad McKinney on 3/5/12.
 *
 */

#pragma once

#include <vector>
#include "SequenceableCollection.h"
#include "boost/any.hpp"

namespace sc {

	class List : public SequenceableCollection, public std::vector<boost::any> {
		
	public:
				
	};
};