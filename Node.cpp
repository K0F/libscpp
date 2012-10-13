/*
 *  Node.cpp
 *  SC++
 *
 *  Created by Chad McKinney on 2/22/12.
 *
 */

#include "Node.h"

namespace sc {
	
	/////////
	// Node
	/////////
	
	Node::Node(Server* _server, uint32 _nodeID) :
		server(_server)
	{
		if(server == NULL)
			scprintf("Error. Node: Internal Server is null");
		if(_nodeID == NEGNULL)
			nodeID = server->nextNodeID();
		else
			nodeID = _nodeID;
	}
		
	void Node::free(bool sendFlag)
	{
		if(sendFlag)
		{
			big_scpacket* packet = freeMsg();
			server->sendMsg(packet);
			delete packet;
			packet = NULL;
		}
		group = NULL;
		setIsPlaying(false);
		setIsRunning(false);
	}
	
	big_scpacket* Node::freeMsg()
	{
		big_scpacket* packet = new big_scpacket();
		packet->BeginMsg();
		
		packet->addi(n_free);
		packet->maketags(2);
		packet->addtag(',');
		packet->addtag('i');
		packet->addi(nodeID);
		
		packet->EndMsg();
		
		return packet;
	}
	
	void Node::run(bool flag)
	{
		big_scpacket* packet = runMsg(flag);
		server->sendMsg(packet);
		delete packet;
		packet = NULL;
	}
	
	big_scpacket* Node::runMsg(bool flag)
	{
		big_scpacket* packet = new big_scpacket();
		packet->BeginMsg();
		
		packet->addi(n_run);
		packet->maketags(3);
		packet->addtag(',');
		packet->addtag('i');
		packet->addi(nodeID);
		packet->addtag('i');
		packet->addi(binaryValue(flag));
		
		packet->EndMsg();
		
		return packet;
	}
	
	void Node::set(const char* _argName, float64 _argVal)
	{
		big_scpacket packet;
		packet.BeginMsg();
		
		packet.addi(n_set);
		packet.maketags(4);
		packet.addtag(',');
		packet.addtag('i');
		packet.addi(nodeID);
		packet.addtag('s');
		packet.adds(_argName);
		packet.addtag('f');
		packet.addf(_argVal);
		
		packet.EndMsg();
		server->sendMsg(&packet);
	}
	
	void Node::set(node_arg_list args)
	{
		big_scpacket* packet = setMsg(args);
		server->sendMsg(packet);
		delete packet;
		packet = NULL;
	}
	
	big_scpacket* Node::setMsg(node_arg_list args)
	{
		uint32 numArgs = args.size();
		
		big_scpacket* packet = new big_scpacket();
		packet->BeginMsg();
		
		packet->addi(n_set);
		packet->maketags(2 + (numArgs * 2));
		packet->addtag(',');
		packet->addtag('i');
		packet->addi(nodeID);
		
		try 
		{
			node_arg_list::iterator argIter = args.begin();
			while(argIter != args.end())
			{
				packet->addtag('s');
				packet->adds(argIter->arg_name);
				packet->addtag('f');
				packet->addf(argIter->arg_val);
				++argIter;
			}
		} 
		
		catch (std::runtime_error & e) 
		{
			scprintf("Error. Server: sendMsg failed");
			//error("makeSynthMsgWithTags: %s\n", e.what());
			return NULL;
		}
		
		packet->EndMsg();
		return packet;
	}
	
	void Node::setn(uint32 firstIndex, uint32 numIndexes, std::vector<float64> args)
	{
		//server->sendMsg(n_setn, nodeID, firstIndex, numIndexes, args);
	}
	
	void Node::fill(const char* controlName, uint32 numControls, std::vector<float64> args)
	{
		//server->sendMsg(n_fill, nodeID, controlName, numControls, args);
	}
	
	void Node::release(float64 releaseTime)
	{
		set(ARGS("gate", -1.0 - releaseTime));
	}
	
	void Node::trace()
	{
		big_scpacket* packet = traceMsg();
		server->sendMsg(packet);
		delete packet;
		packet = NULL;
	}
	
