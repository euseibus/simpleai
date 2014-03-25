#pragma once

#include <AI.h>
#include <tree/TreeNode.h>
#include "Network.h"
#include "SelectHandler.h"
#include "PauseHandler.h"

namespace ai {

class AIStateNode;

class Server {
protected:
	typedef std::map<CharacterId, AI*> AIMap;
	typedef AIMap::const_iterator AIMapConstIter;
	typedef AIMap::iterator AIMapIter;
	AIMap _ais;
	Network _network;
	CharacterId _selectedCharacterId;
	uint32_t _time;
	SelectHandler _selectHandler;
	PauseHandler _pauseHandler;
	bool _pause;

	void addChildren(const TreeNodePtr& node, AIStateNode& parent, AI& ai) const;
	void broadcastState();
	void broadcastCharacterDetails();
public:
	Server(int port = 10001, const std::string& hostname = "0.0.0.0");
	virtual ~Server();

	bool start();

	void select(const ClientId& clientId, const CharacterId& id);
	void pause(const ClientId& clientId, bool pause);

	// call then when you spawn a new @code AI that should be traceable via the debug viewer
	bool addAI(AI& ai);
	bool removeAI(AI& ai);
	// call this to update the server - should get called somewhere from your game tick
	void update(uint32_t deltaTime);
};

}
