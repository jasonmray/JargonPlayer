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

	Rect buildRectForQuadrant(WindowQuadrant q, int width, int height);

}