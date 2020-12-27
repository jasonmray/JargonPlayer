#include "VideoVR.h"
#include "MpvCommands.h"
#include "ProgramOptions.h"
#include "TraceLogging.h"
#include "Util.h"

#include "Jargon/StringUtilities.h"
#include "Jargon/Math/Utilities.h"

#include <algorithm>
#include <cassert>
#include <string>

#include <sdl/include/SDL.h>
#include <glew-2.1.0/include/GL/glew.h>
#include <sdl/include/SDL_opengl.h>

#include <libmpv/include/client.h>
#include <libmpv/include/render_gl.h>

#include <windows.h>

#pragma comment (lib, "sdl/lib/x64/sdl2.lib")
#pragma comment (lib, "libmpv/lib/x64/mpv.lib")
//#pragma comment (lib, "angle/lib/x64/libGLESv2.lib")
#pragma comment (lib, "angle/lib/x64/libANGLE.lib")
#pragma comment (lib, "opengl32.lib")
#pragma comment (lib, "glew-2.1.0/lib/Release/x64/glew32.lib")

#define QUOTE(...) #__VA_ARGS__

bool compileShader(GLenum type, const char* source, GLuint* shaderIdOut){
	GLuint shaderId = glCreateShader(type);
	glShaderSource(shaderId, 1, &source, NULL);
	glCompileShader(shaderId);

	GLint status;
	glGetShaderiv(shaderId, GL_COMPILE_STATUS, &status);

	if(status != GL_TRUE){
		char infoLog[1024] = {0};
		glGetShaderInfoLog(shaderId, 1024, NULL, infoLog);
		
		Util::log("shader comp failed: %s\n", infoLog);

		return false;
	}
	*shaderIdOut = shaderId;
	return true;
}

class Renderer {
public:
	Renderer(uint32_t renderWidth, uint32_t renderHeight):
		renderWidth(renderWidth),
		renderHeight(renderHeight)
	{
		shaderProgram = glCreateProgram();

		const char* vertexSrc = R"glsl(
			#version 140
			in vec3 position;
			in vec2 uv;

			out vec2 TexCoord;

			void main()
			{
				gl_Position = vec4(position.x, position.y, 0.0, 1.0);
				TexCoord = uv; //(vec2(1.f, 1.f) + position.xy) / 2.f;
				//TexCoord.x /= 2.f;
			}
		)glsl";

		GLuint vertexShader = 0;
		if (!compileShader(GL_VERTEX_SHADER, vertexSrc, &vertexShader)) {
			throw "Vertex shader error";
		}


		const char* fragmentSrc = R"glsl(
			#version 140
			in vec2 TexCoord;
			out vec4 LFragment;
			
			uniform sampler2D videoTexture;

