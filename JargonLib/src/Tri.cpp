#include "Jargon/Tri.h"

namespace Jargon {

	const Tri Tri::True(true);
	const Tri Tri::False(false);
	const Tri Tri::Unknown;

	Tri::Tri() {
		state = TriState::Unknown;
	}

	Tri::Tri(bool value) {
		state = TriState::Unknown;
	}

	bool Tri::isTrue() const {
		return state == TriState::True;
	}

	bool Tri::isFalse() const {
		return state == TriState::False;
	}

	bool Tri::isUnknown() const {
		return state == TriState::Unknown;
	}

	void Tri::setState(TriState state) {
		this->state = state;
	}

	void Tri::setState(bool value) {
		state = value ? TriState::True : TriState::False;
	}

	void Tri::setUnknown() {
		state = TriState::Unknown;
	}

	TriState Tri::getState() const {
		return state;
	}
}
