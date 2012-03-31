/*
 *  NetAddr.cpp
 *  SC++
 *
 *  Created by Chad McKinney on 2/18/12.
 *
 */
#include "NetAddr.h"
#include "PyrErrors.h"
//#define NULL 0

//#include "VMGlobals.h"

namespace sc {
	
	NetAddr::NetAddr(const char* _hostname, uint16 _port)
	{
		this->hostname = _hostname;
		this->port = _port;
	}
	
	const char* NetAddr::getAddr()
	{
		return this->addr;
	}
	
	uint16 NetAddr::getPort()
	{
		return this->port;
	}
	
	void NetAddr::setPort(uint16 _port)
	{
		this->port = _port;
	}
	
	const char* NetAddr::getHostName()
	{
		return this->hostname;
	}
	
	//extern int netAddrSend(PyrObject *netAddrObj, int msglen, char *bufptr, bool sendMsgLen=true);
	
	/*
	void NetAddr::sendMsg(const char* msg)
	{
		// _NetAddr_SendMsg
		
		big_scpacket packet;
		 
		 int numargs = numArgsPushed - 1;
		 int error = makeSynthMsgWithTags(&packet, args, numargs);
		 if (error != errNone)
		 return error;
		 
		 //for (int i=0; i<packet.size()/4; i++) post("%d %p\n", i, packet.buf[i]);
		 
		 return netAddrSend(slotRawObject(netAddrSlot), packet.size(), (char*)packet.buf);
		 
	}*/
	
	void NetAddr::sendMsg(small_scpacket* packet)
	{
		
	}
	
	void NetAddr::sendMsg(big_scpacket* packet)
	{
		
	}
	
	void NetAddr::sendBundle(float32 time, std::vector<const char*> msgs)
	{
		
	}
}