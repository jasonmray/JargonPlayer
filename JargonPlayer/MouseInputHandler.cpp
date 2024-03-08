#include "MouseInputHandler.h"
#include "MpvCommands.h"
#include "ProgramOptions.h"
#include "Util.h"
#include "VideoWindow.h"

#include "Jargon/StringUtilities.h"

#include <libmpv/include/client.h>


MouseInputHandler::MouseInputHandler(){
}

MouseInputHandler::~MouseInputHandler(){
}

void MouseInputHandler::handleInput(VideoWindow* videoWindow, mpv_handle *mpv, SDL_Event& event) {
	SDL_Keymod keyModState = SDL_GetModState();
	if ((keyModState & KMOD_CTRL)) {
	switch (event.type) {
		case SDL_MOUSEMOTION:
			case SDL_MOUSEBUTTONDOWN:
			{
				int localMouseX = 0;
				int localMouseY = 0;
				if (SDL_GetMouseState(&localMouseX, &localMouseY) & SDL_BUTTON_LMASK)
				{
					int windowWidth = 0;
					int windowHeight = 0;
					videoWindow->getClientSize(&windowWidth, &windowHeight);

					videoWindow->setCurrentPlaybackPercent(100.0 * localMouseX / windowWidth);
				}
			}
			break;
		}
	}
	else {
		switch (event.type) {
			case SDL_MOUSEMOTION:
			{
				if (SDL_GetMouseState(nullptr, nullptr) & SDL_BUTTON_LMASK)
		{
					int newMouseX = 0;
					int newMouseY = 0;

					SDL_GetGlobalMouseState(&newMouseX, &newMouseY);
					videoWindow->dragWindow(newMouseX - previousMouseX, newMouseY - previousMouseY);
				}
				else
				{
					const double playbackDuration = videoWindow->getCurrentItemPlaybackDuration();

					if (playbackDuration > 0) {
			mpv_command(mpv, MpvCommands::ShowProgressBar);
		}
				}

				SDL_GetGlobalMouseState(&previousMouseX, &previousMouseY);
			}
		break;
		case SDL_MOUSEBUTTONDOWN:
		{
			if (event.button.clicks == 2) {
				videoWindow->enterFullscreen();
			}
				else {
					SDL_CaptureMouse(SDL_TRUE);
				}
			}
			break;
			case SDL_MOUSEBUTTONUP:
			{
				SDL_CaptureMouse(SDL_FALSE);
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
}
