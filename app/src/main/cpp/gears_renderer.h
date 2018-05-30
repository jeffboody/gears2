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

#define GEARS_TOUCH_ACTION_DOWN 0
#define GEARS_TOUCH_ACTION_UP   1
#define GEARS_TOUCH_ACTION_MOVE 2

#define GEARS_TOUCH_STATE_INIT    0
#define GEARS_TOUCH_STATE_ROTATE  1
#define GEARS_TOUCH_STATE_ZOOM    2
#define GEARS_TOUCH_STATE_OVERLAY 3

#define GEARS_CMD_PLAY_CLICK 1
#define GEARS_CMD_EXIT       2
#define GEARS_CMD_LOADURL    3

typedef void (*gears_renderer_cmd_fn)(int cmd, const char* msg);

/***********************************************************
* public                                                   *
***********************************************************/

typedef struct gears_renderer_s
{
	gear_t* gear1;
	gear_t* gear2;
	gear_t* gear3;

	GLsizei w;
	GLsizei h;
	float   density;

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

	// touch state
	int   touch_state;
	float touch_x1;
	float touch_y1;
	float touch_ds;

	// escape state
	double escape_t0;

	// overlay
	gears_overlay_t* overlay;

	// JNI callback(s)
	gears_renderer_cmd_fn cmd_fn;
} gears_renderer_t;

gears_renderer_t* gears_renderer_new(gears_renderer_cmd_fn cmd_fn);
void              gears_renderer_delete(gears_renderer_t** _self);
void              gears_renderer_exit(gears_renderer_t* self);
void              gears_renderer_loadURL(gears_renderer_t* self,
                                         const char* url);
void              gears_renderer_playClick(void* ptr);
void              gears_renderer_resize(gears_renderer_t* self, int w, int h);
void              gears_renderer_density(gears_renderer_t* self, float density);
void              gears_renderer_draw(gears_renderer_t* self);
void              gears_renderer_touch(gears_renderer_t* self,
                                       int action, int count, double ts,
                                       float x0, float y0,
                                       float x1, float y1,
                                       float x2, float y2,
                                       float x3, float y3);
void              gears_renderer_keyPress(gears_renderer_t* self,
                                          int keycode, int meta);

#endif