			void main()
			{
				//LFragment = vec4( 1.0, 0.0, 1.0, 1.0 );
				LFragment = texture(videoTexture, TexCoord);
			}
		)glsl";

		GLuint fragmentShader = 0;
		if (!compileShader(GL_FRAGMENT_SHADER, fragmentSrc, &fragmentShader)) {
			throw "Fragment shader error";
		}


		glAttachShader(shaderProgram, vertexShader);
		glAttachShader(shaderProgram, fragmentShader);

		glLinkProgram(shaderProgram);

		GLint programSuccess = GL_TRUE;
		glGetProgramiv(shaderProgram, GL_LINK_STATUS, &programSuccess);
		if (programSuccess != GL_TRUE) {
			char infoLog[1024] = {};
			glGetProgramInfoLog(shaderProgram, 1024, NULL, infoLog);

			Util::log("shader link failed: %s\n", infoLog);

			throw "Error linking shader program";
		}

		posAttrib = glGetAttribLocation(shaderProgram, "position");
		if (posAttrib == -1) {
			throw "Bad shader variable";
		}
		uvAttrib = glGetAttribLocation(shaderProgram, "uv");
		if (uvAttrib == -1) {
			throw "Bad shader variable";
		}
		//glVertexAttribPointer(posAttrib, 2, GL_FLOAT, GL_FALSE, 0, 0);
		//glEnableVertexAttribArray(posAttrib);

		glClearColor(1.0f, 0.f, 0.5f, 1.f);

		GLfloat vertexData[] =
		{
			-1.f, -1.f,  0.f,     0.f,  0.f,  // x,y,z,   u,v
			 1.f, -1.f,  0.f,     1.f,  0.f,
			 1.f,  1.f,  0.f,     1.f,  1.f,
			-1.f,  1.f,  0.f,     0.f,  1.f,
		};


		glGenBuffers(1, &vbo);
		if (vbo == 0) {
			GLenum error = glGetError();
			Util::log("gl error: %d\n", error);
		}
		glBindBuffer(GL_ARRAY_BUFFER, vbo);
		glBufferData(GL_ARRAY_BUFFER, 5 * 4 * sizeof(GLfloat), vertexData, GL_STATIC_DRAW);

		GLuint indexData[] = {0, 1, 2, 3};
		glGenBuffers(1, &ibo);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, 4 * sizeof(GLuint), indexData, GL_STATIC_DRAW);


		//glUseProgram(shaderProgram);

		// https://open.gl/drawing

		// create left framebuffer
		{
			glGenFramebuffers(1, &fboLeft);
			glBindFramebuffer(GL_FRAMEBUFFER, fboLeft);

			glGenTextures(1, &texture);
			glBindTexture(GL_TEXTURE_2D, texture);
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, renderWidth, renderHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

			glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, texture, 0);
			glBindTexture(GL_TEXTURE_2D, 0);

			GLenum fbStatus = glCheckFramebufferStatus(GL_FRAMEBUFFER);
			if (fbStatus == GL_FRAMEBUFFER_COMPLETE) {
				Util::log("framebuffer success");
			} else {
				GLenum error = glGetError();
				Util::log("error %u\n", error);
			}
			glBindFramebuffer(GL_FRAMEBUFFER, 0);
		}

	}

	void renderLeft(SDL_Window* sdlWindow, SDL_GLContext& glContext, mpv_render_context* mpvRenderContext) {
		mpv_opengl_fbo fboOptions = {};
		fboOptions.fbo = fboLeft;
		fboOptions.w = renderWidth;
		fboOptions.h = renderHeight;

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
		SDL_GL_MakeCurrent(sdlWindow, glContext);
		mpv_render_context_render(mpvRenderContext, params);

		glClear(GL_COLOR_BUFFER_BIT);

		glUseProgram(shaderProgram);
		glEnableVertexAttribArray(posAttrib);
		glEnableVertexAttribArray(uvAttrib);

		//Set vertex data
		glBindBuffer(GL_ARRAY_BUFFER, vbo);
		glVertexAttribPointer(posAttrib, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (const void *)(0));
		glVertexAttribPointer(uvAttrib, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (const void*)(3 * sizeof(GLfloat)));

		//Set index data
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);


		glBindTexture(GL_TEXTURE_2D, texture);
		glDrawElements(GL_TRIANGLE_FAN, 4, GL_UNSIGNED_INT, NULL);

		glDisableVertexAttribArray(posAttrib);
		glDisableVertexAttribArray(uvAttrib);
		glUseProgram(NULL);
		
		//SDL_GL_SwapWindow(sdlWindow);
		//mpv_render_context_report_swap(mpvRenderContext);
	}

private:
	GLuint fboLeft = 0;
	GLuint fboRight = 0;
	uint32_t renderWidth = 0;
	uint32_t renderHeight = 0;
	GLuint shaderProgram = 0;
	GLuint texture = 0;
	GLuint vbo = 0;
	GLuint ibo = 0;
	GLint posAttrib = 0;
	GLint uvAttrib = 0;

};

