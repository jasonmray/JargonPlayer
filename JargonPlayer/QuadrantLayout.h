#pragma once

namespace QuadrantLayout{
	enum class WindowQuadrant{
		TopLeft,
		TopRight,
		BottomLeft,
		BottomRight,
		Center
	};

	struct Rect{
		int top;
		int left;
		int width;
		int height;
	};

	struct Bounds {
		int x1;
		int x2;
		int y1;
		int y2;
	};

	Rect buildRectForQuadrant(WindowQuadrant q, int width, int height);

	Bounds rectToBounds(const Rect& r);

}