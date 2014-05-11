#include "ITimedNode.h"

#include <stdlib.h>

namespace ai {

#define NOTSTARTED -1

ITimedNode::ITimedNode(const std::string& name, const std::string& parameters, const ConditionPtr& condition) :
		TreeNode(name, parameters, condition), _timerMillis(NOTSTARTED) {
	if (!parameters.empty())
		_millis = ::atol(parameters.c_str());
	else
		_millis = 1000L;
}

ITimedNode::~ITimedNode() {
}

TreeNodeStatus ITimedNode::execute(AI& entity, long deltaMillis) {
	const TreeNodeStatus result = TreeNode::execute(entity, deltaMillis);
	if (result == CANNOTEXECUTE)
		return CANNOTEXECUTE;
	if (_timerMillis == NOTSTARTED) {
		_timerMillis = _millis;
	} else if (_timerMillis - deltaMillis > 0) {
		_timerMillis -= deltaMillis;
	} else {
		_timerMillis = NOTSTARTED;
		return state(entity, FINISHED);
	}

	return state(entity, executeTimed(entity, deltaMillis));
}

}
