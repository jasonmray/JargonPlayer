
#include "MpvCommands.h"
#include "ConfigFile.h"
#include "ProgramOptions.h"
#include "QuadrantLayout.h"
#include "Util.h"
#include "VideoWindow.h"
#include "WindowManager.h"

#include "Jargon/System/WindowsMemoryLeakHelpers.h"

#include <algorithm>
#include <vector>


static int findTopLeftDisplayIndex(){
	int chosenDisplay = 0;

	int displayCount = SDL_GetNumVideoDisplays();
	SDL_Rect chosenBounds = {};
	SDL_GetDisplayUsableBounds(0, &chosenBounds);

	for(int i = 1; i < displayCount; i++){
		SDL_Rect bounds = {};
		SDL_GetDisplayUsableBounds(i, &bounds);

		if(bounds.x < chosenBounds.x || bounds.y < chosenBounds.y){
			chosenBounds = bounds;
			chosenDisplay = i;
		}
	}

	return chosenDisplay;
}

static void applyWindowMode(const ProgramOptions& programOptions, SdlWindow* window) {
	if (programOptions.windowMode == ProgramOptions::WindowMode::Fullscreen) {
		window->enterFullscreen();
	} else if (programOptions.windowMode == ProgramOptions::WindowMode::Maximized) {
		window->maximize();
	} else {
		window->moveToQuadrant(QuadrantLayout::WindowQuadrant::Center);
	}
}

int appmain(int argc, const char *argv[]){
	SDL_Init(SDL_INIT_VIDEO | SDL_INIT_GAMECONTROLLER);
	SDL_DisableScreenSaver();
	SDL_JoystickEventState(SDL_ENABLE);

	SDL_GameController* gameController = SDL_GameControllerOpen(0);

	ProgramOptions& programOptions = ProgramOptions::Instance;
	if(!programOptions.processOptions(argc, argv)){
		return 1;
	}

	ConfigFile& configFile = ConfigFile::Instance;
	configFile.loadFromDefaultFile();


	WindowManager windowManager(programOptions);

	const size_t fileCount = programOptions.files.size();

	if(programOptions.openMode == ProgramOptions::OpenMode::Tile){
		if(fileCount > 5){
			// try to tile multi-monitor
			for(int i = 0; i < fileCount; i++){
				auto window = windowManager.createWindow();
				window->moveToQuadrant(i / 4, (QuadrantLayout::WindowQuadrant)(i%4));
				window->startPlayAsync(programOptions.files[i].c_str());
			}
		}else if(fileCount > 1){
			// tile single-monitor
			int chosenDisplay = findTopLeftDisplayIndex();
			for(int i = 0; i < fileCount; i++){
				auto window = windowManager.createWindow();
				window->moveToQuadrant(chosenDisplay, (QuadrantLayout::WindowQuadrant)i);
				window->startPlayAsync(programOptions.files[i].c_str());
			}
		}else if(fileCount == 1){
			// open a single file
			auto window = windowManager.createWindow();

			applyWindowMode(programOptions, window.get());

			window->startPlayAsync(programOptions.files[0].c_str());
		}else{
			// open an empty window
			windowManager.createWindow();
		}
	}else{
		// open a single window and enqueue everything
		auto window = windowManager.createWindow();

		applyWindowMode(programOptions, window.get());

		if(fileCount > 0){
			window->startPlayAsync(programOptions.files[0].c_str());
			for(size_t i = 1; i < fileCount; i++){
				window->enqueueFile(programOptions.files[i].c_str());
			}
		}
	}

	windowManager.pumpEvents();
	
	SDL_EnableScreenSaver();
	SDL_GameControllerClose(gameController);

	JARGON_WINDOWS_DUMP_MEMORY_LEAKS();

	return 0;
}