std::unique_ptr<Renderer> gRenderer;

const char* VideoVR::DefaultWindowTitle = "JargonPlayer";
const int VideoVR::DefaultWindowWidth = 800;
const int VideoVR::DefaultWindowHeight = 450;

VideoVR::VideoVR():
	SdlWindow(DefaultWindowTitle, DefaultWindowWidth, DefaultWindowHeight),
	mpvEventsAvailable(true)
{
	SDL_GL_MakeCurrent(getSDLWindow(), getGLContext());

	GLenum glewError = glewInit();
	if (glewError != GLEW_OK) {
		Util::log("Error initializing GLEW! %s\n", glewGetErrorString(glewError));
	}

	SDL_Renderer* renderer = SDL_CreateRenderer(getSDLWindow(), -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_TARGETTEXTURE);


	int result = 0;

	mpv = mpv_create();

	static double zeroD = 0;
	static int64_t one = 1;
	static int64_t two = 2;
	static int64_t three = 3;
	static bool t = true;
	static bool f = false;
	static const char* yes = "yes";

	mpv_set_property(mpv, "hwdec", MPV_FORMAT_STRING, &yes);

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


	if (SDL_GL_MakeCurrent(getSDLWindow(), getGLContext()) != 0) {
		Util::log("Failed to switch GL context %s\n", SDL_GetError());
	}

	gRenderer = std::make_unique<Renderer>(DefaultWindowWidth, DefaultWindowHeight);


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

	mpvEventThread = std::thread(&VideoVR::processMpvEvents, this);
	mpvEventThreadShouldExit = false;
}

