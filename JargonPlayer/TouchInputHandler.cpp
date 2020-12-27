#include "TouchInputHandler.h"
#include "MpvCommands.h"
#include "VideoWindow.h"

#include "Jargon/StringUtilities.h"

#include <libmpv/include/client.h>


TouchInputHandler::TouchInputHandler(){
}

TouchInputHandler::~TouchInputHandler(){
}

void TouchInputHandler::handleInput(VideoWindow* videoWindow, mpv_handle *mpv, SDL_Event& event) {
	switch (event.type) {
		case SDL_MULTIGESTURE:
		{
			if (event.mgesture.numFingers == 2) {
				std::string deltaString = Jargon::StringUtilities::format("%+f", event.mgesture.dDist * 10);
				const char* args[] = {"add", "video-zoom", deltaString.c_str(), 0};
				mpv_command(mpv, args);
			}
		}
		break;
		default:
			break;
	}
}
