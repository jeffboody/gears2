/*
 * Copyright (c) 2012 Jeff Boody
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

#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include "SDL.h"
#include <GL/glew.h>
#include <GL/gl.h>
#include "gears_renderer.h"

#define LOG_TAG "gears"
#include "a3d/a3d_log.h"

/***********************************************************
* private                                                  *
***********************************************************/

static gears_renderer_t* gears_renderer = NULL;

// SDL
SDL_Surface* Surface;
SDL_Event Event;
const SDL_VideoInfo* Video_Info;
int Video_Flags = SDL_OPENGL | SDL_RESIZABLE;

#define WIDTH  640
#define HEIGHT 480

void Quit()
{
	if(gears_renderer)
	{
		gears_renderer_delete(&gears_renderer);
	}
	if(Surface && (Surface->flags & SDL_FULLSCREEN))
	{
		SDL_WM_ToggleFullScreen(Surface);
	}
	SDL_Quit();
}

int main( int argc, char ** argv )
{
	// Initialize SDL
	if(SDL_Init(SDL_INIT_VIDEO) < 0)
	{
		LOGE("%s", SDL_GetError());
		return EXIT_FAILURE;
	}
	atexit(Quit);
	if(!(Surface = SDL_SetVideoMode(WIDTH, HEIGHT, 24, Video_Flags)))
	{
		LOGE("%s", SDL_GetError());
		return EXIT_FAILURE;
	}
	SDL_WM_SetCaption("gears2-sdl", "gears2-sdl");

	// Initialize Glew
	GLenum glew_error = glewInit();
	if(GLEW_OK != glew_error)
	{
		LOGE("%s", glewGetErrorString(glew_error));
		return EXIT_FAILURE;
	}

	// Initialize gears renderer
	gears_renderer = gears_renderer_new("whitrabt.texgz");
	if(gears_renderer == NULL)
	{
		return EXIT_FAILURE;
	}
	gears_renderer_resize(gears_renderer, WIDTH, HEIGHT);

	// Main Loop
	while(1)
	{
		while(SDL_PollEvent(&Event))
		{
			if(Event.type == SDL_KEYDOWN)
			{
				if(Event.key.keysym.sym == SDLK_F1)
					SDL_WM_ToggleFullScreen(Surface);
				else if(Event.key.keysym.sym == SDLK_ESCAPE)
					return EXIT_SUCCESS;
				Video_Info = SDL_GetVideoInfo();
				gears_renderer_resize(gears_renderer, Video_Info->current_w, Video_Info->current_h);
			}
			else if(Event.type == SDL_VIDEORESIZE)
			{
				if(!(Surface = SDL_SetVideoMode(Event.resize.w, Event.resize.h, 24, Video_Flags)))
				{
					LOGE("%s", SDL_GetError());
					return EXIT_FAILURE;
				}
				gears_renderer_resize(gears_renderer, Event.resize.w, Event.resize.h);
			}
			else if(Event.type == SDL_QUIT)
			{
				return EXIT_SUCCESS;
			}
		}

		gears_renderer_draw(gears_renderer);
		SDL_GL_SwapBuffers();
	}
}
