#include "VideoWindow.h"
#include "MpvCommands.h"
#include "ProgramOptions.h"
#include "TraceLogging.h"
#include "Util.h"

#include "Jargon/StringUtilities.h"
#include "Jargon/Math/Utilities.h"

#include <algorithm>
#include <cassert>
#include <string>

const char* VideoWindow::DefaultWindowTitle = "JargonPlayer";
const int VideoWindow::DefaultWindowWidth = 800;
const int VideoWindow::DefaultWindowHeight = 450;

VideoWindow::VideoWindow():
	SdlWindow(DefaultWindowTitle, DefaultWindowWidth, DefaultWindowHeight),
	mpvEventsAvailable(true)
{
	int result = 0;

	mpv = mpv_create();

	static double zeroD = 0;
	static int64_t one = 1;
	static int64_t two = 2;
	static int64_t three = 3;
	static int32_t t = true;
	static int32_t f = false;
	static const char* yes = "yes";

	if(ProgramOptions::Instance.useHardwareDecoding){
		mpv_set_property(mpv, "hwdec", MPV_FORMAT_STRING, &yes);
	}

	result = mpv_initialize(mpv);
	assert(result == 0);

	mpv_set_option(mpv, "video-timing-offset", MPV_FORMAT_DOUBLE, &zeroD);
	mpv_set_option(mpv, "osc", MPV_FORMAT_FLAG, &t);
	mpv_set_option(mpv, "osd-bar", MPV_FORMAT_FLAG, &t);
	mpv_set_option(mpv, "term-osd-bar", MPV_FORMAT_FLAG, &t);
	mpv_set_option(mpv, "video-osd", MPV_FORMAT_FLAG, &t);
	{
		static double y = 0.95;
		mpv_set_option(mpv, "osd-bar-align-y", MPV_FORMAT_DOUBLE, &y);
	}
	{
		static double w = 95;
		mpv_set_option(mpv, "osd-bar-w", MPV_FORMAT_DOUBLE, &w);
	}
	mpv_set_option_string(mpv, "video-osd", "yes");
	mpv_set_option_string(mpv, "osc-visibility", "auto");

	mpv_set_option_string(mpv, "osd-on-seek", "msg-bar");
	mpv_set_option(mpv, "osd-level", MPV_FORMAT_INT64, &one);
	mpv_set_option_string(mpv, "player-operation-mode", "pseudo-gui");
	mpv_set_option_string(mpv, "keep-open", "yes");

	mpv_observe_property(mpv, 0, "file-format", MPV_FORMAT_STRING);
	mpv_observe_property(mpv, 0, "playlist", MPV_FORMAT_NODE);

	mpvGLParams.get_proc_address = GetProcAddress;

	mpv_render_param params[] = {
		{MPV_RENDER_PARAM_API_TYPE, (void*)MPV_RENDER_API_TYPE_OPENGL},
		{MPV_RENDER_PARAM_OPENGL_INIT_PARAMS, &mpvGLParams},
		{}
	};

	// This makes mpv use the currently set GL context. It will use the callback
	// (passed via params) to resolve GL builtin functions, as well as extensions.
	result = mpv_render_context_create(&mpvGLRenderContext, mpv, params);
	assert(result == 0);

	// We use events for thread-safe notification of the SDL main loop.
	// Generally, the wakeup callbacks (set further below) should do as least
	// work as possible, and merely wake up another thread to do actual work.
	// On SDL, waking up the mainloop is the ideal course of action. SDL's
	// SDL_PushEvent() is thread-safe, so we use that.
	wakeForMpvRedrawEventId = SDL_RegisterEvents(1);
	assert(wakeForMpvRedrawEventId >= 0);

	wakeForMpvEventsEventId = SDL_RegisterEvents(1);
	assert(wakeForMpvEventsEventId >= 0);

	// When normal mpv events are available.
	mpv_set_wakeup_callback(mpv, OnMpvEvents, this);

	// When a new frame should be drawn with mpv_opengl_cb_draw().
	// (Separate from the normal event handling mechanism for the sake of
	// users which run OpenGL on a different thread.)
	mpv_render_context_set_update_callback(mpvGLRenderContext, OnMpvRedraw, this);

	mpvEventThread = std::thread(&VideoWindow::processMpvEvents, this);
	mpvEventThreadShouldExit = false;
}

