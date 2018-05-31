/*
 * Copyright (c) 2015 Jeff Boody
 *
 * Permission is hereby granted, free of charge, to any person obtaining a
 * copy of this software and associated documentation files (the "Software"),
 * to deal in the Software without restriction, including without limitation
 * the rights to use, copy, modify, merge, publish, distribute, sublicense,
 * and/or sell copies of the Software, and to permit persons to whom the
 * Software is furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included
 * in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 *
 */

#include <stdlib.h>
#include <assert.h>
#include "a3d/a3d_timestamp.h"
#include "a3d/widget/a3d_key.h"
#include "gears_renderer.h"

#define LOG_TAG "gears"
#include "a3d/a3d_log.h"

/***********************************************************
* private                                                  *
***********************************************************/

static gears_renderer_t* gears_renderer = NULL;

static int           gRunning = 1;
static SDL_Window*   gWindow = NULL;
static SDL_GLContext gContext;

static void cmd_fn(int cmd, const char* msg)
{
	if(cmd == GEARS_CMD_EXIT)
	{
		gRunning = 0;
	}
}

static int keyPress(SDL_Keysym* keysym,
                    int* keycode, int* meta)
{
	assert(keysym);
	assert(keycode);
	assert(meta);

	// convert the keycode
	// TODO - INSERT
	*keycode = 0;
	*meta    = 0;
	if((keysym->sym >= SDLK_a) &&
	   (keysym->sym <= SDLK_z))
	{
		int a = (int) 'a';
		*keycode = a + (keysym->sym - SDLK_a);
	}
	else if((keysym->sym >= SDLK_0) &&
	        (keysym->sym <= SDLK_9))
	{
		int z = (int) '0';
		*keycode = z + (keysym->sym - SDLK_0);
	}
	else if((keysym->sym >= SDLK_KP_0) &&
	        (keysym->sym <= SDLK_KP_9))
	{
		int z = (int) '0';
		*keycode = z + (keysym->sym - SDLK_KP_0);
	}
	else if((keysym->sym == SDLK_RETURN) ||
	        (keysym->sym == SDLK_KP_ENTER))
	{
		*keycode = A3D_KEY_ENTER;
	}
	else if(keysym->sym == SDLK_ESCAPE)
	{
		*keycode = A3D_KEY_ESCAPE;
	}
	else if(keysym->sym == SDLK_BACKSPACE)
	{
		*keycode = A3D_KEY_BACKSPACE;
	}
	else if(keysym->sym == SDLK_DELETE)
	{
		*keycode = A3D_KEY_DELETE;
	}
	else if(keysym->sym == SDLK_UP)
	{
		*keycode = A3D_KEY_UP;
	}
	else if(keysym->sym == SDLK_DOWN)
	{
		*keycode = A3D_KEY_DOWN;
	}
	else if(keysym->sym == SDLK_LEFT)
	{
		*keycode = A3D_KEY_LEFT;
	}
	else if(keysym->sym == SDLK_RIGHT)
	{
		*keycode = A3D_KEY_RIGHT;
	}
	else if(keysym->sym == SDLK_HOME)
	{
		*keycode = A3D_KEY_HOME;
	}
	else if(keysym->sym == SDLK_END)
	{
		*keycode = A3D_KEY_END;
	}
	else if(keysym->sym == SDLK_PAGEUP)
	{
		*keycode = A3D_KEY_PGUP;
	}
	else if(keysym->sym == SDLK_PAGEDOWN)
	{
		*keycode = A3D_KEY_PGDOWN;
	}
	else if(keysym->sym == SDLK_ESCAPE)
	{
		*keycode = A3D_KEY_ESCAPE;
	}
	else if(keysym->sym == SDLK_BACKSPACE)
	{
		*keycode = (int) '\b';
	}
	else if(keysym->sym == SDLK_TAB)
	{
		*keycode = (int) '\t';
	}
	else if(keysym->sym == SDLK_SPACE)
	{
		*keycode = (int) ' ';
	}
	else if(keysym->sym == SDLK_EXCLAIM)
	{
		*keycode = (int) '!';
	}
	else if(keysym->sym == SDLK_QUOTEDBL)
	{
		*keycode = (int) '"';
	}
	else if(keysym->sym == SDLK_HASH)
	{
		*keycode = (int) '#';
	}
	else if(keysym->sym == SDLK_DOLLAR)
	{
		*keycode = (int) '$';
	}
	else if(keysym->sym == SDLK_AMPERSAND)
	{
		*keycode = (int) '&';
	}
	else if(keysym->sym == SDLK_QUOTE)
	{
		*keycode = (int) '\'';
	}
	else if(keysym->sym == SDLK_LEFTPAREN)
	{
		*keycode = (int) '(';
	}
	else if(keysym->sym == SDLK_RIGHTPAREN)
	{
		*keycode = (int) ')';
	}
	else if((keysym->sym == SDLK_ASTERISK) ||
	        (keysym->sym == SDLK_KP_MULTIPLY))
	{
		*keycode = (int) '*';
	}
	else if((keysym->sym == SDLK_PLUS) ||
	        (keysym->sym == SDLK_KP_PLUS))
	{
		*keycode = (int) '+';
	}
	else if(keysym->sym == SDLK_COMMA)
	{
		*keycode = (int) ',';
	}
	else if((keysym->sym == SDLK_MINUS) ||
	        (keysym->sym == SDLK_KP_MINUS))
	{
		*keycode = (int) '-';
	}
	else if((keysym->sym == SDLK_PERIOD) ||
	        (keysym->sym == SDLK_KP_PERIOD))
	{
		*keycode = (int) '.';
	}
	else if((keysym->sym == SDLK_SLASH) ||
	        (keysym->sym == SDLK_KP_DIVIDE))
	{
		*keycode = (int) '/';
	}
	else if(keysym->sym == SDLK_COLON)
	{
		*keycode = (int) ':';
	}
	else if(keysym->sym == SDLK_SEMICOLON)
	{
		*keycode = (int) ';';
	}
	else if(keysym->sym == SDLK_LESS)
	{
		*keycode = (int) '<';
	}
	else if((keysym->sym == SDLK_EQUALS) ||
	        (keysym->sym == SDLK_KP_EQUALS))
	{
		*keycode = (int) '=';
	}
	else if(keysym->sym == SDLK_GREATER)
	{
		*keycode = (int) '>';
	}
	else if(keysym->sym == SDLK_QUESTION)
	{
		*keycode = (int) '?';
	}
	else if(keysym->sym == SDLK_AT)
	{
		*keycode = (int) '@';
	}
	else if(keysym->sym == SDLK_LEFTBRACKET)
	{
		*keycode = (int) '[';
	}
	else if(keysym->sym == SDLK_BACKSLASH)
	{
		*keycode = (int) '\\';
	}
	else if(keysym->sym == SDLK_RIGHTBRACKET)
	{
		*keycode = (int) ']';
	}
	else if(keysym->sym == SDLK_CARET)
	{
		*keycode = (int) '^';
	}
	else if(keysym->sym == SDLK_UNDERSCORE)
	{
		*keycode = (int) '_';
	}
	else if(keysym->sym == SDLK_BACKQUOTE)
	{
		*keycode = (int) '`';
	}
	else if(keysym->sym == SDLK_BACKQUOTE)
	{
		*keycode = (int) '`';
	}
	else
	{
		// ignore
		LOGD("sym=0x%X", keysym->sym);
		return 0;
	}

	// convert the meta
	if(keysym->mod & KMOD_ALT)
	{
		*meta |= A3D_KEY_ALT;
	}
	if(keysym->mod & KMOD_LALT)
	{
		*meta |= A3D_KEY_ALT_L;
	}
	if(keysym->mod & KMOD_RALT)
	{
		*meta |= A3D_KEY_ALT_R;
	}
	if(keysym->mod & KMOD_CTRL)
	{
		*meta |= A3D_KEY_CTRL;
	}
	if(keysym->mod & KMOD_LCTRL)
	{
		*meta |= A3D_KEY_CTRL_L;
	}
	if(keysym->mod & KMOD_RCTRL)
	{
		*meta |= A3D_KEY_CTRL_R;
	}
	if(keysym->mod & KMOD_SHIFT)
	{
		*meta |= A3D_KEY_SHIFT;
	}
	if(keysym->mod & KMOD_LSHIFT)
	{
		*meta |= A3D_KEY_SHIFT_L;
	}
	if(keysym->mod & KMOD_RSHIFT)
	{
		*meta |= A3D_KEY_SHIFT_R;
	}
	if(keysym->mod & KMOD_CAPS)
	{
		*meta |= A3D_KEY_CAPS;
	}

	return 1;
}
/***********************************************************
* main                                                     *
***********************************************************/

