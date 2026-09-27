#include "MouseInputHandler.h"
#include "MpvCommands.h"
#include "ProgramOptions.h"
#include "Util.h"
#include "VideoWindow.h"

#include "Jargon/StringUtilities.h"
#include "Jargon/Math/Utilities.h"

#include <mpv/client.h>


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
				// handle ctrl+click or ctrl+drag by moving playback position
				int localMouseX = 0;
				int localMouseY = 0;
				if (SDL_GetMouseState(&localMouseX, &localMouseY) & SDL_BUTTON_LMASK) {
					int windowWidth = 0;
					int windowHeight = 0;
					videoWindow->getClientSize(&windowWidth, &windowHeight);

					const int margin = (int)(windowWidth * 0.024f + 0.5f);
					const double seekPosition = Jargon::Math::rangeMap<int, double>(localMouseX, margin, windowWidth - margin, 0.0, 100.0);

					videoWindow->setCurrentPlaybackPercent(seekPosition);
				}
			}
			break;
		}
	} else {
		switch (event.type) {
			case SDL_MOUSEMOTION:
			{
				if (dragging && (SDL_GetMouseState(nullptr, nullptr) & SDL_BUTTON_LMASK)) {
					int mouseX = 0;
					int mouseY = 0;

					SDL_GetGlobalMouseState(&mouseX, &mouseY);
					videoWindow->screenToClient(&mouseX, &mouseY);

					videoWindow->dragWindow(mouseX - dragStartClientX, mouseY - dragStartClientY);
				} else {
					const double playbackDuration = videoWindow->getCurrentItemPlaybackDuration();

					if (playbackDuration > 0) {
						mpv_command(mpv, MpvCommands::ShowProgressBar);
					}
				}
			}
			break;
			case SDL_MOUSEBUTTONDOWN:
			{
				if (event.button.clicks == 2) {
					SDL_CaptureMouse(SDL_FALSE);
					dragging = false;

					videoWindow->toggleFullscreen();
				} else {
					SDL_CaptureMouse(SDL_TRUE);
					SDL_GetMouseState(&dragStartClientX, &dragStartClientY);
					dragging = true;
				}
			}
			break;
			case SDL_MOUSEBUTTONUP:
			{
				SDL_CaptureMouse(SDL_FALSE);
				dragging = false;
			}
			break;
			case SDL_MOUSEWHEEL:
			{
				int32_t delta = event.wheel.y;
				if (event.wheel.direction == SDL_MOUSEWHEEL_FLIPPED) {
					delta = -delta;
				}
				std::string deltaString = Jargon::StringUtilities::format("%d", delta);
				const char* command[] = { "osd-bar", "add", "volume", deltaString.c_str(), 0 };
				mpv_command(mpv, command);
			}
			break;
			default:
				break;
		}
	}
}
