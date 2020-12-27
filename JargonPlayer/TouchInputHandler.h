#pragma once

#include <sdl/include/SDL.h>

struct mpv_handle;
class VideoWindow;

class TouchInputHandler{
	public:
		TouchInputHandler();
		~TouchInputHandler();

		void handleInput(VideoWindow* videoWindow, mpv_handle *mpv, SDL_Event& event);

	private:

};
