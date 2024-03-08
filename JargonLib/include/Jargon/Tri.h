#ifndef JARGON_TRI_H
#define JARGON_TRI_H

namespace Jargon {
	enum class TriState {
		False,
		True,
		Unknown
	};

	class Tri {
	public:
		static const Tri True;
		static const Tri False;
		static const Tri Unknown;

		Tri();
		Tri(bool value);

		bool isTrue() const;
		bool isFalse() const;
		bool isUnknown() const;

		void setState(TriState state);
		void setState(bool value);
		void setUnknown();

		TriState getState() const;

		bool operator==(bool value) const {
			return value ? isTrue() : isFalse();
		}

		Tri& operator=(bool value) {
			setState(value);
			return *this;
		}
	private:
		TriState state;
	};
}

#endif