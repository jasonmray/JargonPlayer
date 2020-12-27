#pragma once

#include <sdl/include/SDL.h>

struct mpv_handle;
class VideoWindow;

class GamepadInputHandler{
	public:
		GamepadInputHandler();
		~GamepadInputHandler();

		void handleInput(VideoWindow* videoWindow, mpv_handle *mpv, SDL_Event& event);

	private:
		void handleControllerButton(VideoWindow* videoWindow, mpv_handle *mpv, SDL_Event& event);
		void handleControllerAxis(VideoWindow* videoWindow, mpv_handle *mpv, SDL_Event& event);
};
