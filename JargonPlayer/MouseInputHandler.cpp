#include "MouseInputHandler.h"
#include "MpvCommands.h"
#include "VideoWindow.h"

#include "Jargon/StringUtilities.h"

#include <libmpv/include/client.h>


MouseInputHandler::MouseInputHandler(){
}

MouseInputHandler::~MouseInputHandler(){
}

void MouseInputHandler::handleInput(VideoWindow* videoWindow, mpv_handle *mpv, SDL_Event& event) {
	switch (event.type) {
		case SDL_MOUSEMOTION:
		{
			mpv_command(mpv, MpvCommands::ShowProgressTime);
			mpv_command(mpv, MpvCommands::ShowProgressBar);
		}
		break;
		case SDL_MOUSEBUTTONDOWN:
		{
			if (event.button.clicks == 2) {
				videoWindow->enterFullscreen();
			}
		}
		break;
		case SDL_MOUSEWHEEL:
		{
			int32_t delta = event.wheel.y;
			if (event.wheel.direction == SDL_MOUSEWHEEL_FLIPPED) {
				delta = -delta;
			}
			std::string deltaString = Jargon::StringUtilities::format("%d", delta);
			const char* command[] = {"osd-bar", "add", "volume", deltaString.c_str(), 0};
			mpv_command(mpv, command);
		}
		break;
		default:
			break;
	}
}
