
#include "MpvCommands.h"
#include "ProgramOptions.h"
#include "QuadrantLayout.h"
#include "Util.h"
#include "VideoWindow.h"
#include "WindowManager.h"

#include <algorithm>
#include <vector>

#pragma comment (lib, "sdl/lib/x64/sdl2.lib")
#pragma comment (lib, "libmpv/lib/x64/mpv.lib")


int findTopLeftDisplayIndex(){
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


int appmain(int argc, const char *argv[]){
	SDL_Init(SDL_INIT_VIDEO | SDL_INIT_GAMECONTROLLER);
	SDL_DisableScreenSaver();
	SDL_JoystickEventState(SDL_ENABLE);

	SDL_GameController* gameController = SDL_GameControllerOpen(0);

	ProgramOptions& programOptions = ProgramOptions::Instance;

	if(!programOptions.processOptions(argc, argv)){
		return 1;
	}

	WindowManager windowManager(programOptions);

	const size_t fileCount = programOptions.files.size();

	if(programOptions.openMode == ProgramOptions::OpenMode::Tile){
		if(fileCount > 5){
			// try to tile multi-monitor
			for(int i = 0; i < fileCount; i++){
				auto window = windowManager.createWindow();
				window->startPlayAsync(programOptions.files[i]);
				window->moveToQuadrant(i / 4, (QuadrantLayout::WindowQuadrant)(i%4));
			}
		}else if(fileCount > 1){
			// tile single-monitor
			int chosenDisplay = findTopLeftDisplayIndex();
			for(int i = 0; i < fileCount; i++){
				auto window = windowManager.createWindow();
				window->startPlayAsync(programOptions.files[i]);
				window->moveToQuadrant(chosenDisplay, (QuadrantLayout::WindowQuadrant)i);
			}
		}else if(fileCount == 1){
			// open a single file
			auto window = windowManager.createWindow();
			window->startPlayAsync(programOptions.files[0]);
		}else{
			// open an empty window
			windowManager.createWindow();
		}
	}else{
		// open a single window and enqueue everything
		auto window = windowManager.createWindow();

		if(fileCount > 0){
			window->startPlayAsync(programOptions.files[0]);
			for(size_t i = 1; i < fileCount; i++){
				window->enqueueFile(programOptions.files[i]);
			}
		}
	}

	windowManager.pumpEvents();
	
	SDL_EnableScreenSaver();
	SDL_GameControllerClose(gameController);
	return 0;
}
