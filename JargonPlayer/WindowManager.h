#pragma once

#include "Util.h"
#include "ProgramOptions.h"
#include "VideoWindow.h"

#include <memory>
#include <map>


class WindowManager{
public:
	WindowManager(const ProgramOptions& programOptions);

	std::shared_ptr<VideoWindow> createWindow();
	void closeWindow(uint32_t windowId);
	void closeAll();
	void playPauseAll();
	bool isAnyWindowPlaying();

	void pumpEvents();

private:
	static const uint32_t InvalidWindow = ~0;

	const ProgramOptions& programOptions;
	std::map<uint32_t, std::shared_ptr<VideoWindow>> windows;
	uint32_t focusedWindow = InvalidWindow;
	bool processingDragAndDrop = false;
	int droppedFileCount = 0;
	bool wasAnyWindowPlaying = false;

	void pingDisplayTimeout();
};
