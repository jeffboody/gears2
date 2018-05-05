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

#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <assert.h>
#include <pthread.h>
#include <signal.h>

#include "bcm_host.h"
#include "GLES2/gl2.h"
#include "EGL/egl.h"
#include "EGL/eglext.h"

#include "gears_renderer.h"

#define LOG_TAG "gears"
#include "a3d/a3d_log.h"

/***********************************************************
* private                                                  *
***********************************************************/

static gears_renderer_t* gears_renderer = NULL;

typedef struct
{
	uint32_t   screen_width;
	uint32_t   screen_height;
	EGLDisplay display;
	EGLSurface surface;
	EGLContext context;

	int quit;
	pthread_mutex_t mutex;
} raspi_state_t;

static raspi_state_t* raspi = NULL;

static void raspi_state_quit(int signo)
{
	assert(raspi);

	if(signo != SIGINT)
	{
		return;
	}

	pthread_mutex_lock(&raspi->mutex);
	raspi->quit = 1;
	pthread_mutex_unlock(&raspi->mutex);
}

static int raspi_state_isQuit(void)
{
	LOGD("debug");

	pthread_mutex_lock(&raspi->mutex);
	int quit = raspi->quit;
	pthread_mutex_unlock(&raspi->mutex);
	return quit;
}

static int raspi_state_new()
{
	raspi = (raspi_state_t*) malloc(sizeof(raspi_state_t));
	if(raspi == NULL)
	{
		LOGE("malloc failed");
		return 0;
	}

	bcm_host_init();

	raspi->display = eglGetDisplay(EGL_DEFAULT_DISPLAY);
	if(raspi->display == EGL_NO_DISPLAY)
	{
		LOGE("eglGetDisplay failed");
		goto fail_display;
	}

	if(eglInitialize(raspi->display, NULL, NULL) == EGL_FALSE)
	{
		LOGE("eglInitialize failed");
		goto fail_initialize;
	}

	const EGLint attribute_list[] =
	{
		EGL_RED_SIZE, 8,
		EGL_GREEN_SIZE, 8,
		EGL_BLUE_SIZE, 8,
		EGL_ALPHA_SIZE, 8,
		EGL_DEPTH_SIZE, 16,
		EGL_SURFACE_TYPE, EGL_WINDOW_BIT,
		EGL_NONE
	};
	EGLConfig config;
	EGLint    num_config;
	if(eglChooseConfig(raspi->display,
	                   attribute_list,
	                   &config, 1,
	                   &num_config) == EGL_FALSE)
	{
		LOGE("eglChooseConfig failed");
		goto fail_config;
	}

	const EGLint context_attributes[] =
	{
		EGL_CONTEXT_CLIENT_VERSION, 2,
		EGL_NONE
	};
	raspi->context = eglCreateContext(raspi->display,
	                                  config,
	                                  EGL_NO_CONTEXT,
	                                  context_attributes);
	if(raspi->context == EGL_NO_CONTEXT)
	{
		LOGE("eglCreateContext failed");
		goto fail_context;
	}

	if(graphics_get_display_size(0,
	                             &raspi->screen_width,
	                             &raspi->screen_height) < 0)
	{
		LOGE("graphics_get_display_size failed");
		goto fail_size;
	}

	VC_RECT_T dst_rect;
	dst_rect.x = 0;
	dst_rect.y = 0;
	dst_rect.width  = raspi->screen_width;
	dst_rect.height = raspi->screen_height;

	VC_RECT_T src_rect;
	src_rect.x = 0;
	src_rect.y = 0;
	src_rect.width  = raspi->screen_width << 16;
	src_rect.height = raspi->screen_height << 16;

	DISPMANX_ELEMENT_HANDLE_T dispman_element;
	DISPMANX_DISPLAY_HANDLE_T dispman_display;
	DISPMANX_UPDATE_HANDLE_T  dispman_update;
	dispman_display = vc_dispmanx_display_open(0);
	dispman_update = vc_dispmanx_update_start(0);
	dispman_element = vc_dispmanx_element_add(dispman_update,
	                                          dispman_display,
	                                          0, &dst_rect,
	                                          0, &src_rect,
	                                          DISPMANX_PROTECTION_NONE,
	                                          0, 0, 0);

	EGL_DISPMANX_WINDOW_T nativewindow;
	nativewindow.element = dispman_element;
	nativewindow.width   = raspi->screen_width;
	nativewindow.height  = raspi->screen_height;
	vc_dispmanx_update_submit_sync(dispman_update);

	raspi->surface = eglCreateWindowSurface(raspi->display,
	                                        config,
	                                        &nativewindow,
	                                        NULL);
	if(raspi->surface == EGL_NO_SURFACE)
	{
		LOGE("eglCreateWindowSurface failed");
		goto fail_surface;
	}

	if(eglMakeCurrent(raspi->display,
	                  raspi->surface,
	                  raspi->surface,
	                  raspi->context) == EGL_FALSE)
	{
		LOGE("eglMakeCurrent failed");
		goto fail_current;
	}

	// PTHREAD_MUTEX_DEFAULT is not re-entrant
	if(pthread_mutex_init(&raspi->mutex, NULL) != 0)
	{
		LOGE("pthread_mutex_init failed");
		goto fail_mutex;
	}
	raspi->quit = 0;

	// install signal handler
	if(signal(SIGINT, raspi_state_quit) == SIG_ERR)
	{
		goto fail_signal;
	}

	gears_renderer = gears_renderer_new("whitrabt.texgz");
	if(gears_renderer == NULL)
	{
		goto fail_gears;
	}

	// success
	return 1;

	// failure
	fail_gears:
		signal(SIGINT, SIG_DFL);
	fail_signal:
		pthread_mutex_destroy(&raspi->mutex);
	fail_mutex:
		eglMakeCurrent(raspi->display, EGL_NO_SURFACE,
		               EGL_NO_SURFACE, EGL_NO_CONTEXT);
	fail_current:
		eglDestroySurface(raspi->display, raspi->surface);
	fail_surface:
		eglDestroyContext(raspi->display, raspi->context);
	fail_size:
	fail_context:
	fail_config:
	fail_initialize:
	fail_display:
		bcm_host_deinit();
		free(raspi);
		raspi = NULL;
	return 0;
}

static void raspi_state_delete(void)
{
	gears_renderer_delete(&gears_renderer);

	signal(SIGINT, SIG_DFL);
	pthread_mutex_destroy(&raspi->mutex);

	eglMakeCurrent(raspi->display, EGL_NO_SURFACE,
	               EGL_NO_SURFACE, EGL_NO_CONTEXT);
	eglDestroySurface(raspi->display, raspi->surface);
	eglDestroyContext(raspi->display, raspi->context);
	eglTerminate(raspi->display);

	free(raspi);
	raspi = NULL;

	bcm_host_deinit();
}

int main( int argc, char ** argv )
{
	if(raspi_state_new() == 0)
	{
		return EXIT_FAILURE;
	}

	while(raspi_state_isQuit() == 0)
	{
		gears_renderer_resize(gears_renderer,
		                      raspi->screen_width,
		                      raspi->screen_height);
		gears_renderer_draw(gears_renderer);
		eglSwapBuffers(raspi->display, raspi->surface);
	}
	raspi_state_delete();

	return EXIT_SUCCESS;
}
