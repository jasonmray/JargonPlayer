#include "QuadrantLayout.h"

namespace QuadrantLayout{
	Rect buildRectForQuadrant(WindowQuadrant q, int width, int height){
		switch(q){
			case WindowQuadrant::TopLeft:
				return Rect{0, 0, width / 2, height / 2};
			case WindowQuadrant::TopRight:
				return Rect{0, width / 2, width / 2, height / 2};
			case WindowQuadrant::BottomLeft:
				return Rect{height / 2, 0, width / 2, height / 2};
			case WindowQuadrant::BottomRight:
				return Rect{height / 2, width / 2, width / 2, height / 2};
			case WindowQuadrant::Center:
			default:
				return Rect{height / 4, width / 4, width / 2, height / 2};
		}
	}

	Bounds rectToBounds(const Rect& r) {
		Bounds b;
		b.x1 = r.left;
		b.x2 = r.left + r.width;
		b.y1 = r.top;
		b.y2 = r.top + r.height;

		return b;
	}
}