/*
 * Copyright (c) 2013 Jeff Boody
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
#include <assert.h>
#include <math.h>
#include "loax/loax_client.h"
#include "loax/loax_gl2.h"
#include "loax/gl2.h"
#include "gears_renderer.h"

#define LOG_TAG "gears"
#include "loax/loax_log.h"

/***********************************************************
* private                                                  *
***********************************************************/

static gears_renderer_t* gears_renderer = NULL;

// touch state
#define TOUCH_STATE_INIT   0
#define TOUCH_STATE_ROTATE 1
#define TOUCH_STATE_ZOOM   2

static int   g_touch_state = TOUCH_STATE_INIT;
static float g_touch_x1    = 0.0f;
static float g_touch_y1    = 0.0f;
static float g_touch_x2    = 0.0f;
static float g_touch_y2    = 0.0f;
static float g_touch_s     = 0.0f;

// axis state
static float g_axis_x1 = 0.0f;
static float g_axis_y1 = 0.0f;
static float g_axis_x2 = 0.0f;
static float g_axis_y2 = 0.0f;

static void touch_event(gears_renderer_t* self, loax_event_t* e)
{
	assert(self);
	assert(e);
	LOGD("debug");

	int type  = e->type;
	int count = e->event_touch.count;
	if(type == LOAX_EVENT_TOUCHUP)
	{
		// do nothing
		g_touch_state = TOUCH_STATE_INIT;
	}
	else if(count == 1)
	{
		if(g_touch_state == TOUCH_STATE_ROTATE)
		{
			float dx = e->event_touch.coord[0].x - g_touch_x1;
			float dy = e->event_touch.coord[0].y - g_touch_y1;
			gears_renderer_rotate(self, dx, dy);
			g_touch_x1 = e->event_touch.coord[0].x;
			g_touch_y1 = e->event_touch.coord[0].y;
		}
		else
		{
			g_touch_x1    = e->event_touch.coord[0].x;
			g_touch_y1    = e->event_touch.coord[0].y;
			g_touch_state = TOUCH_STATE_ROTATE;
		}
	}
	else if(count == 2)
	{
		if(g_touch_state == TOUCH_STATE_ZOOM)
		{
			// some unknown device throws an exception here
			float x1 = e->event_touch.coord[0].x;
			float y1 = e->event_touch.coord[0].y;
			float x2 = e->event_touch.coord[1].x;
			float y2 = e->event_touch.coord[1].y;

			float dx = fabsf(x2 - x1);
			float dy = fabsf(y2 - y1);
			float s  = sqrtf((dx * dx) + (dy * dy));
			gears_renderer_scale(gears_renderer, g_touch_s - s);

			// a.b = mag_a*mag_b*cos(roll)
			// direction or roll is determined by axb
			float ax = x2 - x1;
			float ay = y2 - y1;
			float bx = g_touch_x2 - g_touch_x1;
			float by = g_touch_y2 - g_touch_y1;
			float mag_a = sqrtf(ax*ax + ay*ay);
			float mag_b = sqrtf(bx*bx + by*by);
			if((mag_a >= 5.0f) && (mag_b >= 5.0f))
			{
				float axb  = (ax*by - ay*bx);
				float ab   = (ax*bx + ay*by);
				float roll = (180.0f/M_PI)*acosf(ab/(mag_a*mag_b));
				if(fabs(roll) >= 0.5f)
				{
					gears_renderer_roll(gears_renderer, (axb > 0.0f) ? roll : -roll);
				}
			}

			g_touch_x1 = x1;
			g_touch_y1 = y1;
			g_touch_x2 = x2;
			g_touch_y2 = y2;
			g_touch_s  = s;
		}
		else
		{
			g_touch_x1 = e->event_touch.coord[0].x;
			g_touch_y1 = e->event_touch.coord[0].y;
			g_touch_x2 = e->event_touch.coord[1].x;
			g_touch_y2 = e->event_touch.coord[1].y;

			float dx      = fabsf(g_touch_x2 - g_touch_x1);
			float dy      = fabsf(g_touch_y2 - g_touch_y1);
			g_touch_s     = sqrtf((dx * dx) + (dy * dy));
			g_touch_state = TOUCH_STATE_ZOOM;
		}
	}
}

static void axis_update(gears_renderer_t* self, int w, int h)
{
	assert(self);
	LOGD("debug w=%i, h=%i", w, h);

	if((fabs(g_axis_x1) > 0.1f) || (fabs(g_axis_y1) > 0.1f))
	{
		float dx = 0.01f*g_axis_x1*w;
		float dy = 0.01f*g_axis_y1*h;
		gears_renderer_rotate(self, dx, dy);
	}

	if(fabs(g_axis_y2) > 0.1f)
	{
		float s = 0.05f*g_axis_y2*h;
		gears_renderer_scale(gears_renderer, s);
	}
}

/***********************************************************
* main                                                     *
***********************************************************/

int main(int argc, char** argv)
{
	while(1)
	{
		loax_client_t* c = loax_client_new();
		if(c == NULL)
		{
			return EXIT_FAILURE;
		}

		// Initialize gears renderer
		gears_renderer = gears_renderer_new("whitrabt.texgz");
		if(gears_renderer == NULL)
		{
			loax_client_delete(&c);
			return EXIT_FAILURE;
		}

		do
		{
			loax_event_t e;
			while(loax_client_poll(c, &e))
			{
				if((e.type == LOAX_EVENT_TOUCHDOWN) ||
				   (e.type == LOAX_EVENT_TOUCHUP)   ||
				   (e.type == LOAX_EVENT_TOUCHMOVE))
				{
					touch_event(gears_renderer, &e);
				}
				else if(e.type == LOAX_EVENT_AXISMOVE)
				{
					int   axis  = e.event_axis.axis;
					float value = e.event_axis.value;
					if(axis == LOAX_AXIS_X1)
					{
						g_axis_x1 = value;
					}
					else if(axis == LOAX_AXIS_Y1)
					{
						g_axis_y1 = value;
					}
					else if(axis == LOAX_AXIS_X2)
					{
						g_axis_x2 = value;
					}
					else if(axis == LOAX_AXIS_Y2)
					{
						g_axis_y2 = value;
					}
				}
			}

			int w = 0;
			int h = 0;
			loax_client_size(c, &w, &h);
			axis_update(gears_renderer, w, h);
			gears_renderer_resize(gears_renderer, w, h);
			gears_renderer_draw(gears_renderer);
		} while(loax_client_swapbuffers(c));

		gears_renderer_delete(&gears_renderer);
		loax_client_delete(&c);
	}
}
