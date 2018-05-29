/*
 * Copyright (c) 2018 Jeff Boody
 */

#include <stdlib.h>
#include <assert.h>
#include "gears_layerHud.h"
#include "gears_overlay.h"
#include "a3d/a3d_unit.h"

#define LOG_TAG "gears"
#include "a3d/a3d_log.h"

/***********************************************************
* private                                                  *
***********************************************************/

/***********************************************************
* public                                                   *
***********************************************************/

gears_layerHud_t* gears_layerHud_new(struct gears_overlay_s* overlay)
{
	assert(overlay);

	a3d_vec4f_t clear =
	{
		.r = 0.0f,
		.g = 0.0f,
		.b = 0.0f,
		.a = 0.0f
	};

	a3d_vec4f_t yellow =
	{
		.r = 1.0f,
		.g = 1.0f,
		.b = 0.235f,
		.a = 1.0f
	};

	gears_layerHud_t* self = (gears_layerHud_t*)
	                         a3d_layer_new(overlay->screen,
	                                       sizeof(gears_layerHud_t),
	                                       A3D_WIDGET_ANCHOR_TL,
	                                       A3D_WIDGET_WRAP_STRETCH_PARENT,
	                                       A3D_WIDGET_WRAP_STRETCH_PARENT,
	                                       A3D_WIDGET_STRETCH_ASPECT,
	                                       1.0f,
	                                       A3D_WIDGET_BORDER_LARGE,
	                                       A3D_WIDGET_LINE_NONE,
	                                       &clear, &clear,
	                                       A3D_LAYER_MODE_LAYERED);
	if(self == NULL)
	{
		return NULL;
	}
	a3d_widget_priv((a3d_widget_t*) self, (void*) overlay);

	self->text_fps = a3d_text_new(overlay->screen,
	                              0,
	                              A3D_WIDGET_ANCHOR_BR,
	                              A3D_WIDGET_BORDER_NONE,
	                              A3D_WIDGET_LINE_NONE,
	                              A3D_TEXT_STYLE_SMALL,
	                              &clear, &clear, &yellow,
	                              16,
	                              NULL,
	                              NULL);
	if(self->text_fps == NULL)
	{
		goto fail_text_fps;
	}
	a3d_text_printf(self->text_fps, "%s", "0 fps");
	self->fps = 0;

	a3d_layer_t* layer = (a3d_layer_t*) self;
	a3d_list_push(layer->list, self->text_fps);

	// success
	return self;

	// failure
	fail_text_fps:
		a3d_layer_delete((a3d_layer_t**) &self);
	return NULL;
}

void gears_layerHud_delete(gears_layerHud_t** _self)
{
	assert(_self);

	gears_layerHud_t* self = *_self;
	if(self)
	{
		LOGD("debug");

		// empty layer
		a3d_layer_t*    layer = (a3d_layer_t*) self;
		a3d_listitem_t* iter  = a3d_list_head(layer->list);
		while(iter)
		{
			a3d_list_remove(layer->list, &iter);
		}

		a3d_text_delete(&self->text_fps);
		a3d_layer_delete((a3d_layer_t**) _self);
	}
}

void gears_layerHud_updateFps(gears_layerHud_t* self, int fps)
{
	assert(self);

	if(self->fps == fps)
	{
		return;
	}
	self->fps = fps;

	a3d_text_printf(self->text_fps, "%i fps", fps);
}