VideoWindow::~VideoWindow(){

	mpvEventThreadShouldExit = true;
	mpvEventsAvailable.set();

	if(mpvEventThread.joinable()){
		mpvEventThread.join();
	}

	// Destroy the GL renderer and all of the GL objects it allocated. If video
	// is still running, the video track will be deselected.
	mpv_render_context_free(mpvGLRenderContext);

	mpv_destroy(mpv);
}

void VideoWindow::showFormattedMessage(const char * formatString, ...){
	va_list args;
	va_start(args, formatString);
	std::string s(Jargon::StringUtilities::formatVarArgs(formatString, args));
	va_end(args);
	showMessage(s.c_str());
}

void VideoWindow::showMessage(const char* msg){
	const char* cmd[] = {"show-text", msg, 0};
	mpv_command(mpv, cmd);
}

void VideoWindow::startPlayAsync(const char* filename){
	const char *cmd[] = {"loadfile", filename, NULL};
	mpv_command(mpv, cmd);

	std::string baseFilename = Util::getBaseFilename(filename);
	setTitle(baseFilename.c_str());
}

void VideoWindow::enqueueFile(const char* filename){
	const char *cmd[] = {"loadfile", filename, "append-play", NULL};
	mpv_command(mpv, cmd);
}

void VideoWindow::playPause(){
	mpv_command(mpv, MpvCommands::TogglePlayPause);
}

void VideoWindow::ensureUnpaused(){
	uint32_t paused = false;
	mpv_get_property(mpv, "pause", MPV_FORMAT_FLAG, &paused);
	if (paused) {
		mpv_command(mpv, MpvCommands::TogglePlayPause);
	}
}

void VideoWindow::moveToQuadrant(int displayIndex, QuadrantLayout::WindowQuadrant quadrant){
	int displayCount = SDL_GetNumVideoDisplays();
	displayIndex %= displayCount;

	SDL_DisplayMode displayMode = {};
	SDL_GetCurrentDisplayMode(displayIndex, &displayMode);

	QuadrantLayout::Rect r = buildRectForQuadrant(quadrant, displayMode.w, displayMode.h);

	SDL_Rect displayBounds = {};
	SDL_GetDisplayBounds(displayIndex, &displayBounds);
	bool isOnTop = isAlwaysOnTop();

	SDL_SetWindowPosition(getSDLWindow(), r.left + displayBounds.x, r.top + displayBounds.y);
	SDL_SetWindowSize(getSDLWindow(), r.width, r.height);

	SDL_SetWindowBordered(getSDLWindow(), quadrant == QuadrantLayout::WindowQuadrant::Center ? SDL_TRUE : SDL_FALSE);
	SDL_SetWindowResizable(getSDLWindow(), SDL_TRUE);
}

void VideoWindow::moveToQuadrant(QuadrantLayout::WindowQuadrant quadrant){
	int currentDisplayIndex = SDL_GetWindowDisplayIndex(getSDLWindow());
	moveToQuadrant(currentDisplayIndex, quadrant);
}

void VideoWindow::zoomToActualSize(){
	uint64_t videoWidth;
	uint64_t videoHeight;
	mpv_get_property(mpv, "width", MPV_FORMAT_INT64, &videoWidth);
	mpv_get_property(mpv, "height", MPV_FORMAT_INT64, &videoHeight);

	int windowWidth;
	int windowHeight;
	SDL_GetWindowSize(getSDLWindow(), &windowWidth, &windowHeight);

	float widthZoomFactor = 1.f;
	float heightZoomFactor = 1.f;
	if(videoWidth > windowWidth){
		widthZoomFactor = (float)static_cast<uint32_t>(videoWidth) / (float)windowWidth;
	}
	if(videoHeight > windowHeight){
		heightZoomFactor = (float)static_cast<uint32_t>(videoHeight) / (float)windowHeight;
	}
	float zoomFactor = std::max(widthZoomFactor, heightZoomFactor);
	float logZoomFactor = std::log2f(zoomFactor);
	std::string zoomString = Jargon::StringUtilities::format("%f", logZoomFactor);

	const char* command[] = {"set", "video-zoom", zoomString.c_str(), 0};
	mpv_command(mpv, command);

	zoomedToActualSize = true;
}

