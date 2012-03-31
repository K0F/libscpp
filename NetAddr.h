/*
 *  NetAddr.h
 *  SC++
 *
 *  Created by Chad McKinney on 2/18/12.
 *
 */

#pragma once

/* The classes below are exported */
#pragma GCC visibility push(default)

// generic pyrslot implementation
#include "SC_Endian.h"
#include "SC_Types.h"
#include "PyrErrors.h"

#include <cstddef>
#include <cassert>
//#include <assert.h>

#include <map>
#include <vector>

#include "sc.h"
#include "SC_WorldOptions.h"
#include "SC_Endian.h"
#include "SC_Types.h"
#include "PyrErrors.h"
#include "scsynthsend.h"

//#include <cstddef>
//#include <cassert>
//#include "PyrSlot.h"
//#include "PyrSlot32.h"
//#include "OSCData.h"

namespace sc {

	class NetAddr {
	
	public:
		
		NetAddr(const char* _hostname = "internal", uint16 _port = 0);
		
		typedef void (*DisconnectHandler)();
		
		static std::map<NetAddr*, DisconnectHandler*> connections;
		
		const char* getAddr();
		uint16 getPort();
		void setPort(uint16 _port);
		const char* getHostName();
		//void sendMsg(const char* msg);
		void sendMsg(small_scpacket* packet);
		void sendMsg(big_scpacket* packet);
		void sendBundle(float32 time, std::vector<const char*> msgs);
		
	private:

		const char* addr;
		const char* hostname;
		uint16 port;
	};
};

#pragma GCC visibility pop