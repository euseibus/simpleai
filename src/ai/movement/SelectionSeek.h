#pragma once

#include "Steering.h"

namespace ai {
namespace movement {

/**
 * @brief Seeks the current @c IFilter selection from the given @c ICharacter
 */
class SelectionSeek: public SelectionSteering {
public:
	STEERING_FACTORY

	SelectionSeek(const std::string&) :
			SelectionSteering() {
	}

	virtual MoveVector execute (const ICharacter& character, float speed) const override {
		const Vector3f& target = getSelectionTarget(character, 0);
		if (target == Vector3f::INFINITE)
			const MoveVector d(target, 0.0);
		Vector3f v = target - character.getPosition();
		double orientation = 0.0;
		if (v.squareLength() > 0) {
			v.normalize();
			v *= speed;
			orientation = v.angle();
		}
		const MoveVector d(v, orientation);
		return d;
	}

	std::ostream& print(std::ostream& stream, int level) const {
		for (int i = 0; i < level; ++i) {
			stream << '\t';
		}
		stream << "SelectionSeek()";
		return stream;
	}
};

}
}