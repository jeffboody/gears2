/*
 * Copyright (c) 2009-2010 Jeff Boody
 *
 * Gears for Android is a heavily modified port of the famous "gears" demo.
 * As such, it is a derived work subject to the license requirements (below)
 * of the original work.
 *
 * Copyright (c) 1999-2001  Brian Paul   All Rights Reserved.
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

#ifndef gears_renderer_H
#define gears_renderer_H

#include <pthread.h>
#include "a3d/math/a3d_quaternion.h"
#include "a3d/math/a3d_mat4f.h"
#include "a3d/math/a3d_stack4f.h"
#include "gears_overlay.h"
#include "gear.h"

/***********************************************************
* public                                                   *
***********************************************************/

typedef struct
{
	gear_t* gear1;
	gear_t* gear2;
	gear_t* gear3;

	GLsizei w;
	GLsizei h;

	// mutex protects view state
	pthread_mutex_t  mutex;
	GLfloat          view_scale;
	a3d_quaternion_t view_q;

	// animation state
	GLfloat angle;

	// matrices
	a3d_mat4f_t    pm;
	a3d_mat4f_t    mvm;
	a3d_stack4f_t* mvm_stack;

	// fps state
	double t0;
	double t_last;
	double t_start;
	int    frames;
	float  last_fps;

	// overlay
	gears_overlay_t* overlay;
} gears_renderer_t;

gears_renderer_t* gears_renderer_new(void);
void              gears_renderer_delete(gears_renderer_t** _self);
void              gears_renderer_resize(gears_renderer_t* self, int w, int h);
void              gears_renderer_rotate(gears_renderer_t* self, float dx, float dy);
void              gears_renderer_scale(gears_renderer_t* self, float ds);
void              gears_renderer_roll(gears_renderer_t* self, float roll);
void              gears_renderer_draw(gears_renderer_t* self);

#endif
