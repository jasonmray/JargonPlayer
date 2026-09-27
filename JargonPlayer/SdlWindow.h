#pragma once

#include "QuadrantLayout.h"

#include <SDL.h>


class SdlWindow{
public:
	SdlWindow(const char* windowTitle, int width, int height);
	virtual ~SdlWindow();

	uint32_t getWindowId() const;
	SDL_Window* getSDLWindow();
	SDL_GLContext& getGLContext();
	void setTitle(const char* title);
	void close();

	void handleEvent(SDL_Event& event);

	void toggleFullscreen();
	void enterFullscreen();
	void exitFullscreen();
	void minimize();

	QuadrantLayout::Bounds getWindowBounds();
	void setWindowPosition(int screenX, int screenY);
	void dragWindow(int deltaX, int deltaY);
	void getClientSize(int* windowWidth, int* windowHeight);
	void clientToScreen(int* x, int* y);
	void screenToClient(int* x, int* y);
	void resizeWindow(int deltaX, int deltaY);
	void resizeWindowProportional(int deltaX);

	void maximize();
	void moveToQuadrant(int displayIndex, QuadrantLayout::WindowQuadrant quadrant);
	void moveToQuadrant(QuadrantLayout::WindowQuadrant quadrant);
	void moveToMonitorFullscreen(int displayIndex);
	void tryPositionWithinMonitor();

	bool isAlwaysOnTop() const;
	void setAlwaysOnTop(bool onTop);
	void toggleAlwaysOnTop(bool& isNowOnTop);

	void increaseTransparency(float delta);
	void decreaseTransparency(float delta);

private:
	static uint32_t EnqueueMouseCheckEvent(uint32_t interval, void *param);

	static constexpr uint32_t MouseDwellBeforeHideMs = 3000;
	static constexpr uint32_t MouseCheckIntervalMs = 1000;

	uint32_t windowID;
	SDL_Window *window;
	SDL_GLContext sdlGLContext;

	SDL_TimerID mouseCheckTimerId;
	uint32_t userEventCode_MouseCheck;

	uint32_t mouseDwellCount;
	SDL_Point previousMousePosition;

	void checkMousePosition();
	void hideMouse();
};