	big_scpacket* Node::traceMsg()
	{
		big_scpacket* packet = new big_scpacket();
		packet->BeginMsg();
		
		packet->addi(n_trace);
		packet->maketags(2);
		packet->addtag(',');
		packet->addtag('i');
		packet->addi(nodeID);
		
		packet->EndMsg();
		
		return packet;
	}
	
	void Node::query()
	{
		// NOT IMPLEMENTED
	}
	
	void Node::registerNode(bool assumePlaying)
	{
		// NOT IMPLEMENTED
	}
	
	void Node::onFree(OnFreeHandler* func)
	{
		// NOT IMPLEMENTED
	}
	
	void Node::waitForFree()
	{
		// NOT IMPLEMENTED
	}
	
	void Node::moveBefore(Node* aNode)
	{
		group = aNode->getGroup();
		
		big_scpacket* packet = moveBeforeMsg(aNode);
		server->sendMsg(packet);
		delete packet;
		packet = NULL;
	}
	
	big_scpacket* Node::moveBeforeMsg(Node* aNode)
	{
		big_scpacket* packet = new big_scpacket();
		packet->BeginMsg();
		
		packet->addi(n_before);
		packet->maketags(3);
		packet->addtag(',');
		packet->addtag('i');
		packet->addi(nodeID);
		packet->addtag('i');
		packet->addi(aNode->getNodeID());
		
		packet->EndMsg();
		
		return packet;
	}
	
	void Node::moveAfter(Node* aNode)
	{
		group = aNode->getGroup();
		
		big_scpacket* packet = moveBeforeMsg(aNode);
		server->sendMsg(packet);
		delete packet;
		packet = NULL;
	}
	
	big_scpacket* Node::moveAfterMsg(Node* aNode)
	{
		big_scpacket* packet = new big_scpacket();
		packet->BeginMsg();
		
		packet->addi(n_after);
		packet->maketags(3);
		packet->addtag(',');
		packet->addtag('i');
		packet->addi(nodeID);
		packet->addtag('i');
		packet->addi(aNode->getNodeID());
		
		packet->EndMsg();
		
		return packet;
	}
	
	void Node::moveToHead(AbstractGroup* aGroup)
	{
		if(aGroup == NULL)
			Server::defaultGroup->moveNodeToHead(this);
		else
			aGroup->moveNodeToHead(this);
	}
	
	big_scpacket* Node::moveToHeadMsg(AbstractGroup* aGroup)
	{
		if(aGroup == NULL)
			return Server::defaultGroup->moveNodeToHeadMsg(this);
		else
			return aGroup->moveNodeToHeadMsg(this);
	}
	
	void Node::moveToTail(AbstractGroup* aGroup)
	{
		if(aGroup == NULL)
			Server::defaultGroup->moveNodeToTail(this);
		else
			aGroup->moveNodeToTail(this);
	}
	
	big_scpacket* Node::moveToTailMsg(AbstractGroup* aGroup)
	{
		if(aGroup == NULL)
			return Server::defaultGroup->moveNodeToTailMsg(this);
		else
			return aGroup->moveNodeToTailMsg(this);
	}
	
	void Node::asUGenInput()
	{
		std::cerr << "Should not use a Node inside a SynthDef";
	}
	
	uint32 Node::asControlInput()
	{
		return this->nodeID;
	}
	
	uint32 Node::getNodeID()
	{
		return this->nodeID;
	}
	
	void Node::setNodeID(uint32 _nodeID)
	{
		this->nodeID = _nodeID;
	}
	
	Server* Node::getServer()
	{
		return this->server;
	}
	
	void Node::setServer(Server* _server)
	{
		this->server = _server;
	}
	
	AbstractGroup* Node::getGroup()
	{
		return this->group;
	}
	
	void Node::setGroup(AbstractGroup* _group)
	{
		this->group = _group;
	}
	
	bool Node::getIsPlaying()
	{
		return this->isPlaying;
	}
	
	void Node::setIsPlaying(bool _isPlaying)
	{
		this->isPlaying = _isPlaying;
	}
	