void VideoWindow::resetZoom(){
	mpv_command(mpv, MpvCommands::PanResetX);
	mpv_command(mpv, MpvCommands::PanResetY);
	mpv_command(mpv, MpvCommands::ZoomReset);

	zoomedToActualSize = false;
}

void VideoWindow::toggleZoomToActualSize(){
	if(zoomedToActualSize){
		resetZoom();
	}else{
		zoomToActualSize();
	}
}

void VideoWindow::rotate(double delta){
	double rotation = 0;
	mpv_get_property(mpv, "video-rotate", MPV_FORMAT_DOUBLE, &rotation);
	double newRotation = rotation + delta;
	if(newRotation < 0){
		newRotation += 360;
	}else if(newRotation > 360){
		newRotation -= 360;
	}

	std::string rotateString;

	double modDelta = newRotation - rotation;
	if(modDelta >= 0){
		std::string rotateString = Jargon::StringUtilities::format("%f", modDelta);
		const char* command[] = {"add", "video-rotate", rotateString.c_str(), 0};
		mpv_command(mpv, command);
	}else{
		std::string rotateString = Jargon::StringUtilities::format("%f", modDelta);
		const char* command[] = {"add", "video-rotate", rotateString.c_str(), 0};
		mpv_command(mpv, command);
	}
}

std::string VideoWindow::getActiveFilename() const {
	const char* filename = mpv_get_property_string(mpv, "stream-open-filename");
	if (filename == nullptr){
		return "";
	}
	
	return std::string(filename);
}

void VideoWindow::enterFullscreen(){
	SDL_SetWindowFullscreen(getSDLWindow(), SDL_WINDOW_FULLSCREEN_DESKTOP);

	int currentDisplayIndex = SDL_GetWindowDisplayIndex(getSDLWindow());

	SDL_Rect displayBounds = {};
	SDL_GetDisplayBounds(currentDisplayIndex, &displayBounds);

	SDL_WarpMouseInWindow(getSDLWindow(), displayBounds.w, displayBounds.h / 2);
}

void VideoWindow::exitFullscreen(){
	SDL_SetWindowFullscreen(getSDLWindow(), 0);
}

void VideoWindow::minimize(){
	SDL_MinimizeWindow(getSDLWindow());
}

void VideoWindow::hideCursor(){
	SDL_Rect displayBounds = {};
	SDL_GetDisplayBounds(0, &displayBounds);
	SDL_WarpMouseGlobal(displayBounds.w, displayBounds.h / 2);
}

void VideoWindow::handleEvent(SDL_Event& event){
	switch(event.type){
		case SDL_WINDOWEVENT:
		{
			if(event.window.event == SDL_WINDOWEVENT_EXPOSED){
				redraw();
			}else if(event.window.event == SDL_WINDOWEVENT_CLOSE){
				mpv_command(mpv, MpvCommands::Stop);
			}
		}
		break;
		default:
			// Happens when a new video frame should be rendered, or if the
			// current frame has to be redrawn e.g. due to OSD changes.
			if(event.type == wakeForMpvRedrawEventId){
				redraw();
			}

			// Happens when at least 1 new event is in the mpv event queue.
			if(event.type == wakeForMpvEventsEventId){
				TraceLogging::Instance()->logMpvWakeupEvent(this);

				processMpvEvents();
			}
	}

	gamepadInputHandler.handleInput(this, mpv, event);
	keyboardInputHander.handleInput(this, mpv, event);
	mouseInputHandler.handleInput(this, mpv, event);
	touchInputHandler.handleInput(this, mpv, event);

	// call superclass
	SdlWindow::handleEvent(event);
}

