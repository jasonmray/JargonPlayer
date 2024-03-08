#include "KeyboardInputHandler.h"
#include "DeviceStatus.h"
#include "MpvCommands.h"
#include "Screenshots.h"
#include "TraceLogging.h"
#include "VideoWindow.h"
#include "Util.h"

#include "Jargon/StringUtilities.h"
#include "Jargon/System/Clipboard.h"
#include "Jargon/System/Utilities.h"

#include <libmpv/include/client.h>


KeyboardInputHandler::KeyboardInputHandler(){
}

KeyboardInputHandler::~KeyboardInputHandler(){
}

void KeyboardInputHandler::handleInput(VideoWindow* videoWindow, mpv_handle *mpv, SDL_Event& event) {
	switch (event.type) {
		case SDL_KEYDOWN:
			handleKeydown(videoWindow, mpv, event);
			break;
		default:
			break;
	}
}

void KeyboardInputHandler::handleKeydown(VideoWindow* videoWindow, mpv_handle *mpv, SDL_Event& event) {

	bool keymodCtrl = (event.key.keysym.mod & KMOD_CTRL) != 0;
	bool keymodShift = (event.key.keysym.mod & KMOD_SHIFT) != 0;
	bool keymodAlt = (event.key.keysym.mod & KMOD_ALT) != 0;

	if (event.key.keysym.mod == 0) {
		if (event.key.keysym.sym == SDLK_SPACE || event.key.keysym.sym == SDLK_c) {
			videoWindow->playPause();
		} else if (event.key.keysym.sym == SDLK_LEFT) {
			mpv_command(mpv, MpvCommands::SeekBackShort);
		} else if (event.key.keysym.sym == SDLK_RIGHT) {
			mpv_command(mpv, MpvCommands::SeekForwardShort);
		} else if (event.key.keysym.sym == SDLK_PAGEUP) {
			mpv_command(mpv, MpvCommands::SeekBackLong);
		} else if (event.key.keysym.sym == SDLK_PAGEDOWN) {
			mpv_command(mpv, MpvCommands::SeekForwardLong);
		} else if (event.key.keysym.sym == SDLK_HOME) {
			mpv_command(mpv, MpvCommands::SeekToStart);
		} else if (event.key.keysym.sym == SDLK_x) {
			mpv_command(mpv, MpvCommands::SeekToStart);
//		} else if (event.key.keysym.sym == SDLK_v) {
//			mpv_command(mpv, MpvCommands::Stop);
		} else if (event.key.keysym.sym == SDLK_z) {
			mpv_command(mpv, MpvCommands::PlaylistPrevious);
		} else if (event.key.keysym.sym == SDLK_b) {
			mpv_command(mpv, MpvCommands::PlaylistNext);
		} else if (event.key.keysym.sym == SDLK_ESCAPE) {
			videoWindow->exitFullscreen();
		} else if (event.key.keysym.sym == SDLK_MINUS) {
			mpv_command(mpv, MpvCommands::FrameStepBack);
		} else if (event.key.keysym.sym == SDLK_EQUALS) {
			mpv_command(mpv, MpvCommands::FrameStepForward);
		} else if (event.key.keysym.sym == SDLK_LEFTBRACKET) {
			mpv_command(mpv, MpvCommands::SpeedDecrease);
			mpv_command(mpv, MpvCommands::ShowSpeed);
		} else if (event.key.keysym.sym == SDLK_RIGHTBRACKET) {
			mpv_command(mpv, MpvCommands::SpeedIncrease);
			mpv_command(mpv, MpvCommands::ShowSpeed);
		} else if (event.key.keysym.sym == SDLK_BACKSLASH) {
			mpv_command(mpv, MpvCommands::SpeedReset);
			mpv_command(mpv, MpvCommands::ShowSpeed);
		} else if (event.key.keysym.sym == SDLK_DOWN) {
			mpv_command(mpv, MpvCommands::VolumeDecrease);
		} else if (event.key.keysym.sym == SDLK_UP) {
			mpv_command(mpv, MpvCommands::VolumeIncrease);
		} else if (event.key.keysym.sym == SDLK_m) {
			mpv_command(mpv, MpvCommands::MuteToggle);
			mpv_command(mpv, MpvCommands::ShowMuteStatus);
		} else if (event.key.keysym.sym == SDLK_COMMA) {
			mpv_command(mpv, MpvCommands::GammaDecrease);
			mpv_command(mpv, MpvCommands::BrightnessDecrease);
		} else if (event.key.keysym.sym == SDLK_PERIOD) {
			mpv_command(mpv, MpvCommands::GammaIncrease);
			mpv_command(mpv, MpvCommands::BrightnessIncrease);
		} else if (event.key.keysym.sym == SDLK_SLASH) {
			mpv_command(mpv, MpvCommands::GammaReset);
			mpv_command(mpv, MpvCommands::BrightnessReset);
		} else if (event.key.keysym.sym == SDLK_q) {
			mpv_command(mpv, MpvCommands::ZoomDecrease);
		} else if (event.key.keysym.sym == SDLK_e) {
			mpv_command(mpv, MpvCommands::ZoomIncrease);
		} else if (event.key.keysym.sym == SDLK_RETURN) {
			videoWindow->toggleZoomToActualSize();
		} else if (event.key.keysym.sym == SDLK_w) {
			mpv_command(mpv, MpvCommands::PanUp);
		} else if (event.key.keysym.sym == SDLK_s) {
			mpv_command(mpv, MpvCommands::PanDown);
		} else if (event.key.keysym.sym == SDLK_a) {
			mpv_command(mpv, MpvCommands::PanLeft);
		} else if (event.key.keysym.sym == SDLK_d) {
			mpv_command(mpv, MpvCommands::PanRight);
		} else if (event.key.keysym.sym == SDLK_r) {
			videoWindow->resetZoom();
		} else if (event.key.keysym.sym == SDLK_f) {
			videoWindow->enterFullscreen();
		} else if (event.key.keysym.sym == SDLK_g) {
			mpv_command(mpv, MpvCommands::GammaFactorIncrease);
			mpv_command(mpv, MpvCommands::ShowGammaFactor);
		} else if (event.key.keysym.sym == SDLK_i) {
			mpv_command(mpv, MpvCommands::ToggleDeinterlace);
			mpv_command(mpv, MpvCommands::ShowInterlaceStatus);
		} else if (event.key.keysym.sym == SDLK_h) {
			bool borderless = SDL_GetWindowFlags(videoWindow->getSDLWindow()) & SDL_WINDOW_BORDERLESS;
			SDL_SetWindowBordered(videoWindow->getSDLWindow(), borderless ? SDL_TRUE : SDL_FALSE);
		} else if (event.key.keysym.sym == SDLK_k) {
			//videoWindow->hideCursor();
			//mpv_command(mpv, MpvCommands::CycleColorTemp); // doesn't work with hwdec
			mpv_command(mpv, MpvCommands::PanAudioLeft);
		} else if (event.key.keysym.sym == SDLK_1) {
			videoWindow->moveToQuadrant(0, QuadrantLayout::WindowQuadrant::TopLeft);
		} else if (event.key.keysym.sym == SDLK_2) {
			videoWindow->moveToQuadrant(0, QuadrantLayout::WindowQuadrant::TopRight);
		} else if (event.key.keysym.sym == SDLK_3) {
			videoWindow->moveToQuadrant(0, QuadrantLayout::WindowQuadrant::BottomLeft);
		} else if (event.key.keysym.sym == SDLK_4) {
			videoWindow->moveToQuadrant(0, QuadrantLayout::WindowQuadrant::BottomRight);
		} else if (event.key.keysym.sym == SDLK_5) {
			videoWindow->moveToQuadrant(1, QuadrantLayout::WindowQuadrant::TopLeft);
		} else if (event.key.keysym.sym == SDLK_6) {
			videoWindow->moveToQuadrant(1, QuadrantLayout::WindowQuadrant::TopRight);
		} else if (event.key.keysym.sym == SDLK_7) {
			videoWindow->moveToQuadrant(1, QuadrantLayout::WindowQuadrant::BottomLeft);
		} else if (event.key.keysym.sym == SDLK_8) {
			videoWindow->moveToQuadrant(1, QuadrantLayout::WindowQuadrant::BottomRight);
		} else if (event.key.keysym.sym == SDLK_BACKQUOTE) {
			videoWindow->moveToQuadrant(QuadrantLayout::WindowQuadrant::Center);
		} else if (event.key.keysym.sym == SDLK_t) {
			mpv_command(mpv, MpvCommands::CycleSubtitlesUp);
			mpv_command(mpv, MpvCommands::ShowSubtitlesTrack);
		} else if (event.key.keysym.sym == SDLK_y) {
			mpv_command(mpv, MpvCommands::CycleAudioTrackUp);
			mpv_command(mpv, MpvCommands::ShowAudioTrack);
		} else if (event.key.keysym.sym == SDLK_o) {
			mpv_command(mpv, MpvCommands::CycleAspectRatioUp);
			mpv_command(mpv, MpvCommands::ShowAspectRatio);
		} else if (event.key.keysym.sym == SDLK_p) {
			mpv_command(mpv, MpvCommands::TogglePerfStats);
		} else if (event.key.keysym.sym == SDLK_n) {
			videoWindow->minimize();
		} else if (event.key.keysym.sym == SDLK_TAB) {
			videoWindow->togglePlaylist();
		} else if (event.key.keysym.sym == SDLK_l) {
			mpv_command(mpv, MpvCommands::PanAudioRight);
		} else if (event.key.keysym.sym == SDLK_QUOTE) {
			std::string deviceStatus = DeviceStatus::BuildDeviceStatusString(videoWindow);
			videoWindow->showMessage(deviceStatus);
		} else if (event.key.keysym.sym == SDLK_AUDIONEXT) {
			mpv_command(mpv, MpvCommands::PlaylistNext);
		} else if (event.key.keysym.sym == SDLK_AUDIOPREV) {
			mpv_command(mpv, MpvCommands::PlaylistPrevious);
		} else if (event.key.keysym.sym == SDLK_F2) {
			videoWindow->toggleSlideshowForImages();
		}else if (event.key.keysym.sym == SDLK_F3) {
			mpv_command(mpv, MpvCommands::ShufflePlaylist);
			mpv_command(mpv, MpvCommands::PlaylistRestart);
			videoWindow->showMessage("Playlist shuffled");
		} else {
			TraceLogging::Instance()->logSdlKeyIgnored(event.key.keysym.sym, event.key.keysym.scancode, event.key.keysym.mod);
		}
	} else if (keymodAlt && keymodCtrl && keymodShift) {
		if (event.key.keysym.sym == SDLK_s) {
			Screenshots::SaveScreenshotToSourceFolder(mpv, videoWindow, ScreenshotType_WindowDisplay);
		}
	} else if (keymodAlt && keymodCtrl) {
		if (event.key.keysym.sym == SDLK_s) {
			Screenshots::SaveScreenshotToUserFolder(mpv, videoWindow, ScreenshotType_WindowDisplay);
		}
	} else if (keymodCtrl && keymodShift) {
		if (event.key.keysym.sym == SDLK_c) {
			std::string filename = videoWindow->getActiveFilename();
			Jargon::System::copyFileToClipboard(filename.c_str());
		} else if (event.key.keysym.sym == SDLK_PAGEUP) {
			mpv_command(mpv, MpvCommands::SeekBackSuperLong);
		} else if (event.key.keysym.sym == SDLK_PAGEDOWN) {
			mpv_command(mpv, MpvCommands::SeekForwardSuperLong);
		} else if (event.key.keysym.sym == SDLK_s) {
			// save screenshot to same folder as source file
			Screenshots::SaveScreenshotToSourceFolder(mpv, videoWindow, ScreenshotType_OriginalVideo);
		}
	} else if (keymodAlt && keymodShift) {
		if (event.key.keysym.sym == SDLK_MINUS) {
			videoWindow->resizeWindowProportional(-32);
		} else if (event.key.keysym.sym == SDLK_EQUALS) {
			videoWindow->resizeWindowProportional(32);
		}
	} else if (keymodCtrl) {
		if (event.key.keysym.sym == SDLK_c) {
			std::string filename = videoWindow->getActiveFilename();
			SDL_SetClipboardText(filename.c_str());
		} else if (event.key.keysym.sym == SDLK_e) {
			std::string filename = videoWindow->getActiveFilename();
			Jargon::System::showFileInExplorer(filename.c_str());
		} else if (event.key.keysym.sym == SDLK_LEFT) {
			mpv_command(mpv, MpvCommands::PlaylistPrevious);
		} else if (event.key.keysym.sym == SDLK_RIGHT) {
			mpv_command(mpv, MpvCommands::PlaylistNext);
		} else if (event.key.keysym.sym == SDLK_UP) {
			mpv_command(mpv, MpvCommands::VolumeIncrease);
		} else if (event.key.keysym.sym == SDLK_DOWN) {
			mpv_command(mpv, MpvCommands::VolumeDecrease);
		} else if (event.key.keysym.sym == SDLK_HOME) {
			mpv_command(mpv, MpvCommands::PlaylistRestart);
		} else if (event.key.keysym.sym == SDLK_1) {
			mpv_command(mpv, MpvCommands::Rotate0);
		} else if (event.key.keysym.sym == SDLK_2) {
			mpv_command(mpv, MpvCommands::Rotate90);
		} else if (event.key.keysym.sym == SDLK_3) {
			mpv_command(mpv, MpvCommands::Rotate180);
		} else if (event.key.keysym.sym == SDLK_4) {
			mpv_command(mpv, MpvCommands::Rotate270);
		} else if (event.key.keysym.sym == SDLK_m) {
			mpv_command(mpv, MpvCommands::ToggleMirror);
		} else if (event.key.keysym.sym == SDLK_f) {
			mpv_command(mpv, MpvCommands::ToggleFlip);
		} else if (event.key.keysym.sym == SDLK_F4 || event.key.keysym.sym == SDLK_w) {
			mpv_command(mpv, MpvCommands::Stop);
			videoWindow->close();
		} else if (event.key.keysym.sym == SDLK_a) {
			bool isNowOnTop = false;
			videoWindow->toggleAlwaysOnTop(isNowOnTop);

			{
				std::string msg = Jargon::StringUtilities::format("Always on top: %s", isNowOnTop ? "yes" : "no");
				const char* args[] = {"show-text", msg.c_str(), 0};
				mpv_command(mpv, args);
			}
		} else if (event.key.keysym.sym == SDLK_o) {
			mpv_command(mpv, MpvCommands::ResetAspectRatio);
			mpv_command(mpv, MpvCommands::ShowAspectRatio);
		} else if (event.key.keysym.sym == SDLK_s) {
			Screenshots::SaveScreenshotToUserFolder(mpv, videoWindow, ScreenshotType_OriginalVideo);
		} else if (event.key.keysym.sym == SDLK_8) {
			videoWindow->resetAudioFrequency();
		} else if (event.key.keysym.sym == SDLK_9) {
			videoWindow->changeAudioFrequency(-5);
		} else if (event.key.keysym.sym == SDLK_0) {
			videoWindow->changeAudioFrequency(5);
		}
	} else if (keymodShift) {
		if (event.key.keysym.sym == SDLK_LEFT) {
			mpv_command(mpv, MpvCommands::SeekBackNormal);
		} else if (event.key.keysym.sym == SDLK_RIGHT) {
			mpv_command(mpv, MpvCommands::SeekForwardNormal);
		} else if (event.key.keysym.sym == SDLK_PAGEUP) {
			mpv_command(mpv, MpvCommands::SeekBackVeryLong);
		} else if (event.key.keysym.sym == SDLK_PAGEDOWN) {
			mpv_command(mpv, MpvCommands::SeekForwardVeryLong);
		} else if (event.key.keysym.sym == SDLK_t) {
			mpv_command(mpv, MpvCommands::CycleSubtitlesDown);
			mpv_command(mpv, MpvCommands::ShowSubtitlesTrack);
		} else if (event.key.keysym.sym == SDLK_y) {
			mpv_command(mpv, MpvCommands::CycleAudioTrackDown);
			mpv_command(mpv, MpvCommands::ShowAudioTrack);
		} else if (event.key.keysym.sym == SDLK_q) {
			videoWindow->rotate(-1);
		} else if (event.key.keysym.sym == SDLK_e) {
			videoWindow->rotate(1);
		} else if (event.key.keysym.sym == SDLK_o) {
			mpv_command(mpv, MpvCommands::CycleAspectRatioDown);
			mpv_command(mpv, MpvCommands::ShowAspectRatio);
		} else if (event.key.keysym.sym == SDLK_k || event.key.keysym.sym == SDLK_l) {
			mpv_command(mpv, MpvCommands::ResetAudioPan);
		} else if (event.key.keysym.sym == SDLK_g) {
			mpv_command(mpv, MpvCommands::GammaFactorDecrease);
			mpv_command(mpv, MpvCommands::ShowGammaFactor);
		}
	} else if (keymodAlt) {
		if (event.key.keysym.sym == SDLK_LEFT) {
			mpv_command(mpv, MpvCommands::SeekBackVeryShort);
		} else if (event.key.keysym.sym == SDLK_RIGHT) {
			mpv_command(mpv, MpvCommands::SeekForwardVeryShort);
		} else if (event.key.keysym.sym == SDLK_1) {
			videoWindow->moveToMonitorFullscreen(0);
		} else if (event.key.keysym.sym == SDLK_2) {
			videoWindow->moveToMonitorFullscreen(1);
		} else if (event.key.keysym.sym == SDLK_MINUS) {
			videoWindow->resizeWindowProportional(-16);
		} else if (event.key.keysym.sym == SDLK_EQUALS) {
			videoWindow->resizeWindowProportional(16);
		} else if (event.key.keysym.sym == SDLK_t) {
			mpv_command(mpv, MpvCommands::CycleSubtitleBackgroundColor);
		} else if (event.key.keysym.sym == SDLK_g) {
			mpv_command(mpv, MpvCommands::GammaFactorReset);
			mpv_command(mpv, MpvCommands::ShowGammaFactor);
		}
	}
}