	bool Node::getIsRunning()
	{
		return this->isRunning;
	}
	
	void Node::setIsRunning(bool _isRunning)
	{
		this->isRunning = _isRunning;
	}
	
	void Node::initClass()
	{
		
	}
	
	Node* Node::basicNew(Server* _server, int32 _nodeID)
	{
		if(_server == NULL)
			_server = Server::internal;
		if(_nodeID == NEGNULL)
			_nodeID = _server->nextNodeID();
		return new Node(_server, _nodeID);
	}
	
	/////////////////
	// AbstractGroup
	/////////////////
	
	/** immediately sends **/
	AbstractGroup::AbstractGroup(Node* target, AddAction addAction, uint32 _nodeID) :
		Node(target->getServer(), _nodeID)
	{
		if(_nodeID != 0) // if not a root node
			init(target, addAction);
	}
	
	/** immediately sends **/
	AbstractGroup::AbstractGroup(Server* target, AddAction addAction, uint32 _nodeID) :
		Node(target, _nodeID)
	{
		if(_nodeID != 0) // if not a root node
			init(target->getDefaultGroup(), addAction);
	}
	
	void AbstractGroup::init(Node* target, AddAction addAction)
	{
		if(addAction < 2)
			group = (AbstractGroup*) target;
		else
			group = target->getGroup();
		
		//server->sendMsg(this->creationCmd(), nodeID, addAction, target->getNodeID());
		big_scpacket* packet = newMsg(target, addAction);
		server->sendMsg(packet);
		delete packet;
		packet = NULL;
	}
	
	big_scpacket* AbstractGroup::newMsg(Node* target, AddAction addAction)
	{
		big_scpacket* packet = new big_scpacket();
		packet->BeginMsg();
		
		packet->addi(creationCmd());
		packet->maketags(4);
		packet->addtag(',');
		packet->addtag('i');
		packet->addi(nodeID);
		packet->addtag('i');
		packet->addi(addAction);
		packet->addtag('i');
		packet->addi(target->getNodeID());
		
		packet->EndMsg();
		return packet;
	}
	
	void AbstractGroup::moveNodeToHead(Node* aNode)
	{
		big_scpacket* packet = moveNodeToHeadMsg(aNode);
		server->sendMsg(packet);
		delete packet;
		packet = NULL;
	}
	
	void AbstractGroup::moveNodeToTail(Node* aNode)
	{
		big_scpacket* packet = moveNodeToTailMsg(aNode);
		server->sendMsg(packet);
		delete packet;
		packet = NULL;
	}
	
	big_scpacket* AbstractGroup::moveNodeToHeadMsg(Node* aNode)
	{
		aNode->setGroup(this);
		
		big_scpacket* packet = new big_scpacket();
		packet->BeginMsg();
		
		packet->addi(g_head);
		packet->maketags(3);
		packet->addtag(',');
		packet->addtag('i');
		packet->addi(nodeID);
		packet->addtag('i');
		packet->addi(aNode->getNodeID());
		
		packet->EndMsg();
		
		return packet;
	}
	
	big_scpacket* AbstractGroup::moveNodeToTailMsg(Node* aNode)
	{
		aNode->setGroup(this);
		
		big_scpacket* packet = new big_scpacket();
		packet->BeginMsg();
		
		packet->addi(g_tail);
		packet->maketags(3);
		packet->addtag(',');
		packet->addtag('i');
		packet->addi(nodeID);
		packet->addtag('i');
		packet->addi(aNode->getNodeID());
		
		packet->EndMsg();
		
		return packet;
	}
	
	void AbstractGroup::freeAll()
	{		
		big_scpacket* packet = freeAllMsg();
		server->sendMsg(packet);
		delete packet;
		packet = NULL;
	}
	
	big_scpacket* AbstractGroup::freeAllMsg()
	{		
		big_scpacket* packet = new big_scpacket();
		packet->BeginMsg();
		
		packet->addi(g_freeAll);
		packet->maketags(2);
		packet->addtag(',');
		packet->addtag('i');
		packet->addi(nodeID);
		
		packet->EndMsg();
		
		return packet;
	}
	
