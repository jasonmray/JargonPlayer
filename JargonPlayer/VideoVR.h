#pragma once

#include "GamepadInputHandler.h"
#include "KeyboardInputHandler.h"
#include "MouseInputHandler.h"
#include "TouchInputHandler.h"

#include "PlaylistFilter.h"

#include "QuadrantLayout.h"
#include "SdlWindow.h"

#include "Jargon/System/Event.h"

#include <libmpv/include/client.h>
#include <libmpv/include/render_gl.h>

#include <thread>

class VideoVR : public SdlWindow{
public:
	static const char* DefaultWindowTitle;
	static const int DefaultWindowWidth;
	static const int DefaultWindowHeight;

	VideoVR();
	~VideoVR();

	void showFormattedMessage(const char * formatString, ...);
	void showMessage(const char* msg);
	void startPlayAsync(const char* filename);
	void enqueueFile(const char* filename);

	void playPause();

	void hideCursor();

	void handleEvent(SDL_Event& event);
	void redraw();

private:
	static void* GetProcAddress(void *fn_ctx, const char *name);
	static void OnMpvEvents(void *ctx);
	static void OnMpvRedraw(void *ctx);

	void onMpvEvents();
	void onMpvRedraw();

	void processMpvEvents();

	mpv_handle *mpv = nullptr;
	mpv_render_context *mpvGLRenderContext = nullptr;
	mpv_opengl_init_params mpvGLParams = {};
	uint32_t wakeForMpvRedrawEventId = 0;
	uint32_t wakeForMpvEventsEventId = 0;

	std::thread mpvEventThread;
	Jargon::System::Event mpvEventsAvailable;
	bool mpvEventThreadShouldExit = false;

	GamepadInputHandler gamepadInputHandler;
	KeyboardInputHandler keyboardInputHander;
	MouseInputHandler mouseInputHandler;
	TouchInputHandler touchInputHandler;
	PlaylistFilter playlistFilter;
};