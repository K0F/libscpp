/*
 *  Node.h
 *  SC++
 *
 *  Created by Chad McKinney on 2/19/12.
 *
 */
#pragma once

/* The classes below are exported */
#pragma GCC visibility push(default)

//#include <boost/container/list.hpp>
#include <vector>
#include <utility>
#include <iostream>
#include <set>
#include "sc.h"
#include "Server.h"
#include "scsynthsend.h"

// Macro for easily creating argument lists. To use: ARGS("Arg1", Val1, "Arg2", Val2, etc..); YOU MUST USE DOUBLES: 1., 2., 3., NOT 1, 2, 3
#define ARGS(...) make_args(ARGS_IMPL(__VA_ARGS__, 20, 19, 18, 17, 16, 15, 14, 13, 12, 11, 10, 9, 8, 7, 6, 5, 4, 3, 2, 1), __VA_ARGS__)
#define ARGS_IMPL(_1, _2, _3, _4, _5, _6, _7, _8, _9, _10, _11, _12, _13, _14, _15, _16, _17, _18, _19, _20, N, ...) N

namespace sc {
	
	/*
	inline std::map<const char*, float64> args(int numArgPairs, ...)
	{
		va_list vl;
		
		std::map<const char*, float64> args;
		
		va_start(vl, numArgPairs);
		for(int i = 0; i < numArgPairs; ++i)
		{
			const char* key = va_arg(vl, const char*);
			args.insert(std::make_pair(key, va_arg(vl, float64)));
		}
		va_end(vl);
		return args;
	};
	
	typedef boost::array< std::pair<const char*, float64> > arg_array;*/
	
	
	// CONVENIANCE METHOD FOR CREATE ARGUMENT/VALUE PAIRS
	inline node_arg_list make_args(int numArgs, ...)
	{
		node_arg_list argList;
		va_list vl;
		va_start(vl, numArgs);
		for(int i = 0; i < numArgs / 2; ++i)
		{
			argList.push_back(arg_pair(va_arg(vl, const char*), va_arg(vl, float64)));
		}
		va_end(vl);
		
		return argList;
	};

	class AbstractGroup;
	
	/////////
	// Node
	/////////
	
	class Node {
		
	public:
		
		Node(Server* _server = Server::internal, uint32 _nodeID = NEGNULL);
		
		void free(bool sendFlag = true);
		big_scpacket* freeMsg();
		void run(bool flag = true);
		big_scpacket* runMsg(bool flag = true);
		
		/* METHODS NOT IMPLEMENTED
		void map() 
		mapMsg
		mapn
		mapnMsg
		*/
		
		void set(const char* _argName, float64 _argVal);
		void set(node_arg_list args);
		big_scpacket* setMsg(node_arg_list args);
		void setn(uint32 firstIndex, uint32 numIndexes, std::vector<float64> args);
		big_scpacket* setnMsg(uint32 firstIndex, uint32 numIndexes, std::vector<float64> args);
		
		void fill(const char* controlName, uint32 numControls, std::vector<float64> args);
		big_scpacket* fillMsg(const char* controlName, uint32 numControls, std::vector<float64>& args);
		void release(float64 releaseTime);
		big_scpacket* releaseMsg(float64 releaseTime);
		
		void trace();
		big_scpacket* traceMsg();
		void query();
		void registerNode(bool assumePlaying); // formerly register
		
		typedef void (*OnFreeHandler)();
		void onFree(OnFreeHandler* func);
		void waitForFree();
		
		void moveBefore(Node* aNode);
		big_scpacket* moveBeforeMsg(Node* aNode);
		void moveAfter(Node* aNode);
		big_scpacket* moveAfterMsg(Node* aNode);
		void moveToHead(AbstractGroup* aGroup = NULL);
		big_scpacket* moveToHeadMsg(AbstractGroup* aGroup = NULL);
		void moveToTail(AbstractGroup* aGroup = NULL);
		big_scpacket* moveToTailMsg(AbstractGroup* aGroup = NULL);
		
		
		/* METHODS NOT IMPLEMENTED
		 moveBeforeMsg { arg aNode;
		 group = aNode.group;
		 ^[18, nodeID, aNode.nodeID];//"/n_before"
		 }
		 moveAfterMsg { arg aNode;
		 group = aNode.group;
		 ^[19, nodeID, aNode.nodeID]; //"/n_after"
		 }
		 moveToHeadMsg { arg aGroup;
		 ^(aGroup ? server.defaultGroup).moveNodeToHeadMsg(this);
		 }
		 moveToTailMsg { arg aGroup;
		 ^(aGroup ? server.defaultGroup).moveNodeToTailMsg(this);
		 }
		 
		 *orderNodesMsg { arg nodes;
		 var msg = [18]; // "/n_after"
		 nodes.doAdjacentPairs { |first, toMoveAfter|
		 msg = msg.add(toMoveAfter.nodeID);
		 msg = msg.add(first.nodeID);
		 };
		 ^msg
		 }
		 
		*/
		
		//hash {  ^server.hash bitXor: nodeID.hash	}
		
		void asUGenInput();
		uint32 asControlInput();
		
		uint32 getNodeID();
		void setNodeID(uint32 _nodeID);
		Server* getServer();
		void setServer(Server* _server);
		AbstractGroup* getGroup();
		void setGroup(AbstractGroup* _group);
		bool getIsPlaying();
		void setIsPlaying(bool _isPlaying);
		bool getIsRunning();
		void setIsRunning(bool _isRunning);
		
		void static initClass();
		static Node* basicNew(Server* _server = NULL, int32 _nodeID = NEGNULL);
		static std::set<int> addActions;
		
	protected:
		
		uint32 nodeID;
		Server* server;
		AbstractGroup* group;
		bool isPlaying, isRunning;
	};
	