int main(int argc, char** argv)
{
	SDL_version version;
	SDL_VERSION(&version);
	LOGI("SDL %i.%i.%i", version.major, version.minor, version.patch);

	if(SDL_Init(SDL_INIT_VIDEO) < 0)
	{
		LOGE("SDL_Init failed %s", SDL_GetError());
		return EXIT_FAILURE;
	}

	if(SDL_GetNumVideoDisplays() <= 0)
	{
		LOGE("SDL_GetNumVideoDisplays failed %s", SDL_GetError());
		goto fail_displays;
	}

	// set the default screen size
	SDL_DisplayMode dpy;
	int   screen_width      = 1920;
	int   screen_height     = 1080;
	float screen_density    = 1.0;
	int   screen_fullscreen = 1;
	if(SDL_GetCurrentDisplayMode(0, &dpy) == 0)
	{
		screen_width  = dpy.w;
		screen_height = dpy.h;
	}

	// override the default screen size
	FILE* f = fopen("sdl.cfg", "r");
	if(f)
	{
		if(fscanf(f, "%i %i %f %i",
		          &screen_width,
		          &screen_height,
		          &screen_density,
		          &screen_fullscreen) != 4)
		{
			LOGW("fscanf failed");
		}
		fclose(f);
	}

	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 2);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 1);
	SDL_GL_SetAttribute(SDL_GL_RED_SIZE, 5);
	SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, 6);
	SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE, 5);
	SDL_GL_SetAttribute(SDL_GL_ALPHA_SIZE, 0);
	SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 16);

	#ifdef __APPLE__
	int screen_flags = (screen_fullscreen ? SDL_WINDOW_FULLSCREEN_DESKTOP : 0) |
	                   SDL_WINDOW_ALLOW_HIGHDPI |
	                   SDL_WINDOW_RESIZABLE     |
	                   SDL_WINDOW_OPENGL        |
	                   SDL_WINDOW_SHOWN;
	int screen_scale = 2;
	#else
	int screen_flags = (screen_fullscreen ? SDL_WINDOW_FULLSCREEN : 0) |
	                   SDL_WINDOW_RESIZABLE |
	                   SDL_WINDOW_OPENGL    |
	                   SDL_WINDOW_SHOWN;
	int screen_scale = 1;
	#endif
	gWindow = SDL_CreateWindow("gears2",
	                           SDL_WINDOWPOS_UNDEFINED,
	                           SDL_WINDOWPOS_UNDEFINED,
	                           screen_width,
	                           screen_height,
	                           screen_flags);
	if(gWindow == NULL)
	{
		LOGE("SDL_CreateWindow failed: %s", SDL_GetError());
		goto fail_window;
	}

	gContext = SDL_GL_CreateContext(gWindow);
	if(gContext == NULL)
	{
		LOGE("SDL_GL_CreateContext failed: %s", SDL_GetError());
		goto fail_context;
	}

	if(SDL_GL_SetSwapInterval(1) < 0)
	{
		LOGW("SDL_GL_SetSwapInterval failed: %s", SDL_GetError());
	}

	// Initialize Glew
	#ifndef __APPLE__
		GLenum glew_error = glewInit();
		if(GLEW_OK != glew_error)
		{
			LOGE("%s", glewGetErrorString(glew_error));
			goto fail_context;
		}
	#endif

	// Initialize gears renderer
	gears_renderer = gears_renderer_new(cmd_fn);
	if(gears_renderer == NULL)
	{
		goto fail_renderer;
	}

	SDL_GL_GetDrawableSize(gWindow, &screen_width, &screen_height);
	gears_renderer_resize(gears_renderer, screen_width, screen_height);
	gears_renderer_density(gears_renderer, screen_density);

	// main loop
	while(gRunning)
	{
		SDL_Event e;
		while(SDL_PollEvent(&e))
		{
			int keycode = 0;
			int meta    = 0;
			if((e.type == SDL_KEYUP) ||
			   ((e.type == SDL_KEYDOWN) && (e.key.repeat)))
			{
				if(keyPress(&e.key.keysym, &keycode, &meta))
				{
					gears_renderer_keyPress(gears_renderer,
					                        keycode, meta);
				}
			}
			else if(e.type == SDL_MOUSEBUTTONUP)
			{
				float  x  = (float) screen_scale*e.button.x;
				float  y  = (float) screen_scale*e.button.y;
				double ts = a3d_timestamp();
				gears_renderer_touch(gears_renderer,
				                     GEARS_TOUCH_ACTION_UP,
				                     1, ts, x, y,
				                     0.0f, 0.0f,
				                     0.0f, 0.0f,
				                     0.0f, 0.0f);
			}
			else if(e.type == SDL_MOUSEBUTTONDOWN)
			{
				float  x  = (float) screen_scale*e.button.x;
				float  y  = (float) screen_scale*e.button.y;
				double ts = a3d_timestamp();
				gears_renderer_touch(gears_renderer,
				                     GEARS_TOUCH_ACTION_DOWN,
				                     1, ts, x, y,
				                     0.0f, 0.0f,
				                     0.0f, 0.0f,
				                     0.0f, 0.0f);
			}
			else if(e.type == SDL_MOUSEMOTION)
			{
				float  x  = (float) screen_scale*e.button.x;
				float  y  = (float) screen_scale*e.button.y;
				double ts = a3d_timestamp();
				gears_renderer_touch(gears_renderer,
				                     GEARS_TOUCH_ACTION_MOVE,
				                     1, ts, x, y,
				                     0.0f, 0.0f,
				                     0.0f, 0.0f,
				                     0.0f, 0.0f);
			}
			else if(e.type == SDL_WINDOWEVENT)
			{
				gears_renderer_resize(gears_renderer,
				                      screen_scale*e.window.data1,
				                      screen_scale*e.window.data2);
			}
			else if(e.type == SDL_QUIT)
			{
				gRunning = 0;
			}
		}

		gears_renderer_draw(gears_renderer);
		SDL_GL_SwapWindow(gWindow);
	}

	// success
	gears_renderer_delete(&gears_renderer);
	SDL_GL_DeleteContext(gContext);
	SDL_DestroyWindow(gWindow);
	SDL_Quit();
	return EXIT_SUCCESS;

	// failure
	fail_renderer:
		SDL_GL_DeleteContext(gContext);
	fail_context:
		SDL_DestroyWindow(gWindow);
	fail_window:
	fail_displays:
		SDL_Quit();
	return EXIT_FAILURE;
}