	void AbstractGroup::deepFree()
	{
		big_scpacket* packet = deepFreeMsg();
		server->sendMsg(packet);
		delete packet;
		packet = NULL;
	}
							  
	big_scpacket* AbstractGroup::deepFreeMsg()
	{
		big_scpacket* packet = new big_scpacket();
		packet->BeginMsg();
		
		packet->addi(g_deepFree);
		packet->maketags(2);
		packet->addtag(',');
		packet->addtag('i');
		packet->addi(nodeID);
		
		packet->EndMsg();
		
		return packet;
	}
	
	void AbstractGroup::dumpTree(bool postControls)
	{
		big_scpacket* packet = dumpTreeMsg();
		server->sendMsg(packet);
		delete packet;
		packet = NULL;
	}
	
	big_scpacket* AbstractGroup::dumpTreeMsg(bool postControls)
	{
		big_scpacket* packet = new big_scpacket();
		packet->BeginMsg();
		
		packet->addi(g_dumpTree);
		packet->maketags(3);
		packet->addtag(',');
		packet->addtag('i');
		packet->addi(nodeID);
		packet->addtag('i');
		packet->addi(binaryValue(postControls));
		
		packet->EndMsg();
		
		return packet;
	}
	
	uint32 AbstractGroup::creationCmd()
	{ 
		return 21; 
	}
	
	//////////
	// Group
	//////////
	
	Group::Group(Node* target, AddAction addAction, uint32 _nodeID) :
		AbstractGroup(target, addAction, _nodeID)
	{
		
	}
	
	Group::Group(Server* target, AddAction addAction, uint32 _nodeID) :
		AbstractGroup(target, addAction, _nodeID)
	{
		
	}
	
	Group::~Group()
	{
		
	}
	
	Group* Group::after(Node* aNode)
	{
		return new Group(aNode, addAfter);
	}
	
	Group* Group::before(Node* aNode)
	{
		return new Group(aNode, addBefore);
	}
	
	Group* Group::head(AbstractGroup* aGroup)
	{
		return new Group(aGroup, addToHead);
	}
	
	Group* Group::tail(AbstractGroup* aGroup)
	{
		return new Group(aGroup, addToTail);
	}
	
	Group* Group::replace(Node* nodeToReplace)
	{
		return new Group(nodeToReplace, addReplace, nodeToReplace->getNodeID());
	}
	
	uint32 Group::creationCmd()
	{ 
		return 21; 
	}
	
	/////////////
	// Synth
	////////////
	
	/*
	Synth::Synth(const char* _defName, node_arg_list args, Node* target, AddAction addAction, uint32 _nodeID) :
	Node(target->getServer(), _nodeID)
	{
		init(_defName, args, target, addAction);
	}*/
	
	Synth::Synth(const char* _defName, node_arg_list args) :
		Node(Server::internal)
	{
		init(_defName, args, Server::internal->getDefaultGroup(), addToHead);
	}
	
	Synth::Synth(const char* _defName, node_arg_list args, Node* target, AddAction addAction, uint32 _nodeID) :
		Node(target->getServer(), _nodeID)
	{
		init(_defName, args, target, addAction);
	}
	
	Synth::Synth(const char* _defName, node_arg_list args, Server* target, AddAction addAction, uint32 _nodeID) :
		Node(target, _nodeID)
	{
		init(_defName, args, target->getDefaultGroup(), addAction);
	}
	
	void Synth::init(const char* _defName, node_arg_list args, Node* target, AddAction addAction)
	{
		this->defName = _defName;
		
		if(addAction < 2)
			group = (AbstractGroup*) target;
		else
			group = target->getGroup();
		
		//server->sendMsg(s_new, _defName, nodeID, addAction, target->getNodeID(), args);
		big_scpacket* packet = newMsg(target, args, addAction);
		server->sendMsg(packet);
		delete packet;
		packet = NULL;
	}
	
