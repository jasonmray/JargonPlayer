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

#include <string>
#include <thread>

class VideoWindow : public SdlWindow{
public:
	static const char* DefaultWindowTitle;
	static const int DefaultWindowWidth;
	static const int DefaultWindowHeight;

	VideoWindow();
	~VideoWindow();

	void showFormattedMessage(const char * formatString, ...);
	void showMessage(const char* msg);
	void startPlayAsync(const char* filename);
	void enqueueFile(const char* filename);

	void playPause();
	void pause();
	void ensureUnpaused();
	bool isPlaying();
	void moveToQuadrant(int displayIndex, QuadrantLayout::WindowQuadrant quadrant);
	void moveToQuadrant(QuadrantLayout::WindowQuadrant quadrant);

	void enterFullscreen();
	void exitFullscreen();

	void minimize();

	void hideCursor();

	void handleEvent(SDL_Event& event);
	void redraw();

	void resetZoom();
	void toggleZoomToActualSize();

	void rotate(double delta);

	std::string getActiveFilename() const;

	void changeAudioFrequency(int percentDelta);
	void resetAudioFrequency();

private:
	static void* GetProcAddress(void *fn_ctx, const char *name);
	static void OnMpvEvents(void *ctx);
	static void OnMpvRedraw(void *ctx);

	void onMpvEvents();
	void onMpvRedraw();

	void zoomToActualSize();

	void processMpvEvents();

	mpv_handle *mpv = nullptr;
	mpv_render_context *mpvGLRenderContext = nullptr;
	mpv_opengl_init_params mpvGLParams = {};
	uint32_t wakeForMpvRedrawEventId = 0;
	uint32_t wakeForMpvEventsEventId = 0;
	bool zoomedToActualSize = false;
	int audioFrequencyPercent = 100;

	std::thread mpvEventThread;
	Jargon::System::Event mpvEventsAvailable;
	bool mpvEventThreadShouldExit = false;
	GamepadInputHandler gamepadInputHandler;
	KeyboardInputHandler keyboardInputHander;
	MouseInputHandler mouseInputHandler;
	TouchInputHandler touchInputHandler;
	PlaylistFilter playlistFilter;
};