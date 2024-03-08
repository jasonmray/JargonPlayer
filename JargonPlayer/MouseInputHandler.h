#pragma once

#include <sdl/include/SDL.h>

struct mpv_handle;
class VideoWindow;

class MouseInputHandler{
	public:
		MouseInputHandler();
		~MouseInputHandler();

		void handleInput(VideoWindow* videoWindow, mpv_handle *mpv, SDL_Event& event);

	private:
		int previousMouseX = 0;
		int previousMouseY = 0;
};
