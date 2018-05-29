/*
 * Copyright (c) 2018 Jeff Boody
 */

#ifndef gears_layerHud_H
#define gears_layerHud_H

#include "a3d/widget/a3d_layer.h"
#include "a3d/widget/a3d_text.h"
#include "a3d/math/a3d_vec4f.h"

struct gears_overlay_s;

typedef struct
{
	a3d_layer_t layer;

	// widgets
	a3d_text_t* text_fps;

	// cached state
	int fps;
} gears_layerHud_t;

gears_layerHud_t* gears_layerHud_new(struct gears_overlay_s* overlay);
void              gears_layerHud_delete(gears_layerHud_t** _self);
void              gears_layerHud_updateFps(gears_layerHud_t* self,
                                           int fps);

#endif
