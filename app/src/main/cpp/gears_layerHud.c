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

static int clickAbout(a3d_widget_t* widget,
                      int state,
                      float x, float y)
{
	assert(widget);

	gears_overlay_t* overlay = (gears_overlay_t*) widget->priv;
	if(state == A3D_WIDGET_POINTER_UP)
	{
		overlay->draw_mode = GEARS_OVERLAY_DRAWMODE_ABOUT;
		a3d_viewbox_scrollTop((a3d_viewbox_t*) overlay->view_about);
		a3d_layer_bringFront(overlay->layer_show,
		                     (a3d_widget_t*) overlay->view_about);
	}
	return 1;
}

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

	a3d_vec4f_t white =
	{
		.r = 1.0f,
		.g = 1.0f,
		.b = 1.0f,
		.a = 1.0f
	};

	a3d_vec4f_t yellow =
	{
		.r = 1.0f,
		.g = 1.0f,
		.b = 0.235f,
		.a = 1.0f
	};

	a3d_widgetLayout_t layout_hud =
	{
		.wrapx    = A3D_WIDGET_WRAP_STRETCH_PARENT,
		.wrapy    = A3D_WIDGET_WRAP_STRETCH_PARENT,
		.aspectx  = A3D_WIDGET_ASPECT_DEFAULT,
		.aspecty  = A3D_WIDGET_ASPECT_DEFAULT,
		.stretchx = 1.0f,
		.stretchy = 1.0f
	};

	gears_layerHud_t* self;
	self = (gears_layerHud_t*)
	       a3d_layer_new(overlay->screen,
	                     sizeof(gears_layerHud_t),
	                     &layout_hud,
	                     A3D_WIDGET_BORDER_LARGE,
	                     &clear,
	                     A3D_LAYER_MODE_LAYERED);
	if(self == NULL)
	{
		return NULL;
	}
	a3d_widget_priv((a3d_widget_t*) self, (void*) overlay);

	self->bulletbox_about = a3d_bulletbox_new(overlay->screen,
	                                          0,
	                                          A3D_WIDGET_BORDER_MEDIUM,
	                                          A3D_TEXT_SIZE_MEDIUM,
	                                          &white, 1,
	                                          clickAbout,
	                                          NULL);
	if(self->bulletbox_about == NULL)
	{
		goto fail_bulletbox_about;
	}
	a3d_widget_priv((a3d_widget_t*) self->bulletbox_about,
	                (void*) overlay);
	a3d_bulletbox_textPrintf(self->bulletbox_about,
	                         "%s", "Gears");

	self->text_fps = a3d_text_new(overlay->screen,
	                              0,
	                              A3D_WIDGET_BORDER_NONE,
	                              A3D_TEXT_SIZE_SMALL,
	                              &clear, &yellow,
	                              NULL, NULL,
	                              NULL, NULL);
	if(self->text_fps == NULL)
	{
		goto fail_text_fps;
	}
	a3d_widget_anchor((a3d_widget_t*) self->text_fps,
	                  A3D_WIDGET_ANCHOR_BR);
	a3d_text_printf(self->text_fps, "%s", "0 fps");
	self->fps = 0;

	if(a3d_bulletbox_spriteLoad(self->bulletbox_about, 0,
	                            "$ic_info_outline_white_24dp.texz") == 0)
	{
		goto fail_icon;
	}

	a3d_layer_t* layer = (a3d_layer_t*) self;
	a3d_list_push(layer->list, self->bulletbox_about);
	a3d_list_push(layer->list, self->text_fps);

	// success
	return self;

	// failure
	fail_icon:
		a3d_text_delete(&self->text_fps);
	fail_text_fps:
		a3d_bulletbox_delete(&self->bulletbox_about);
	fail_bulletbox_about:
		a3d_layer_delete((a3d_layer_t**) &self);
	return NULL;
}

void gears_layerHud_delete(gears_layerHud_t** _self)
{
	assert(_self);

	gears_layerHud_t* self = *_self;
	if(self)
	{
		a3d_layer_clear((a3d_layer_t*) self);
		a3d_text_delete(&self->text_fps);
		a3d_bulletbox_delete(&self->bulletbox_about);
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
