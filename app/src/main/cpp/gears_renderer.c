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

#include <stdlib.h>
#include <assert.h>
#include <math.h>
#include "gears_renderer.h"
#include "a3d/widget/a3d_key.h"
#include "a3d/a3d_timestamp.h"

#define LOG_TAG "gears"
#include "a3d/a3d_log.h"

/***********************************************************
* private                                                  *
***********************************************************/

// gear colors
static const a3d_vec4f_t RED   = { .r=0.8f, .g=0.1f, .b=0.0f, .a=1.0f };
static const a3d_vec4f_t GREEN = { .r=0.0f, .g=0.8f, .b=0.2f, .a=1.0f };
static const a3d_vec4f_t BLUE  = { .r=0.2f, .g=0.2f, .b=1.0f, .a=1.0f };

static void gears_renderer_step(gears_renderer_t* self)
{
	assert(self);
	LOGD("debug");

	double t     = a3d_timestamp();
	double dt0   = t - self->t0;
	++self->frames;

	// don't update fps every frame
	if(dt0 >= 1.0)
	{
		float seconds  = (float) dt0;
		self->last_fps = ((float) self->frames)/seconds;

		//LOGI("%i frames in %.2lf seconds = %.2lf FPS", self->frames, seconds, self->last_fps);
		gears_overlay_updateFps(self->overlay, (int) (self->last_fps + 0.5f));

		self->t0     = t;
		self->frames = 0;
	}

	// next frame
	if(self->t_last > 0.0)
	{
		float dt_last = (float) (t - self->t_last);

		if(self->last_fps > 30.0f)
		{
			dt_last = 1.0f/self->last_fps;
		}

		// make the gears rotate at a constant rate
		// red gear rotates at 1 revolution per 7 seconds
		self->angle += 360.0f * dt_last / 7.0f;
		if(self->angle >= 360.0f)
		{
			self->angle -= 360.0f;
		}
	}
	self->t_last = t;
}

static void gears_renderer_rotate(gears_renderer_t* self,
                                  float dx, float dy)
{
	assert(self);
	LOGD("debug dx=%f, dy=%f", dx, dy);

	// rotating around x-axis is equivalent to moving up-and-down on touchscreen
	// rotating around y-axis is equivalent to moving left-and-right on touchscreen
	// 360 degrees is equivalent to moving completly across the touchscreen
	float   w  = (float) self->w;
	float   h  = (float) self->h;
	GLfloat rx = 360.0f * dy / h;
	GLfloat ry = 360.0f * dx / w;
	a3d_quaternion_t q;
	a3d_quaternion_loadeuler(&q, rx, ry, 0.0f);
	a3d_quaternion_rotateq(&self->view_q, &q);
}

static void gears_renderer_scale(gears_renderer_t* self,
                                 float scale)
{
	assert(self);
	LOGD("debug ds=%f", ds);

	// scale range
	float min = 0.25f;
	float max = 2.0f;

	self->view_scale *= scale;
	if(self->view_scale < min)  self->view_scale = min;
	if(self->view_scale >= max) self->view_scale = max;
}

/***********************************************************
* public                                                   *
***********************************************************/

