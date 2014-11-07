#include <AI.h>
#include <aggro/AggroMgr.h>
#include <algorithm>

namespace ai {

AggroMgr::AggroMgr(std::size_t expectedEntrySize) :
		_dirty(false) {
	if (expectedEntrySize > 0)
		_entries.reserve(expectedEntrySize);
}

AggroMgr::~AggroMgr() {
}

void AggroMgr::cleanupList() {
	Lock lock(_mutex);
	Entries::iterator::difference_type remove = 0;
	for (Entries::iterator i = _entries.begin(); i != _entries.end(); ++i) {
		const float aggroValue = (*i)->getAggro();
		if (aggroValue > 0.0f)
			break;

		++remove;
	}

	if (remove == 0)
		return;

	const int size = static_cast<int>(_entries.size());
	if (size == remove)
		_entries.clear();
	else
		_entries.erase(_entries.begin(), _entries.begin() + remove);
}

void AggroMgr::update(long deltaMillis) {
	Lock lock(_mutex);
	for (EntriesIter i = _entries.begin(); i != _entries.end(); ++i) {
		_dirty |= (*i)->reduceByTime(deltaMillis);
	}

	if (_dirty) {
		sort();
		cleanupList();
	}
}

class CharacterIdPredicate {
private:
	const CharacterId& _id;
public:
	CharacterIdPredicate(const CharacterId& id) :
			_id(id) {
	}

	bool operator()(const EntryPtr &n1) {
		return n1->getCharacterId() == _id;
	}
};

static bool EntrySorter(const EntryPtr& a, const EntryPtr& b) {
	if (a->getAggro() > b->getAggro())
		return false;
	if (::fabs(a->getAggro() - b->getAggro()) < 0.0000001f)
		return a->getCharacterId() < b->getCharacterId();
	return true;
}

inline void AggroMgr::sort() {
	if (!_dirty)
		return;
	Lock lock(_mutex);
	std::sort(_entries.begin(), _entries.end(), EntrySorter);
	_dirty = false;
}

Entry* AggroMgr::addAggro(AI& entity, float amount) {
	const CharacterId id = entity.getCharacter().getId();
	const CharacterIdPredicate p(id);
	Lock lock(_mutex);
	Entries::const_iterator i = std::find_if(_entries.begin(), _entries.end(), p);
	if (i == _entries.end()) {
		Entry* e = new Entry(id, amount);
		const EntryPtr newEntry(e);
		_entries.push_back(newEntry);
		_dirty = true;
		return e;
	}

	(*i)->addAggro(amount);
	_dirty = true;
	return i->get();
}

EntryPtr AggroMgr::getHighestEntry() {
	if (_entries.empty())
		return EntryPtr();

	sort();

	return _entries.back();
}

}