	/////////////////
	// AbstractGroup
	////////////////
	
	// common base for Group and ParGroup classes
	class AbstractGroup : public Node {
		
	public:
		
		/** immediately sends **/
		AbstractGroup(Node* target, AddAction addAction = addToHead, uint32 _nodeID = NEGNULL);
		AbstractGroup(Server* target = Server::internal, AddAction addAction = addToHead, uint32 _nodeID = NEGNULL);
		
		virtual ~AbstractGroup(){ }
		
		big_scpacket* newMsg(Node* target, AddAction addAction = addToHead);
		
		void moveNodeToHead(Node* aNode);
		void moveNodeToTail(Node* aNode);
		big_scpacket* moveNodeToHeadMsg(Node* aNode);
		big_scpacket* moveNodeToTailMsg(Node* aNode);
		
		void freeAll();
		big_scpacket* freeAllMsg();
		void deepFree();
		big_scpacket* deepFreeMsg();
		void dumpTree(bool postControls = false);
		big_scpacket* dumpTreeMsg(bool postControls = false);
		// queryTree
		virtual uint32 creationCmd();
		
	private:
		
		void init(Node* target, AddAction addAction);
		
	};
	
	//////////
	// Group
	//////////
	
	class Group : public AbstractGroup {
	
	public:
		
		Group(Node* target, AddAction addAction = addToHead, uint32 _nodeID = NEGNULL);
		Group(Server* target = Server::internal, AddAction addAction = addToHead, uint32 _nodeID = NEGNULL);
		~Group();
		
		static Group* after(Node* aNode);
		static Group* before(Node* aNode);
		static Group* head(AbstractGroup* aGroup);
		static Group* tail(AbstractGroup* aGroup);
		static Group* replace(Node* nodeToReplace);
		
		uint32 creationCmd();
	};
	
	/////////
	// Synth
	/////////
	
	class Synth : public Node {
		
	public:
		
		Synth(const char* _defName, node_arg_list args = node_arg_list());
		
		Synth(const char* _defName, node_arg_list args, Node* target, AddAction addAction = addToHead, 
			  uint32 _nodeID = NEGNULL);
		Synth(const char* _defName, node_arg_list args, Server* target = Server::internal, 
			  AddAction addAction = addToHead, uint32 _nodeID = NEGNULL);
		
		big_scpacket* newMsg(Node* target, node_arg_list args, AddAction addAction = addToHead);
		
		// NOT IMPLEMENTED YET
		//static Synth* newPaused(const char* _defName, node_arg_list args, Node* target, AddAction addAction = addToHead);
		//static Synth* newPaused(const char* _defName, node_arg_list args, Server* target, AddAction addAction = addToHead);
		
		//static Synth* basicNew(const char* _defName, Server* _server, uint32 _nodeID);
		
		static Synth* after(Node* aNode, const char* _defName, node_arg_list args = node_arg_list());
		static Synth* before(Node* aNode, const char* _defName, node_arg_list args = node_arg_list());
		static Synth* head(AbstractGroup* aGroup, const char* _defName, node_arg_list args = node_arg_list());
		static Synth* tail(AbstractGroup* aGroup, const char* _defName, node_arg_list args = node_arg_list());
		static Synth* replace(Node* nodeToReplace, const char* _defName, node_arg_list args = node_arg_list());
		
		big_scpacket* addAfterMsg(Node* aNode, node_arg_list args = node_arg_list());
		big_scpacket* addBeforeMsg(Node* aNode, node_arg_list args = node_arg_list());
		big_scpacket* addToHeadMsg(AbstractGroup* aGroup, node_arg_list args = node_arg_list());
		big_scpacket* addToTailMsg(AbstractGroup* aGroup, node_arg_list args = node_arg_list());
		big_scpacket* addReplaceMsg(Node* aNode, node_arg_list args = node_arg_list());
		
		static void grain(const char* _defName, node_arg_list args, Node* target, 
						  AddAction addAction = addToHead);
		static void grain(const char* _defName, node_arg_list args = node_arg_list(), 
						  Server* target = Server::internal, AddAction addAction = addToHead);
		
		const char* getDefName();
		void setDefName(const char* _defName);
		
	private:
		
		const char* defName;
		void init(const char* _defName, node_arg_list args, Node* target, AddAction addAction);
		static void sendGrain(const char* _defName, node_arg_list args, Server* _server, Node* target, 
							  AddAction addAction);
	};
	
	/////////////
	// RootNode
	/////////////
	
	class RootNode : public Group {
		
	public:
		
		RootNode(Server* _server = Server::internal);
		
		void rninit();

		static void initClass();
		
		void free(bool sendFlag = true);
		void run(bool flag = true);
		
		void moveBefore(Node* aNode);
		void moveAfter(Node* aNode);
		void moveToHead(AbstractGroup* aGroup = NULL);
		void moveToTail(AbstractGroup* aGroup = NULL);
		
		static void freeAll();
		
		static std::map<const char*, RootNode*> roots;
	};
	
	/////////////
	// ParGroup
	////////////
	
	class ParGroup : public AbstractGroup {
		
	public:
		
		ParGroup(Node* target, AddAction addAction = addToHead, uint32 _nodeID = NEGNULL);
		ParGroup(Server* target = Server::internal, AddAction addAction = addToHead, uint32 _nodeID = NEGNULL);
		~ParGroup();
		
		static ParGroup* after(Node* aNode);
		static ParGroup* before(Node* aNode);
		static ParGroup* head(AbstractGroup* aGroup);
		static ParGroup* tail(AbstractGroup* aGroup);
		static ParGroup* replace(Node* nodeToReplace);
		
		uint32 creationCmd();
	};
};

#pragma GCC visibility pop