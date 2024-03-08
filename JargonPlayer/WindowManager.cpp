#include "WindowManager.h"
#include "TraceLogging.h"
#include "Jargon/System/Utilities.h"

WindowManager::WindowManager(const ProgramOptions& programOptions):
	programOptions(programOptions)
{
}

std::shared_ptr<VideoWindow> WindowManager::createWindow(){
	std::shared_ptr<VideoWindow> videoWindow = std::make_shared<VideoWindow>();
	windows[videoWindow->getWindowId()] = videoWindow;
	return videoWindow;
}

void WindowManager::closeWindow(uint32_t windowId){
	windows.erase(windowId);
	if(windows.empty()){
		SDL_Quit();
	}
}

void WindowManager::closeAll(){
	for(auto& window : windows){
		window.second->close();
	}
}

void WindowManager::playPauseAll(){
	for(auto& window : windows){
		window.second->playPause();
	}
}

bool WindowManager::isAnyWindowPlaying(){
	for (auto& window : windows) {
		if (window.second->isPlaying()) {
			return true;
		}
	}

	return false;
}

void WindowManager::pumpEvents(){

	while(1){
		SDL_Event event = {};
		if(SDL_WaitEvent(&event) != 1) {
			return;
		}

		TraceLogging::Instance()->logSdlEvent(event.type, "");

		if(event.type == SDL_QUIT){
			return;
		} else if(event.type == SDL_KEYDOWN){
			TraceLogging::Instance()->logSdlKeydown(event.key.keysym.sym, event.key.keysym.scancode, event.key.keysym.mod);

			// Clear Numlock modifier for all key inputs.
			event.key.keysym.mod &= ~KMOD_NUM;

			if(event.key.keysym.sym == SDLK_q && event.key.keysym.mod & KMOD_CTRL){
				closeAll();
				return;
			} else if(event.key.keysym.sym == SDLK_SPACE && event.key.keysym.mod & KMOD_CTRL){
				playPauseAll();
			} else if(event.key.keysym.sym == SDLK_n && event.key.keysym.mod & KMOD_CTRL){
				createWindow();
			} else if (event.key.keysym.sym == SDLK_AUDIOPLAY || event.key.keysym.sym == SDL_SCANCODE_AUDIOSTOP) {
				playPauseAll();
			}
		} else if(event.type == SDL_DROPBEGIN){
			this->processingDragAndDrop = true;
			this->droppedFileCount = 0;
		} else if(event.type == SDL_DROPFILE){
			char* filepath = event.drop.file;

			if(this->droppedFileCount == 0){
				windows[event.drop.windowID]->startPlayAsync(filepath);
			} else {
				windows[event.drop.windowID]->enqueueFile(filepath);
			}

			SDL_free(filepath); // We have to free the path!

			this->droppedFileCount++;
		} else if(event.type == SDL_DROPCOMPLETE){
			this->processingDragAndDrop = false;
			this->droppedFileCount = 0;
		} else if(event.type == SDL_CONTROLLERAXISMOTION || event.type == SDL_CONTROLLERBUTTONDOWN  || event.type == SDL_MULTIGESTURE){
			if(focusedWindow != InvalidWindow){
				windows[focusedWindow]->handleEvent(event);
			}
		}

		if(windows.find(event.window.windowID) != windows.end()){
			// When caps lock enabled, send window events to all windows
			if((SDL_GetModState() & KMOD_CAPS) == KMOD_CAPS){
				if(event.type == SDL_KEYDOWN){
					event.key.keysym.mod &= ~KMOD_CAPS;
				}

				for (auto it : windows) {
					it.second->handleEvent(event);
				}
			}else{
				// Otherwise, send it to the window the event is targeting
				windows[event.window.windowID]->handleEvent(event);
			}
		}

		if(event.type == SDL_WINDOWEVENT){
			if(event.window.event == SDL_WINDOWEVENT_CLOSE){
				closeWindow(event.window.windowID);
			} else if(event.window.event == SDL_WINDOWEVENT_FOCUS_GAINED){
				focusedWindow = event.window.windowID;
			} else if(event.window.event == SDL_WINDOWEVENT_FOCUS_LOST){
				focusedWindow = InvalidWindow;
			}
		}

		pingDisplayTimeout();
	}
}

void WindowManager::pingDisplayTimeout(){
	bool anyWindowPlaying = isAnyWindowPlaying();

	if (anyWindowPlaying && !wasAnyWindowPlaying) {
		Jargon::System::notifyDisplayInUse(true);
	} else if (!anyWindowPlaying && wasAnyWindowPlaying) {
		Jargon::System::notifyDisplayInUse(false);
	}

	wasAnyWindowPlaying = anyWindowPlaying;
}