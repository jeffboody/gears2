/*
 * Copyright (c) 2018 Jeff Boody
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
#include <math.h>
#include "gears_overlay.h"
#include "gears_renderer.h"

#define LOG_TAG "gears"
#include "a3d/a3d_log.h"

/***********************************************************
* private                                                  *
***********************************************************/

static int clickBack(a3d_widget_t* widget,
                     int state,
                     float x, float y)
{
	assert(widget);
	LOGD("debug x=%f, y=%f", x, y);

	gears_overlay_t* overlay = (gears_overlay_t*) widget->priv;
	if(state == A3D_WIDGET_POINTER_UP)
	{
		a3d_layer_bringFront(overlay->layer_show,
		                     (a3d_widget_t*) overlay->layer_hud);
	}
	return 1;
}

/***********************************************************
* public                                                   *
***********************************************************/

gears_overlay_t* gears_overlay_new(struct gears_renderer_s* renderer)
{
	assert(renderer);

	a3d_vec4f_t clear =
	{
		.r = 0.0f,
		.g = 0.0f,
		.b = 0.0f,
		.a = 0.0f
	};

	gears_overlay_t* self = (gears_overlay_t*)
	                        malloc(sizeof(gears_overlay_t));
	if(self == NULL)
	{
		return NULL;
	}
	self->renderer = renderer;

	#ifdef ANDROID
		const char resource[] = "/data/data/com.jeffboody.GearsES2eclair/files/resource.pak";
	#else
		const char resource[] = "resource.pak";
	#endif
	self->font_regular = a3d_font_new(resource,
	                                  "$BarlowSemiCondensed-Regular-64.texz",
	                                  "$BarlowSemiCondensed-Regular-64.xml");
	if(self->font_regular == NULL)
	{
		goto fail_font_regular;
	}

	self->font_bold = a3d_font_new(resource,
	                               "$BarlowSemiCondensed-Bold-64.texz",
	                               "$BarlowSemiCondensed-Bold-64.xml");
	if(self->font_bold == NULL)
	{
		goto fail_font_bold;
	}

	self->screen = a3d_screen_new(resource,
	                              self->font_regular,
	                              self->font_bold,
	                              (void*) renderer,
	                              gears_renderer_playClick);
	if(self->screen == NULL)
	{
		goto fail_screen;
	}

	self->layer_show = a3d_layer_new(self->screen,
	                                 0,
	                                 A3D_WIDGET_ANCHOR_TL,
	                                 A3D_WIDGET_WRAP_STRETCH_PARENT,
	                                 A3D_WIDGET_WRAP_STRETCH_PARENT,
	                                 A3D_WIDGET_STRETCH_ASPECT,
	                                 1.0f,
	                                 A3D_WIDGET_BORDER_NONE,
	                                 A3D_WIDGET_LINE_NONE,
	                                 &clear, &clear,
	                                 A3D_LAYER_MODE_FRONT);
	if(self->layer_show == NULL)
	{
		goto fail_layer_show;
	}

	self->view_about = gears_viewAbout_new(self, clickBack);
	if(self->view_about == NULL)
	{
		goto fail_view_about;
	}

	self->layer_hud = gears_layerHud_new(self);
	if(self->layer_hud == NULL)
	{
		goto fail_layer_hud;
	}

	a3d_screen_top(self->screen,
	               (a3d_widget_t*) self->layer_show);
	a3d_layer_bringFront(self->layer_show,
	                     (a3d_widget_t*) self->layer_hud);

	// success
	return self;

	// failure
	fail_view_about:
		gears_layerHud_delete(&self->layer_hud);
	fail_layer_hud:
		a3d_layer_delete(&self->layer_show);
	fail_layer_show:
		a3d_screen_delete(&self->screen);
	fail_screen:
		a3d_font_delete(&self->font_bold);
	fail_font_bold:
		a3d_font_delete(&self->font_regular);
	fail_font_regular:
		free(self);
	return NULL;
}

void gears_overlay_delete(gears_overlay_t** _self)
{
	assert(_self);

	gears_overlay_t* self = *_self;
	if(self)
	{
		// empty layer
		a3d_listitem_t* iter = a3d_list_head(self->layer_show->list);
		while(iter)
		{
			a3d_list_remove(self->layer_show->list, &iter);
		}

		gears_viewAbout_delete(&self->view_about);
		gears_layerHud_delete(&self->layer_hud);
		a3d_layer_delete(&self->layer_show);
		a3d_screen_delete(&self->screen);
		a3d_font_delete(&self->font_bold);
		a3d_font_delete(&self->font_regular);
		free(self);
		*_self = NULL;
	}
}

void gears_overlay_draw(gears_overlay_t* self,
                        int w, int h, float density)
{
	assert(self);

	a3d_screen_resize(self->screen, w, h);
	a3d_screen_density(self->screen, density);
	glDisable(GL_DEPTH_TEST);
	a3d_screen_draw(self->screen, 1.0f/60.0f);
	glEnable(GL_DEPTH_TEST);
}

void gears_overlay_updateFps(gears_overlay_t* self, int fps)
{
	assert(self);

	gears_layerHud_updateFps(self->layer_hud, fps);
}

int gears_overlay_pointerDown(gears_overlay_t* self,
                              float x, float y, double t0)
{
	assert(self);
	return a3d_screen_pointerDown(self->screen, x, y, t0);
}

int gears_overlay_pointerUp(gears_overlay_t* self,
                            float x, float y, double t0)
{
	assert(self);
	return a3d_screen_pointerUp(self->screen, x, y, t0);
}

int gears_overlay_pointerMove(gears_overlay_t* self,
                              float x, float y, double t0)
{
	assert(self);
	return a3d_screen_pointerMove(self->screen, x, y, t0);
}
