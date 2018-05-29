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

#ifndef gears_overlay_H
#define gears_overlay_H

#include "a3d/widget/a3d_font.h"
#include "a3d/widget/a3d_screen.h"
#include "a3d/widget/a3d_layer.h"
#include "a3d/widget/a3d_text.h"

typedef struct
{
	a3d_font_t*    font_regular;
	a3d_font_t*    font_bold;
	a3d_screen_t*  screen;
	a3d_layer_t*   layer_show;
	a3d_text_t*    text_fps;
} gears_overlay_t;

gears_overlay_t* gears_overlay_new(void);
void             gears_overlay_delete(gears_overlay_t** _self);
void             gears_overlay_draw(gears_overlay_t* self,
                                    int w, int h, float density);
void             gears_overlay_updateFps(gears_overlay_t* self,
                                         float fps);

#endif
