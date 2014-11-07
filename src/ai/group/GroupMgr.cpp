#include "GroupMgr.h"
#include <numeric>

namespace ai {

GroupMgr::GroupMgr() {
}

GroupMgr::~GroupMgr() {
}

bool GroupMgr::add(GroupId id, ICharacter* character) {
	Lock lock(_mutex);
	const GroupMembersIter& i = _members.find(id);
	if (i != _members.end()) {
		const std::pair<GroupMembersSetIter, bool>& ret = i->second.insert(character);
		return ret.second;
	}

	GroupMembersSet set;
	const GroupMembersIter& iInsert = _members.insert(std::pair<GroupId, GroupMembersSet>(id, set)).first;
	const std::pair<GroupMembersSetIter, bool>& ret = iInsert->second.insert(character);
	return ret.second;
}

bool GroupMgr::remove(GroupId id, ICharacter* character) {
	Lock lock(_mutex);
	const GroupMembersIter& i = _members.find(id);
	if (i == _members.end())
		return false;
	const GroupMembersSetIter& si = i->second.find(character);
	if (si == i->second.end())
		return false;
	i->second.erase(si);
	if (i->second.empty())
		_members.erase(i);
	return true;
}

Vector3f GroupMgr::getPosition(GroupId id) const {
	Lock lock(_mutex);
	const GroupMembersConstIter& i = _members.find(id);
	if (i == _members.end())
		return Vector3f::ZERO;

	Vector3f averagePosition = std::accumulate(i->second.begin(), i->second.end(), Vector3f(), AveragePositionFunctor());
	averagePosition *= 1.0f / (float) i->second.size();
	return averagePosition;
}

std::pair<GroupMembersSetIter, GroupMembersSetIter> GroupMgr::getGroupMembers(GroupId id) const {
	Lock lock(_mutex);
	const GroupMembersConstIter& i = _members.find(id);
	if (i == _members.end()) {
		return std::pair<GroupMembersSetIter, GroupMembersSetIter>(_empty.begin(), _empty.end());
	}
	return std::pair<GroupMembersSetIter, GroupMembersSetIter>(i->second.begin(), i->second.end());
}

bool GroupMgr::isGroupLeader(GroupId id, const ICharacter& character) const {
	const std::pair<GroupMembersSetIter, GroupMembersSetIter>& members = getGroupMembers(id);
	if (members.first == members.second)
		return false;
	if ((*members.first)->getId() == character.getId())
		return true;
	return false;
}

int GroupMgr::getGroupSize(GroupId id) const {
	const std::pair<GroupMembersSetIter, GroupMembersSetIter>& members = getGroupMembers(id);
	return static_cast<int>(std::distance(members.first, members.second));
}

bool GroupMgr::isInAnyGroup(const ICharacter& character) const {
	Lock lock(_mutex);
	for (GroupMembersConstIter i = _members.begin(); i != _members.end(); ++i) {
		const GroupMembersSetConstIter& it = i->second.find(const_cast<ICharacter*>(&character));
		if (it != i->second.end())
			return true;
	}
	return false;
}

bool GroupMgr::isInGroup(GroupId id, const ICharacter& character) const {
	const std::pair<GroupMembersSetIter, GroupMembersSetIter>& members = getGroupMembers(id);
	for (GroupMembersSetIter i = members.first; i != members.second; ++i) {
		if ((*i)->getId() == character.getId())
			return true;
	}
	return false;
}

}
