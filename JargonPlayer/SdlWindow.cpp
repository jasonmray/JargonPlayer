#include "SdlWindow.h"

#include "Jargon/System/WindowsDefines.h"
#include "sdl/include/SDL_syswm.h"

#include "Jargon/Math/Utilities.h"

#include <cassert>

namespace{
	HWND GetSdlHwnd(SDL_Window* window) {
		SDL_SysWMinfo systemInfo;
		SDL_VERSION(&systemInfo.version);
		SDL_GetWindowWMInfo(window, &systemInfo);

		return systemInfo.info.win.window;
	}

	bool IsTopMostWindow(HWND hwnd) {
		DWORD dwExStyle = ::GetWindowLong(hwnd, GWL_EXSTYLE);
		if ((dwExStyle & WS_EX_TOPMOST) != 0) {
			return true;
		}
		return false;
	}
}


uint32_t SdlWindow::EnqueueMouseCheckEvent(uint32_t interval, void *param) {
	SdlWindow* targetWindow = (SdlWindow*)param;

	SDL_UserEvent userevent = {};
	userevent.type = SDL_USEREVENT;
	userevent.timestamp = SDL_GetTicks();
	userevent.windowID = targetWindow->windowID;
	userevent.code = targetWindow->userEventCode_MouseCheck;
	userevent.data1 = targetWindow;
	userevent.data2 = NULL;

	SDL_Event event;
	event.type = SDL_USEREVENT;
	event.user = userevent;

	SDL_PushEvent(&event);

	return interval; // run timer again for the same interval
}

SdlWindow::SdlWindow(const char* windowTitle, int width, int height){
	SDL_SetHintWithPriority(SDL_HINT_VIDEO_MINIMIZE_ON_FOCUS_LOSS, "0", SDL_HINT_DEFAULT);

	window = SDL_CreateWindow(
		windowTitle, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
		width, height,
		SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE
	);

	windowID = SDL_GetWindowID(window);

	sdlGLContext = SDL_GL_CreateContext(window);
	assert(sdlGLContext);

	userEventCode_MouseCheck = SDL_RegisterEvents(1);
	mouseCheckTimerId = SDL_AddTimer(MouseCheckIntervalMs, EnqueueMouseCheckEvent, this);
	mouseDwellCount = 0;
	previousMousePosition = {-1, -1};
}

SdlWindow::~SdlWindow(){
	SDL_RemoveTimer(mouseCheckTimerId);
	SDL_GL_DeleteContext(sdlGLContext);
	SDL_DestroyWindow(window);
}

uint32_t SdlWindow::getWindowId() const{
	return windowID;
}

SDL_Window* SdlWindow::getSDLWindow(){
	return window;
}

SDL_GLContext& SdlWindow::getGLContext(){
	return sdlGLContext;
}

void SdlWindow::setTitle(const char* title){
	SDL_SetWindowTitle(getSDLWindow(), title);
}

void SdlWindow::close(){
	SDL_Event event = {};
	event.type = SDL_WINDOWEVENT;
	event.window.windowID = getWindowId();
	event.window.event = SDL_WINDOWEVENT_CLOSE;
	SDL_PushEvent(&event);
}

bool SdlWindow::isAlwaysOnTop() const {
	HWND hwnd = GetSdlHwnd(window);
	if (hwnd != nullptr) {
		return IsTopMostWindow(hwnd);
	}

	return false;
}

void SdlWindow::setAlwaysOnTop(bool onTop) {
	HWND hwnd = GetSdlHwnd(window);
	if (hwnd != nullptr) {
		WINDOWPLACEMENT placement = {0};
		RECT rect = {0};

		if (GetWindowPlacement(hwnd, &placement) && GetWindowRect(hwnd, &rect)) {
			HWND placeAfter = onTop ? HWND_TOPMOST : HWND_NOTOPMOST;
			SetWindowPos(hwnd, placeAfter, rect.left, rect.top, rect.right - rect.left, rect.bottom - rect.top, placement.flags);
		}
	}
}

void SdlWindow::toggleAlwaysOnTop(bool& isNowOnTop) {
	HWND hwnd = GetSdlHwnd(window);
	if (hwnd != nullptr) {
		WINDOWPLACEMENT placement = {0};
		RECT rect = {0};

		if (GetWindowPlacement(hwnd, &placement) && GetWindowRect(hwnd, &rect)) {
			bool currentlyTopMost = IsTopMostWindow(hwnd);

			HWND placeAfter = currentlyTopMost ? HWND_NOTOPMOST : HWND_TOPMOST;
			SetWindowPos(hwnd, placeAfter, rect.left, rect.top, rect.right - rect.left, rect.bottom - rect.top, placement.flags);

			isNowOnTop = !currentlyTopMost;
		}
	}
}

void SdlWindow::handleEvent(SDL_Event& event) {
	switch (event.type) {
		case SDL_KEYDOWN:
		{
			if (event.key.keysym.mod & KMOD_CTRL) {
				if (event.key.keysym.sym == SDLK_EQUALS) {
					increaseTransparency(0.05f);
				} else if (event.key.keysym.sym == SDLK_MINUS) {
					decreaseTransparency(0.05f);
				}
			}
			break;
		}
		case SDL_USEREVENT:
		{
			if(event.user.code == userEventCode_MouseCheck){
				if (event.user.data1 == this) {
					checkMousePosition();
				}
			}
			break;
		}
		default:
			break;
	}
}

void SdlWindow::increaseTransparency(float delta) {
	float currentOpacity = 1.f;
	SDL_GetWindowOpacity(window, &currentOpacity);

	float newOpacity = currentOpacity + delta;
	newOpacity = Jargon::Math::clamp(newOpacity, 0.1f, 1.f);

	SDL_SetWindowOpacity(window, newOpacity);
}

void SdlWindow::decreaseTransparency(float delta) {
	increaseTransparency(-delta);
}

// Test if the mouse position is sitting idle over the window, and hide it if it dwells too long.
void SdlWindow::checkMousePosition() {

	const uint32_t windowFlags = SDL_GetWindowFlags(window);
	const bool mouseIsOverWindow = (windowFlags & SDL_WINDOW_MOUSE_FOCUS) == SDL_WINDOW_MOUSE_FOCUS;
	
	SDL_Point mousePosition = {-1, -1};
	const uint32_t mouseButtonState = SDL_GetGlobalMouseState(&mousePosition.x, &mousePosition.y);
	bool mouseHasNotMoved = (mousePosition.x == previousMousePosition.x && mousePosition.y == previousMousePosition.y);

	if (mouseIsOverWindow && mouseHasNotMoved) {
		mouseDwellCount++;
		if (mouseDwellCount == MouseDwellBeforeHideMs / MouseCheckIntervalMs) {
			hideMouse();
		}
	} else {
		mouseDwellCount = 0;
	}

	previousMousePosition = mousePosition;
}

// Move the mouse cursor to the far right of the display
void SdlWindow::hideMouse() {
	int currentDisplayIndex = SDL_GetWindowDisplayIndex(window);

	SDL_Rect displayBounds = {};
	SDL_GetDisplayBounds(currentDisplayIndex, &displayBounds);

	SDL_WarpMouseGlobal(displayBounds.x + displayBounds.w, displayBounds.y + displayBounds.h / 2);
}