gears_renderer_t* gears_renderer_new(gears_renderer_cmd_fn cmd_fn)
{
	assert(cmd_fn);

	LOGD("debug");

	LOGI("GL vendor     : %s", glGetString(GL_VENDOR));
	LOGI("GL renderer   : %s", glGetString(GL_RENDERER));
	LOGI("GL version    : %s", glGetString(GL_VERSION));
	LOGI("GL extensions : %s", glGetString(GL_EXTENSIONS));

	gears_renderer_t* self = (gears_renderer_t*) malloc(sizeof(gears_renderer_t));
	if(self == NULL)
	{
		LOGE("malloc failed");
		return NULL;
	}

	// create the gears
	self->gear1 = gear_new(&RED, 1.0f, 4.0f, 1.0f, 20, 0.7f);
	if(self->gear1 == NULL) goto fail_gear1;

	self->gear2 = gear_new(&GREEN, 0.5f, 2.0f, 2.0f, 10, 0.7f);
	if(self->gear2 == NULL) goto fail_gear2;

	self->gear3 = gear_new(&BLUE, 1.3f, 2.0f, 0.5f, 10, 0.7f);
	if(self->gear3 == NULL) goto fail_gear3;

	self->mvm_stack = a3d_stack4f_new();
	if(self->mvm_stack == NULL)
		goto fail_stack;

	self->overlay = gears_overlay_new(self);
	if(self->overlay == NULL)
	{
		goto fail_overlay;
	}

	// initialize state
	a3d_quaternion_t qx;
	a3d_quaternion_t qy;
	a3d_quaternion_loadeuler(&qx, 20.0f,  0.0f, 0.0f);
	a3d_quaternion_loadeuler(&qy,  0.0f, 30.0f, 0.0f);
	a3d_quaternion_rotateq_copy(&qy, &qx, &self->view_q);

	self->last_fps = 0.0f;
	self->view_scale = 1.0f;
	self->angle      = 0.0f;
	self->t0         = a3d_timestamp();
	self->t_last     = 0.0;
	self->frames     = 0;
	self->w          = 0;
	self->h          = 0;
	self->density    = 1.0f;
	a3d_mat4f_identity(&self->pm);
	a3d_mat4f_identity(&self->mvm);

	self->touch_state = GEARS_TOUCH_STATE_INIT;
	self->touch_x1    = 0.0f;
	self->touch_y1    = 0.0f;
	self->touch_ds    = 1.0f;
	self->escape_t0   = a3d_timestamp();
	self->cmd_fn      = cmd_fn;

	glEnable(GL_CULL_FACE);
	glEnable(GL_DEPTH_TEST);
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

	// success
	return self;

	// failure
	fail_overlay:
		a3d_stack4f_delete(&self->mvm_stack);
	fail_stack:
		gear_delete(&self->gear3);
	fail_gear3:
		gear_delete(&self->gear2);
	fail_gear2:
		gear_delete(&self->gear1);
	fail_gear1:
		free(self);
	return NULL;
}

void gears_renderer_delete(gears_renderer_t** _self)
{
	// *_self can be null
	assert(_self);

	gears_renderer_t* self = *_self;
	if(self)
	{
		LOGD("debug");

		gears_overlay_delete(&self->overlay);
		a3d_stack4f_delete(&self->mvm_stack);
		gear_delete(&self->gear3);
		gear_delete(&self->gear2);
		gear_delete(&self->gear1);

		free(self);
		*_self = NULL;
	}
}

void gears_renderer_exit(gears_renderer_t* self)
{
	assert(self);

	gears_renderer_cmd_fn cmd_fn = self->cmd_fn;
	(*cmd_fn)(GEARS_CMD_EXIT, "");
}

void gears_renderer_loadURL(gears_renderer_t* self,
                            const char* url)
{
	assert(self);
	assert(url);

	gears_renderer_cmd_fn cmd_fn = self->cmd_fn;
	(*cmd_fn)(GEARS_CMD_LOADURL, url);
}

void gears_renderer_playClick(void* ptr)
{
	assert(ptr);

	gears_renderer_t* self = (gears_renderer_t*) ptr;
	gears_renderer_cmd_fn cmd_fn = self->cmd_fn;
	(*cmd_fn)(GEARS_CMD_PLAY_CLICK, "");
}

void gears_renderer_resize(gears_renderer_t* self, GLsizei w, GLsizei h)
{
	assert(self);

	if((w*h == 0) || ((self->w == w) && (self->h == h)))
	{
		// no resize
	}
	else
	{
		LOGI("%ix%i", w, h);
		self->w = w;
		self->h = h;
		glViewport(0, 0, w, h);
	}
}

void gears_renderer_density(gears_renderer_t* self,
                            float density)
{
	assert(self);

	self->density = density;
}