	big_scpacket* Synth::newMsg(Node* target, node_arg_list args, AddAction addAction)
	{
		uint32 numArgs = args.size();
		//std::cout << "Num Args: " << numArgs << std::endl; 
		big_scpacket* packet = new big_scpacket();
		packet->BeginMsg();
		
		packet->addi(s_new);
		packet->maketags(5 + (numArgs * 2));
		packet->addtag(',');
		packet->addtag('s');
		packet->adds(defName);
		packet->addtag('i');
		packet->addi(nodeID);
		packet->addtag('i');
		packet->addi(addAction);
		packet->addtag('i');
		packet->addi(target->getNodeID());
		
		
		try 
		{
			node_arg_list::iterator argIter = args.begin();
			while(argIter != args.end())
			{
				packet->addtag('s');
				packet->adds(argIter->arg_name);
				packet->addtag('f');
				packet->addf(argIter->arg_val);
				//std::cout << argIter->first << ", " << argIter->second << std::endl;
				++argIter;
			}
		} 
		
		catch (std::runtime_error & e) 
		{
			scprintf("Error. Server: sendMsg failed");
			//error("makeSynthMsgWithTags: %s\n", e.what());
			return NULL;
		}
		
		packet->EndMsg();
		return packet;
	}
	
	/*
	Synth* Synth::newPaused(const char* _defName, node_arg_list args, Node* target, AddAction addAction)
	{
		// NOT IMPLEMENTED YET
	}
	
	Synth* Synth::newPaused(const char* _defName, node_arg_list args, Server* target, AddAction addAction)
	{
		// NOT IMPLEMENTED YET
	}
	
	// does not send (used for bundling
	Synth* Synth::basicNew(const char* _defName, Server* _server, uint32 _nodeID)
	{
		
	}*/
	
	Synth* Synth::after(Node* aNode, const char* _defName, node_arg_list args)
	{
		return new Synth(_defName, args, aNode, addAfter);
	}
	
	Synth* Synth::before(Node* aNode, const char* _defName, node_arg_list args)
	{
		return new Synth(_defName, args, aNode, addBefore);
	}
	
	Synth* Synth::head(AbstractGroup* aGroup, const char* _defName, node_arg_list args)
	{
		return new Synth(_defName, args, aGroup, addToHead);
	}
	
	Synth* Synth::tail(AbstractGroup* aGroup, const char* _defName, node_arg_list args)
	{
		return new Synth(_defName, args, aGroup, addToTail);
	}
	
	Synth* Synth::replace(Node* nodeToReplace, const char* _defName, node_arg_list args)
	{
		return new Synth(_defName, args, nodeToReplace, addReplace);
	}
	
	big_scpacket* Synth::addAfterMsg(Node* aNode, node_arg_list args)
	{
		return newMsg(aNode, args, addAfter);
	}
	
	big_scpacket* Synth::addBeforeMsg(Node* aNode, node_arg_list args)
	{
		return newMsg(aNode, args, addBefore);
	}
	
	big_scpacket* Synth::addToTailMsg(AbstractGroup* aGroup, node_arg_list args)
	{
		return newMsg(aGroup, args, addToTail);
	}
	
	big_scpacket* Synth::addToHeadMsg(AbstractGroup* aGroup, node_arg_list args)
	{
		return newMsg(aGroup, args, addToHead);
	}
	
	big_scpacket* Synth::addReplaceMsg(Node* aNode, node_arg_list args)
	{
		return newMsg(aNode, args, addReplace);
	}
	
	void Synth::grain(const char* _defName, node_arg_list args, Node* target, AddAction addAction)
	{
		sendGrain(_defName, args, target->getServer(), target, addAction);
	}
	
	void Synth::grain(const char* _defName, node_arg_list args, Server* target, AddAction addAction)
	{
		sendGrain(_defName, args, target, target->getDefaultGroup(), addAction);
	}
	