void VideoWindow::processMpvEvents(){
	// Handle all queued mpv events.
	while (1){

		mpvEventsAvailable.wait();
		if(mpvEventThreadShouldExit){
			return;
		}

		while(1){
			mpv_event *mp_event = mpv_wait_event(mpv, 0);

			TraceLogging::Instance()->logMpvEvent(this, mp_event->event_id, mpv_event_name(mp_event->event_id));

			if(mp_event->event_id == MPV_EVENT_NONE){
				break;
			}else if(mp_event->event_id == MPV_EVENT_PLAYBACK_RESTART){
				//mpv_command(mpv, MpvCommands::ShowProgressTime);
				//mpv_command(mpv, MpvCommands::ShowProgressBar);
			}else if(mp_event->event_id == MPV_EVENT_SEEK){
				mpv_command(mpv, MpvCommands::ShowProgressTime);
				mpv_command(mpv, MpvCommands::ShowProgressBar);
			}else if(mp_event->event_id == MPV_EVENT_FILE_LOADED){
				const char* filename = mpv_get_property_string(mpv, "filename");

				bool skipImages = false;
				if(skipImages && Util::getFileExtension(filename) == "jpg"){
					mpv_command(mpv, MpvCommands::PlaylistNext);
				}else{
					setTitle(filename);

					if(zoomedToActualSize){
						resetZoom();
					}

					ensureUnpaused();
				}
			}else if(mp_event->event_id == MPV_EVENT_PROPERTY_CHANGE){
				const mpv_event_property* property = reinterpret_cast<mpv_event_property*>(mp_event->data);
				const char* propertyName = property->name;

				if(std::string("file-format") == propertyName){
					const char* fileFormat = mpv_get_property_string(mpv, "file-format");

					if(fileFormat != nullptr && std::string("mf") == fileFormat){
						//	mpv_command(mpv, MpvCommands::PlaylistNext);
					}
				}else if(std::string("playlist") == propertyName){
					const mpv_event_property* propertyData = (mpv_event_property*)mp_event->data;
					
					const mpv_node& node = *(mpv_node*)propertyData->data;
					playlistFilter.handlePlaylistChange(mpv, node);
				}
			}else if(mp_event->event_id == MPV_EVENT_SHUTDOWN){
				return;
			}
			{
				std::string msg = Jargon::StringUtilities::format("%s", mpv_event_name(mp_event->event_id));
				const char* args[] = {"show-text", msg.c_str(), 0};
				//mpv_command(mpv, args);
			}
		}
	}
}

void VideoWindow::redraw(){
	int w, h;
	SDL_GetWindowSize(getSDLWindow(), &w, &h);
	mpv_opengl_fbo fboOptions = {};
	fboOptions.fbo = 0;
	fboOptions.w = w;
	fboOptions.h = h;

	static int flipY = 1;

	mpv_render_param params[] = {
		// Specify the default framebuffer (0) as target. This will
		// render onto the entire screen. If you want to show the video
		// in a smaller rectangle or apply fancy transformations, you'll
		// need to render into a separate FBO and draw it manually.
		{MPV_RENDER_PARAM_OPENGL_FBO, &fboOptions},
		// Flip rendering (needed due to flipped GL coordinate system).
		{MPV_RENDER_PARAM_FLIP_Y, &flipY},
		{}
	};

	// See render_gl.h on what OpenGL environment mpv expects, and
	// other API details.
	SDL_GL_MakeCurrent(getSDLWindow(), getGLContext());
	mpv_render_context_render(mpvGLRenderContext, params);
	SDL_GL_SwapWindow(getSDLWindow());
	mpv_render_context_report_swap(mpvGLRenderContext);
}

void* VideoWindow::GetProcAddress(void *fn_ctx, const char *name){
	return SDL_GL_GetProcAddress(name);
}

void VideoWindow::OnMpvEvents(void *ctx){
	((VideoWindow*)ctx)->onMpvEvents();
}

void VideoWindow::OnMpvRedraw(void *ctx){
	((VideoWindow*)ctx)->onMpvRedraw();
}

void VideoWindow::onMpvEvents(){
	SDL_Event event = {};
	event.window.windowID = getWindowId();
	event.type = wakeForMpvEventsEventId;
	mpvEventsAvailable.set();
}

void VideoWindow::onMpvRedraw(){
	SDL_Event event = {};
	event.window.windowID = getWindowId();
	event.type = wakeForMpvRedrawEventId;
	SDL_PushEvent(&event);
}