void gears_renderer_draw(gears_renderer_t* self)
{
	assert(self);
	LOGD("debug");

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// for fps text ...
	GLfloat w = (float) self->w;
	GLfloat h = (float) self->h;
	if(h > w)
	{
		GLfloat a = h / w;
		a3d_mat4f_frustum(&self->pm, 1, -1.0f, 1.0f, -a, a, 5.0f, 60.0f);
	}
	else
	{
		GLfloat a = w / h;
		a3d_mat4f_frustum(&self->pm, 1, -a, a, -1.0f, 1.0f, 5.0f, 60.0f);
	}
	a3d_mat4f_translate(&self->mvm, 1, 0.0f, 0.0f, -40.0f);

	// glxgears: event_loop
	a3d_stack4f_push(self->mvm_stack, &self->mvm);
	a3d_mat4f_scale(&self->mvm, 0, self->view_scale, self->view_scale, self->view_scale);
	a3d_mat4f_rotateq(&self->mvm, 0, &self->view_q);

	// Gear1
	a3d_mat4f_t mvp;
	a3d_stack4f_push(self->mvm_stack, &self->mvm);
	a3d_mat4f_translate(&self->mvm, 0, -3.0f, -2.0f, 0.0f);
	a3d_mat4f_rotate(&self->mvm, 0, self->angle, 0.0f, 0.0f, 1.0f);
	a3d_mat4f_mulm_copy(&self->pm, &self->mvm, &mvp);
	gear_draw(self->gear1, &mvp, &self->mvm);
	a3d_stack4f_pop(self->mvm_stack, &self->mvm);

	// Gear2
	a3d_stack4f_push(self->mvm_stack, &self->mvm);
	a3d_mat4f_translate(&self->mvm, 0, 3.1f, -2.0f, 0.0f);
	a3d_mat4f_rotate(&self->mvm, 0, -2.0f * self->angle - 9.0f, 0.0f, 0.0f, 1.0f);
	a3d_mat4f_mulm_copy(&self->pm, &self->mvm, &mvp);
	gear_draw(self->gear2, &mvp, &self->mvm);
	a3d_stack4f_pop(self->mvm_stack, &self->mvm);

	// Gear3
	a3d_stack4f_push(self->mvm_stack, &self->mvm);
	a3d_mat4f_translate(&self->mvm, 0, -3.1f, 4.2f, 0.0f);
	a3d_mat4f_rotate(&self->mvm, 0, -2.0f * self->angle - 25.0f, 0.0f, 0.0f, 1.0f);
	a3d_mat4f_mulm_copy(&self->pm, &self->mvm, &mvp);
	gear_draw(self->gear3, &mvp, &self->mvm);
	a3d_stack4f_pop(self->mvm_stack, &self->mvm);

	a3d_stack4f_pop(self->mvm_stack, &self->mvm);

	gears_overlay_draw(self->overlay, self->w, self->h, self->density);
	gears_renderer_step(self);

	A3D_GL_GETERROR();
}

void gears_renderer_touch(gears_renderer_t* self,
                          int action, int count, double ts,
                          float x0, float y0,
                          float x1, float y1,
                          float x2, float y2,
                          float x3, float y3)
{
	assert(self);

	if(action == GEARS_TOUCH_ACTION_UP)
	{
		if(self->touch_state == GEARS_TOUCH_STATE_OVERLAY)
		{
			gears_overlay_pointerUp(self->overlay, x0, y0, ts);
		}

		// Do nothing
		self->touch_state = GEARS_TOUCH_STATE_INIT;
	}
	else if(count == 1)
	{
		if((self->touch_state == GEARS_TOUCH_STATE_INIT) &&
		   (action == GEARS_TOUCH_ACTION_DOWN) &&
		   gears_overlay_pointerDown(self->overlay, x0, y0, ts))
		{
			self->touch_state = GEARS_TOUCH_STATE_OVERLAY;
		}
		else if(self->touch_state == GEARS_TOUCH_STATE_OVERLAY)
		{
			gears_overlay_pointerMove(self->overlay, x0, y0, ts);
		}
		else if(self->touch_state == GEARS_TOUCH_STATE_ROTATE)
		{
			float dx = x0 - self->touch_x1;
			float dy = y0 - self->touch_y1;
			gears_renderer_rotate(self, dx, dy);
			self->touch_x1 = x0;
			self->touch_y1 = y0;
		}
		else if(action == GEARS_TOUCH_ACTION_DOWN)
		{
			self->touch_x1    = x0;
			self->touch_y1    = y0;
			self->touch_state = GEARS_TOUCH_STATE_ROTATE;
		}
	}
	else if(count == 2)
	{
		if(self->touch_state == GEARS_TOUCH_STATE_OVERLAY)
		{
			// ignore
		}
		else if(self->touch_state == GEARS_TOUCH_STATE_ZOOM)
		{
			float dx = fabsf(x1 - x0);
			float dy = fabsf(y1 - y0);
			float ds = sqrtf(dx*dx + dy*dy);

			gears_renderer_scale(self, ds/self->touch_ds);

			self->touch_ds = ds;
		}
		else
		{
			float dx = fabsf(x1 - x0);
			float dy = fabsf(y1 - y0);
			float ds = sqrtf(dx*dx + dy*dy);

			self->touch_ds    = ds;
			self->touch_state = GEARS_TOUCH_STATE_ZOOM;
		}
	}
}

void gears_renderer_keyPress(gears_renderer_t* self,
                             int keycode, int meta)
{
	assert(self);

	if(keycode == A3D_KEY_ESCAPE)
	{
		if(gears_overlay_escape(self->overlay))
		{
			// ignore
		}
		else
		{
			// double tap back to exit
			double t1 = a3d_timestamp();
			if((t1 - self->escape_t0) < 0.5)
			{
				gears_renderer_exit(self);
			}
			else
			{
				self->escape_t0 = t1;
			}
		}
	}
}
