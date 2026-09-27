#pragma once

#include <SDL.h>

struct mpv_handle;
class VideoWindow;

class MouseInputHandler{
	public:
		MouseInputHandler();
		~MouseInputHandler();

		void handleInput(VideoWindow* videoWindow, mpv_handle *mpv, SDL_Event& event);

	private:
		bool dragging = false;
		int dragStartClientX = 0;  // location of window drag start, in client coordinates
		int dragStartClientY = 0;
};