VideoVR::~VideoVR(){


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

void VideoVR::showFormattedMessage(const char * formatString, ...){
	va_list args;
	va_start(args, formatString);
	std::string s(Jargon::StringUtilities::formatVarArgs(formatString, args));
	va_end(args);
	showMessage(s.c_str());
}

void VideoVR::showMessage(const char* msg){
	const char* cmd[] = {"show-text", msg, 0};
	mpv_command(mpv, cmd);
}

void VideoVR::startPlayAsync(const char* filename){
	const char *cmd[] = {"loadfile", filename, NULL};
	mpv_command(mpv, cmd);

	std::string baseFilename = Util::getBaseFilename(filename);
	setTitle(baseFilename.c_str());
}

void VideoVR::enqueueFile(const char* filename){
	const char *cmd[] = {"loadfile", filename, "append-play", NULL};
	mpv_command(mpv, cmd);
}

void VideoVR::playPause(){
	mpv_command(mpv, MpvCommands::TogglePlayPause);
}

void VideoVR::hideCursor(){
	SDL_Rect displayBounds = {};
	SDL_GetDisplayBounds(0, &displayBounds);
	SDL_WarpMouseGlobal(displayBounds.w, displayBounds.h / 2);
}

void VideoVR::handleEvent(SDL_Event& event){
	switch (event.type){
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

	//gamepadInputHandler.handleInput(this, mpv, event);
	//keyboardInputHander.handleInput(this, mpv, event);
	//mouseInputHandler.handleInput(this, mpv, event);
	//touchInputHandler.handleInput(this, mpv, event);

	// call superclass
	SdlWindow::handleEvent(event);
}

void VideoVR::processMpvEvents(){
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

void VideoVR::redraw(){
	gRenderer->renderLeft(getSDLWindow(), getGLContext(), mpvGLRenderContext);
	SDL_GL_SwapWindow(getSDLWindow());

}

void *VideoVR::GetProcAddress(void *fn_ctx, const char *name){
	return SDL_GL_GetProcAddress(name);
}

void VideoVR::OnMpvEvents(void *ctx){
	((VideoVR*)ctx)->onMpvEvents();
}

void VideoVR::OnMpvRedraw(void *ctx){
	((VideoVR*)ctx)->onMpvRedraw();
}

void VideoVR::onMpvEvents(){
	SDL_Event event = {};
	event.window.windowID = getWindowId();
	event.type = wakeForMpvEventsEventId;
	mpvEventsAvailable.set();
}

void VideoVR::onMpvRedraw(){
	SDL_Event event = {};
	event.window.windowID = getWindowId();
	event.type = wakeForMpvRedrawEventId;
	SDL_PushEvent(&event);
}

GLuint gProgramID = 0;
GLint gVertexPos2DLocation = 0;
GLuint gVBO = 0;
GLuint gIBO = 0;

bool initGL() {
	//Success flag
	bool success = true;

	//Generate program
	gProgramID = glCreateProgram();

	//Create vertex shader
	GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);

	//Get vertex source
	const GLchar* vertexShaderSource[] =
	{
		//"#version 140\nin vec2 LVertexPos2D; void main() { gl_Position = vec4( LVertexPos2D.x, LVertexPos2D.y, 0, 1 ); }"

		R"glsl(
			#version 140
			in vec2 LVertexPos2D;

			//out vec2 TexCoord;

			void main()
			{
				gl_Position = vec4(LVertexPos2D.x, LVertexPos2D.y, 0.0, 1.0);
				//TexCoord = position;
			}
		)glsl"
	};

	//Set vertex source
	glShaderSource(vertexShader, 1, vertexShaderSource, NULL);

	//Compile vertex source
	glCompileShader(vertexShader);

	//Check vertex shader for errors
	GLint vShaderCompiled = GL_FALSE;
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &vShaderCompiled);
	if (vShaderCompiled != GL_TRUE) {
		printf("Unable to compile vertex shader %d!\n", vertexShader);
		success = false;
	} else {
		//Attach vertex shader to program
		glAttachShader(gProgramID, vertexShader);


		//Create fragment shader
		GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);

		//Get fragment source
		const GLchar* fragmentShaderSource[] =
		{
			//"#version 140\nout vec4 LFragment; void main() { LFragment = vec4( 1.0, 0.0, 1.0, 1.0 ); }"
			R"glsl(
				#version 140
			
				out vec4 LFragment;
			
				void main()
				{
					LFragment = vec4( 1.0, 0.0, 1.0, 1.0 );
				}
			)glsl"
		};

		//Set fragment source
		glShaderSource(fragmentShader, 1, fragmentShaderSource, NULL);

		//Compile fragment source
		glCompileShader(fragmentShader);

		//Check fragment shader for errors
		GLint fShaderCompiled = GL_FALSE;
		glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &fShaderCompiled);
		if (fShaderCompiled != GL_TRUE) {
			printf("Unable to compile fragment shader %d!\n", fragmentShader);
			success = false;
		} else {
			//Attach fragment shader to program
			glAttachShader(gProgramID, fragmentShader);


			//Link program
			glLinkProgram(gProgramID);

			//Check for errors
			GLint programSuccess = GL_TRUE;
			glGetProgramiv(gProgramID, GL_LINK_STATUS, &programSuccess);
			if (programSuccess != GL_TRUE) {
				printf("Error linking program %d!\n", gProgramID);
				success = false;
			} else {
				//Get vertex attribute location
				gVertexPos2DLocation = glGetAttribLocation(gProgramID, "LVertexPos2D");
				if (gVertexPos2DLocation == -1) {
					printf("LVertexPos2D is not a valid glsl program variable!\n");
					success = false;
				} else {
					//Initialize clear color
					glClearColor(1.0f, 0.f, 0.5f, 1.f);

					//VBO data
					GLfloat vertexData[] =
					{
						-0.5f, -0.5f,
						0.5f, -0.5f,
						0.5f, 0.5f,
						-0.5f, 0.5f
					};

					//IBO data
					GLuint indexData[] = {0, 1, 2, 3};

					//Create VBO
					glGenBuffers(1, &gVBO);
					glBindBuffer(GL_ARRAY_BUFFER, gVBO);
					glBufferData(GL_ARRAY_BUFFER, 2 * 4 * sizeof(GLfloat), vertexData, GL_STATIC_DRAW);

					//Create IBO
					glGenBuffers(1, &gIBO);
					glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, gIBO);
					glBufferData(GL_ELEMENT_ARRAY_BUFFER, 4 * sizeof(GLuint), indexData, GL_STATIC_DRAW);
				}
			}
		}
	}

	return success;
}

