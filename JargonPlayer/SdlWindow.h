#pragma once

#include <sdl/include/SDL.h>

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