	void Synth::sendGrain(const char* _defName, node_arg_list args, Server* _server, Node* target, AddAction addAction)
	{
		//_server->sendMsg(s_new, _defName, -1, addAction, target->getNodeID(), args);
		
		uint32 numArgs = args.size();
		//std::cout << "Num Args: " << numArgs << std::endl; 
		big_scpacket* packet = new big_scpacket();
		packet->BeginMsg();
		
		packet->addi(s_new);
		packet->maketags(5 + (numArgs * 2));
		packet->addtag(',');
		packet->addtag('s');
		packet->adds(_defName);
		packet->addtag('i');
		packet->addi(-1);
		packet->addtag('i');
		packet->addi(addAction);
		packet->addtag('i');
		packet->addi(target->getNodeID());
		
		
		try 
		{
			node_arg_list::iterator argIter = args.begin();
			while(argIter != args.end())
			{
				packet->addtag('s');
				packet->adds(argIter->arg_name);
				packet->addtag('f');
				packet->addf(argIter->arg_val);
				//std::cout << argIter->first << ", " << argIter->second << std::endl;
				++argIter;
			}
		} 
		
		catch (std::runtime_error & e) 
		{
			scprintf("Error. Server: sendMsg failed");
			//error("makeSynthMsgWithTags: %s\n", e.what());
			return;
		}
		
		packet->EndMsg();
		_server->sendMsg(packet);
		delete packet;
		packet = NULL;
	}
	
	const char* Synth::getDefName()
	{
		return this->defName;
	}
	
	void Synth::setDefName(const char* _defName)
	{
		this->defName = _defName;
	}

	
	/////////////
	// RootNode
	/////////////
	
	std::map<const char*, RootNode*> RootNode::roots;
	
	RootNode::RootNode(Server* _server) :
		Group(_server, addToHead, 0)
	{
		
		rninit();
	}
	
	void RootNode::rninit()
	{
		RootNode::roots[this->server->getName()] = this;
		setIsPlaying(true);
		setGroup(this); // self
	}
	
	void RootNode::initClass()
	{
		
	}
	
	void RootNode::free(bool sendFlag)
	{
		std::cout << "free has no effect on RootNode" << std::endl;
	}
	
	void RootNode::run(bool flag)
	{
		std::cout << "run has no effect on RootNode" << std::endl;
	}
	
	void RootNode::moveBefore(Node* aNode)
	{
		std::cout << "moveBefore has no effect on RootNode" << std::endl;
	}
	
	void RootNode::moveAfter(Node* aNode)
	{
		std::cout << "moveAfter has no effect on RootNode" << std::endl;
	}
	
	void RootNode::moveToHead(AbstractGroup* aGroup)
	{
		std::cout << "moveToHead has no effect on RootNode" << std::endl;
	}
	
	void RootNode::moveToTail(AbstractGroup* aGroup)
	{
		std::cout << "moveToTail has no effect on RootNode" << std::endl;
	}
	
	void RootNode::freeAll()
	{
		std::map<const char*, RootNode*>::iterator iter = RootNode::roots.begin();
		while(iter != RootNode::roots.end())
		{
			iter->second->freeAll();
			++iter;
		}
	}
		
	////////////
	// ParGroup
	////////////
	
	ParGroup::ParGroup(Node* target, AddAction addAction, uint32 _nodeID) :
		AbstractGroup(target, addAction, _nodeID)
	{
		
	}
	
	ParGroup::ParGroup(Server* target, AddAction addAction, uint32 _nodeID) :
		AbstractGroup(target, addAction, _nodeID)
	{
		
	}
	
	ParGroup::~ParGroup()
	{
		
	}
	
	ParGroup* ParGroup::after(Node* aNode)
	{
		return new ParGroup(aNode, addAfter);
	}
	
	ParGroup* ParGroup::before(Node* aNode)
	{
		return new ParGroup(aNode, addBefore);
	}
	
	ParGroup* ParGroup::head(AbstractGroup* aGroup)
	{
		return new ParGroup(aGroup, addToHead);
	}
	
	ParGroup* ParGroup::tail(AbstractGroup* aGroup)
	{
		return new ParGroup(aGroup, addToTail);
	}
	
	ParGroup* ParGroup::replace(Node* nodeToReplace)
	{
		return new ParGroup(nodeToReplace, addReplace, nodeToReplace->getNodeID());
	}
	
	uint32 ParGroup::creationCmd()
	{ 
		return 63; 
	}
}