void render() {
	//Clear color buffer
	glClear(GL_COLOR_BUFFER_BIT);

		//Bind program
		glUseProgram(gProgramID);

		//Enable vertex position
		glEnableVertexAttribArray(gVertexPos2DLocation);

		//Set vertex data
		glBindBuffer(GL_ARRAY_BUFFER, gVBO);
		glVertexAttribPointer(gVertexPos2DLocation, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(GLfloat), NULL);

		//Set index data and render
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, gIBO);
		glDrawElements(GL_TRIANGLE_FAN, 4, GL_UNSIGNED_INT, NULL);

		//Disable vertex position
		glDisableVertexAttribArray(gVertexPos2DLocation);

		//Unbind program
		glUseProgram(NULL);

}


void tryGlCrap() {
	SDL_Window *Window = SDL_CreateWindow("OpenGL Test", 0, 0, 800, 600, SDL_WINDOW_OPENGL);
	assert(Window);
	SDL_GLContext Context = SDL_GL_CreateContext(Window);

	glewExperimental = GL_TRUE;
	GLenum glewError = glewInit();
	if (glewError != GLEW_OK) {
		printf("Error initializing GLEW! %s\n", glewGetErrorString(glewError));
	}

	// vsync
	if (SDL_GL_SetSwapInterval(1) < 0) {
		printf("Warning: Unable to set VSync! SDL Error: %s\n", SDL_GetError());
	}

	initGL();

	while(1){

		SDL_Event Event;
		while (SDL_PollEvent(&Event)) {
			if (Event.type == SDL_KEYDOWN) {
				switch (Event.key.keysym.sym) {
					case SDLK_ESCAPE:
						return;
						break;
					default:
						break;
				}
			} else if (Event.type == SDL_QUIT) {
				return;
			}
		}

		render();

		SDL_GL_SwapWindow(Window);
	}
}

int appmainvr(int argc, const char *argv[]){
	//SDL_Init(SDL_INIT_VIDEO | SDL_INIT_GAMECONTROLLER);
	if (SDL_Init(SDL_INIT_EVERYTHING) < 0) {
		return 1;
	}
	SDL_DisableScreenSaver();
	SDL_JoystickEventState(SDL_ENABLE);

	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 1);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
	//SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_ACCELERATED_VISUAL, 1);

	SDL_GameController* gameController = SDL_GameControllerOpen(0);

	ProgramOptions& programOptions = ProgramOptions::Instance;

	if (!programOptions.processOptions(argc, argv)) {
		return 1;
	}

	//tryGlCrap();

	const size_t fileCount = programOptions.files.size();
	std::shared_ptr<VideoVR> player = std::make_shared<VideoVR>();
	
	if (fileCount > 0) {
		const char* filename = programOptions.files[0];
		player->startPlayAsync(filename);
	}

	while(1){
		SDL_Event event = {};
		if(SDL_WaitEvent(&event) != 1){
			break;
		}
		if(event.type == SDL_QUIT){
			break;
		}
		if(event.type == SDL_KEYDOWN){
			if(event.key.keysym.sym == SDLK_SPACE && event.key.keysym.mod & KMOD_CTRL){
				player->playPause();
			}
		}
	
		player->handleEvent(event);
	
		if(event.type == SDL_WINDOWEVENT){
			if(event.window.event == SDL_WINDOWEVENT_CLOSE){
				player->close();
				break;
			}
		}
	}

	SDL_Quit();
	return 0;
}
