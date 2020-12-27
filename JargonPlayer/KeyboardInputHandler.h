#pragma once

#include <sdl/include/SDL.h>

struct mpv_handle;
class VideoWindow;

class KeyboardInputHandler{
	public:
		KeyboardInputHandler();
		~KeyboardInputHandler();

		void handleInput(VideoWindow* videoWindow, mpv_handle *mpv, SDL_Event& event);

	private:
		void handleKeydown(VideoWindow* videoWindow, mpv_handle *mpv, SDL_Event& event);
};
