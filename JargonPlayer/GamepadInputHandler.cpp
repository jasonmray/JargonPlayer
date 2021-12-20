
#include "GamepadInputHandler.h"
#include "MpvCommands.h"
#include "VideoWindow.h"

#include "Jargon/Math/Utilities.h"

#include <libmpv/include/client.h>

#include <cstdint>

GamepadInputHandler::GamepadInputHandler(){
}

GamepadInputHandler::~GamepadInputHandler(){
}

void GamepadInputHandler::handleInput(VideoWindow* videoWindow, mpv_handle *mpv, SDL_Event& event) {
	switch (event.type) {
		case SDL_CONTROLLERBUTTONDOWN:
			handleControllerButton(videoWindow, mpv, event);
			break;
		case SDL_CONTROLLERAXISMOTION:
			handleControllerAxis(videoWindow, mpv, event);
			break;
		default:
			break;
	}
}

void GamepadInputHandler::handleControllerButton(VideoWindow* videoWindow, mpv_handle *mpv, SDL_Event& event) {
	if (event.cbutton.button == SDL_CONTROLLER_BUTTON_A) {
		videoWindow->playPause();
	} else if (event.cbutton.button == SDL_CONTROLLER_BUTTON_RIGHTSHOULDER) {
		mpv_command(mpv, MpvCommands::PlaylistNext);
	} else if (event.cbutton.button == SDL_CONTROLLER_BUTTON_LEFTSHOULDER) {
		mpv_command(mpv, MpvCommands::PlaylistPrevious);
	} else if (event.cbutton.button == SDL_CONTROLLER_BUTTON_Y) {
		videoWindow->enterFullscreen();
	} else if (event.cbutton.button == SDL_CONTROLLER_BUTTON_B) {
		SDL_SetWindowFullscreen(videoWindow->getSDLWindow(), 0);
	} else if (event.cbutton.button == SDL_CONTROLLER_BUTTON_BACK) {
		videoWindow->resetZoom();
	}
}

void GamepadInputHandler::handleControllerAxis(VideoWindow* videoWindow, mpv_handle *mpv, SDL_Event& event) {
	if (event.caxis.axis == SDL_CONTROLLER_AXIS_LEFTX) {
		int16_t axisValue = event.caxis.value;
		int16_t mappedValue = Jargon::Math::rangeMap(axisValue, -32768, 32767, 2, 6);

		switch (mappedValue) {
			case 0:
				mpv_command(mpv, MpvCommands::SeekBackVeryLong);
				break;
			case 1:
				mpv_command(mpv, MpvCommands::SeekBackLong);
				break;
			case 2:
				mpv_command(mpv, MpvCommands::SeekBackNormal);
				break;
			case 3:
				mpv_command(mpv, MpvCommands::SeekBackShort);
				break;
			case 4:
				break;
			case 5:
				mpv_command(mpv, MpvCommands::SeekForwardShort);
				break;
			case 6:
				mpv_command(mpv, MpvCommands::SeekForwardNormal);
				break;
			case 7:
				mpv_command(mpv, MpvCommands::SeekForwardLong);
				break;
			case 8:
				mpv_command(mpv, MpvCommands::SeekForwardVeryLong);
				break;
			default:
				break;
		}
	} else if (event.caxis.axis == SDL_CONTROLLER_AXIS_RIGHTX) {
		int16_t axisValue = event.caxis.value;
		int16_t mappedValue = Jargon::Math::rangeMap(axisValue, -32768, 32767, 0, 2);
		switch (mappedValue) {
			case 0:
				mpv_command(mpv, MpvCommands::PanRight);
				break;
			case 1:
				break;
			case 2:
				mpv_command(mpv, MpvCommands::PanLeft);
				break;
			default:
				break;
		}
	} else if (event.caxis.axis == SDL_CONTROLLER_AXIS_RIGHTY) {
		int16_t axisValue = event.caxis.value;
		int16_t mappedValue = Jargon::Math::rangeMap(axisValue, -32768, 32767, 0, 2);
		switch (mappedValue) {
			case 0:
				mpv_command(mpv, MpvCommands::PanDown);
				break;
			case 1:
				break;
			case 2:
				mpv_command(mpv, MpvCommands::PanUp);
				break;
			default:
				break;
		}
	} else if (event.caxis.axis == SDL_CONTROLLER_AXIS_TRIGGERLEFT) {
		int16_t axisValue = event.caxis.value;
		int16_t mappedValue = Jargon::Math::rangeMap(axisValue, -32768, 32767, 0, 2);
		switch (mappedValue) {
			case 2:
				mpv_command(mpv, MpvCommands::ZoomDecrease);
				break;
			default:
				break;
		}
	} else if (event.caxis.axis == SDL_CONTROLLER_AXIS_TRIGGERRIGHT) {
		int16_t axisValue = event.caxis.value;
		int16_t mappedValue = Jargon::Math::rangeMap(axisValue, -32768, 32767, 0, 2);
		switch (mappedValue) {
			case 2:
				mpv_command(mpv, MpvCommands::ZoomIncrease);
				break;
			default:
				break;
		}
	}
}
