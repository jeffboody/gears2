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

#include <jni.h>
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <pthread.h>
#include "gears_renderer.h"

#define LOG_TAG "gears"
#include "a3d/a3d_log.h"

/***********************************************************
* private - eventq                                         *
***********************************************************/

// type
#define GEARS_EVENT_KEYDOWN   0
#define GEARS_EVENT_KEYUP     1
#define GEARS_EVENT_RESIZE    2
#define GEARS_EVENT_TOUCHDOWN 3
#define GEARS_EVENT_TOUCHUP   4
#define GEARS_EVENT_TOUCHMOVE 5
#define GEARS_EVENT_DENSITY   6

// max touch events supported
#define GEARS_EVENT_TOUCHMAX 4

typedef struct
{
	int keycode;
	int meta;
} gears_eventkey_t;

typedef struct
{
	int w;
	int h;
} gears_eventresize_t;

typedef struct
{
	float density;
} gears_eventdensity_t;

typedef struct
{
	float x;
	float y;
} gears_eventcoord_t;

typedef struct
{
	int count;
	gears_eventcoord_t coord[GEARS_EVENT_TOUCHMAX];
} gears_eventtouch_t;

typedef struct
{
	int    type;
	double ts;
	union
	{
		gears_eventkey_t     event_key;
		gears_eventresize_t  event_resize;
		gears_eventdensity_t event_density;
		gears_eventtouch_t   event_touch;
	};
} gears_event_t;

#define GEARS_EVENTQ_BUFSIZE 256

typedef struct
{
	// event state
	pthread_mutex_t event_mutex;
	pthread_cond_t  event_cond;
	int             event_head;
	int             event_tail;
	gears_event_t   event_buffer[GEARS_EVENTQ_BUFSIZE];
} gears_eventq_t;

gears_eventq_t* gears_eventq_new(void)
{
	LOGD("debug");

	gears_eventq_t* self = (gears_eventq_t*)
	                       malloc(sizeof(gears_eventq_t));
	if(self == NULL)
	{
		LOGE("malloc failed");
		return NULL;
	}

	// PTHREAD_MUTEX_DEFAULT is not re-entrant
	if(pthread_mutex_init(&self->event_mutex, NULL) != 0)
	{
		LOGE("pthread_mutex_init failed");
		goto fail_mutex_init;
	}

	if(pthread_cond_init(&self->event_cond, NULL) != 0)
	{
		LOGE("pthread_cond_init failed");
		goto fail_cond_init;
	}

	self->event_head = 0;
	self->event_tail = 0;

	// success
	return self;

	// failure
	fail_cond_init:
		pthread_mutex_destroy(&self->event_mutex);
	fail_mutex_init:
		free(self);
	return NULL;
}

void gears_eventq_delete(gears_eventq_t** _self)
{
	assert(_self);

	gears_eventq_t* self = *_self;
	if(self)
	{
		LOGD("debug");

		pthread_cond_destroy(&self->event_cond);
		pthread_mutex_destroy(&self->event_mutex);
		free(self);
		*_self = NULL;
	}
}

int gears_eventq_poll(gears_eventq_t* self, gears_event_t* e)
{
	assert(self);
	assert(e);
	LOGD("debug");

	pthread_mutex_lock(&self->event_mutex);

	int has_event = 0;
	if(self->event_head == self->event_tail)
	{
		// buffer is empty
	}
	else
	{
		*e = self->event_buffer[self->event_head];
		self->event_head = (self->event_head + 1) % GEARS_EVENTQ_BUFSIZE;
		has_event = 1;
		pthread_cond_signal(&self->event_cond);
	}

	pthread_mutex_unlock(&self->event_mutex);

	return has_event;
}

gears_event_t* gears_eventq_dequeue(gears_eventq_t* self)
{
	assert(self);
	LOGD("debug");

	return &self->event_buffer[self->event_tail];
}

void gears_eventq_enqueue(gears_eventq_t* self)
{
	assert(self);
	LOGD("debug");

	pthread_mutex_lock(&self->event_mutex);

	self->event_tail = (self->event_tail + 1) % GEARS_EVENTQ_BUFSIZE;
	if(self->event_tail == self->event_head)
	{
		// wait if the buffer is full
		pthread_cond_wait(&self->event_cond, &self->event_mutex);
	}

	pthread_mutex_unlock(&self->event_mutex);
}

/***********************************************************
* private - cmd_fn callback                                *
***********************************************************/

static JavaVM* g_vm = NULL;

static void cmd_fn(int cmd, const char* msg)
{
	LOGD("debug cmd=%i, msg=%s", cmd, msg);

	if(g_vm == NULL)
	{
		LOGE("g_vm is NULL");
		return;
	}

	JNIEnv* env = NULL;
	if((*g_vm)->AttachCurrentThread(g_vm, &env, NULL) != 0)
	{
		LOGE("AttachCurrentThread failed");
		return;
	}

	jclass cls = (*env)->FindClass(env, "com/jeffboody/GearsES2eclair/GearsES2eclair");
	if(cls == NULL)
	{
		LOGE("FindClass failed");
		return;
	}

	jmethodID mid = (*env)->GetStaticMethodID(env, cls,
	                                          "CallbackCmd",
	                                          "(ILjava/lang/String;)V");
	if(mid == NULL)
	{
		LOGE("GetStaticMethodID failed");
		return;
	}

	jstring jmsg = (*env)->NewStringUTF(env, msg);
	if(jmsg == NULL)
	{
		LOGE("NewStringUTF failed");
		return;
	}

	(*env)->CallStaticVoidMethod(env, cls, mid, cmd, jmsg);
	(*env)->DeleteLocalRef(env, jmsg);
}

/***********************************************************
* private                                                  *
***********************************************************/

static gears_renderer_t* gears_renderer = NULL;
static gears_eventq_t*   eventq         = NULL;

/***********************************************************
* public                                                   *
***********************************************************/

JNIEXPORT void JNICALL
Java_com_jeffboody_a3d_A3DNativeRenderer_NativeCreate(JNIEnv* env)
{
	assert(env);
	LOGD("debug");

	if(gears_renderer != NULL)
	{
		LOGE("renderer already exists");
		return;
	}

	if((*env)->GetJavaVM(env, &g_vm) != 0)
	{
		LOGE("GetJavaVM failed");
		return;
	}

	if(a3d_GL_load() == 0)
	{
		LOGE("a3d_GL_load failed");
		goto fail_load;
	}

	gears_renderer = gears_renderer_new(cmd_fn);
	if(gears_renderer == NULL)
	{
		goto fail_renderer;
	}

	eventq = gears_eventq_new();
	if(eventq == NULL)
	{
		goto fail_eventq;
	}

	// success
	return;

	// failure
	fail_eventq:
		gears_renderer_delete(&gears_renderer);
	fail_renderer:
		a3d_GL_unload();
	fail_load:
		g_vm = NULL;
}

JNIEXPORT void JNICALL
Java_com_jeffboody_a3d_A3DNativeRenderer_NativeDestroy(JNIEnv* env)
{
	assert(env);
	LOGD("debug");

	if(gears_renderer)
	{
		gears_eventq_delete(&eventq);
		gears_renderer_delete(&gears_renderer);
		a3d_GL_unload();
		g_vm = NULL;
	}
}

JNIEXPORT void JNICALL
Java_com_jeffboody_a3d_A3DNativeRenderer_NativeChangeSurface(JNIEnv* env, jobject obj, jint w, jint h)
{
	assert(env);
	LOGD("debug");

	if(gears_renderer)
	{
		gears_event_t* e  = gears_eventq_dequeue(eventq);
		e->type           = GEARS_EVENT_RESIZE;
		e->ts             = 0.0;
		e->event_resize.w = w;
		e->event_resize.h = h;
		gears_eventq_enqueue(eventq);
	}
}

JNIEXPORT void JNICALL
Java_com_jeffboody_a3d_A3DNativeRenderer_NativeChangeDensity(JNIEnv* env, jobject obj, jfloat density)
{
	assert(env);
	LOGD("debug");

	if(gears_renderer)
	{
		gears_event_t* e         = gears_eventq_dequeue(eventq);
		e->type                  = GEARS_EVENT_DENSITY;
		e->ts                    = 0.0;
		e->event_density.density = density;
		gears_eventq_enqueue(eventq);
	}
}

JNIEXPORT void JNICALL
Java_com_jeffboody_a3d_A3DNativeRenderer_NativeResume(JNIEnv* env)
{
	assert(env);
	LOGD("debug");
}

JNIEXPORT void JNICALL
Java_com_jeffboody_a3d_A3DNativeRenderer_NativePause(JNIEnv* env)
{
	assert(env);
	LOGD("debug");
}

JNIEXPORT void JNICALL
Java_com_jeffboody_a3d_A3DNativeRenderer_NativeDraw(JNIEnv* env)
{
	assert(env);
	LOGD("debug");

	if(gears_renderer)
	{
		a3d_GL_frame_begin();
		gears_event_t e;
		while(gears_eventq_poll(eventq, &e))
		{
			if(e.type == GEARS_EVENT_RESIZE)
			{
				int w = e.event_resize.w;
				int h = e.event_resize.h;
				gears_renderer_resize(gears_renderer, w, h);
			}
			else if(e.type == GEARS_EVENT_DENSITY)
			{
				float density = e.event_density.density;
				gears_renderer_density(gears_renderer, density);
			}
			else if(e.type == GEARS_EVENT_KEYUP)
			{
				int keycode = e.event_key.keycode;
				int meta    = e.event_key.meta;
				gears_renderer_keyPress(gears_renderer,
				                        keycode, meta);
			}
			else if((e.type == GEARS_EVENT_TOUCHDOWN) ||
			        (e.type == GEARS_EVENT_TOUCHUP)   ||
			        (e.type == GEARS_EVENT_TOUCHMOVE))
			{
				int    action = e.type - GEARS_EVENT_TOUCHDOWN;
				double ts     = e.ts;
				int    count  = e.event_touch.count;
				float  x0     = e.event_touch.coord[0].x;
				float  y0     = e.event_touch.coord[0].y;
				float  x1     = e.event_touch.coord[1].x;
				float  y1     = e.event_touch.coord[1].y;
				float  x2     = e.event_touch.coord[2].x;
				float  y2     = e.event_touch.coord[2].y;
				float  x3     = e.event_touch.coord[3].x;
				float  y3     = e.event_touch.coord[3].y;
				gears_renderer_touch(gears_renderer,
				                     action, count, ts,
				                     x0, y0,
				                     x1, y1,
				                     x2, y2,
				                     x3, y3);
			}
		}
		gears_renderer_draw(gears_renderer);
		a3d_GL_frame_end();
	}
}

JNIEXPORT int JNICALL
Java_com_jeffboody_a3d_A3DNativeRenderer_NativeClientVersion(JNIEnv* env)
{
	assert(env);
	LOGD("debug");
	return 2;
}

JNIEXPORT int JNICALL
Java_com_jeffboody_a3d_A3DNativeRenderer_NativeRed(JNIEnv* env)
{
	assert(env);
	LOGD("debug");
	return 5;
}

JNIEXPORT int JNICALL
Java_com_jeffboody_a3d_A3DNativeRenderer_NativeGreen(JNIEnv* env)
{
	assert(env);
	LOGD("debug");
	return 6;
}

JNIEXPORT int JNICALL
Java_com_jeffboody_a3d_A3DNativeRenderer_NativeBlue(JNIEnv* env)
{
	assert(env);
	LOGD("debug");
	return 5;
}

JNIEXPORT int JNICALL
Java_com_jeffboody_a3d_A3DNativeRenderer_NativeAlpha(JNIEnv* env)
{
	assert(env);
	LOGD("debug");
	return 0;
}

JNIEXPORT int JNICALL
Java_com_jeffboody_a3d_A3DNativeRenderer_NativeDepth(JNIEnv* env)
{
	assert(env);
	LOGD("debug");
	return 16;
}

JNIEXPORT void JNICALL
Java_com_jeffboody_a3d_A3DSurfaceView_NativeTouch(JNIEnv* env, jobject obj,
                                                  jint action, jint count,
                                                  jfloat x0, jfloat y0,
                                                  jfloat x1, jfloat y1,
                                                  jfloat x2, jfloat y2,
                                                  jfloat x3, jfloat y3,
                                                  jdouble ts)
{
	assert(env);

	if(gears_renderer)
	{
		gears_event_t* e          = gears_eventq_dequeue(eventq);
		e->type                   = action + GEARS_EVENT_TOUCHDOWN;
		e->ts                     = ts;
		e->event_touch.count      = count;
		e->event_touch.coord[0].x = x0;
		e->event_touch.coord[0].y = y0;
		e->event_touch.coord[1].x = x1;
		e->event_touch.coord[1].y = y1;
		e->event_touch.coord[2].x = x2;
		e->event_touch.coord[2].y = y2;
		e->event_touch.coord[3].x = x3;
		e->event_touch.coord[3].y = y3;
		gears_eventq_enqueue(eventq);
	}
}

JNIEXPORT void JNICALL
Java_com_jeffboody_GearsES2eclair_GearsES2eclair_NativeKeyDown(JNIEnv* env, jobject obj,
                                                               jint keycode, jint meta, jdouble ts)
{
	assert(env);
	LOGD("debug keycode=0x%X, meta=0x%X, ts=%lf", keycode, meta, ts);

	gears_event_t* e     = gears_eventq_dequeue(eventq);
	e->type              = GEARS_EVENT_KEYDOWN;
	e->ts                = ts;
	e->event_key.keycode = keycode;
	e->event_key.meta    = meta;
	gears_eventq_enqueue(eventq);
}

JNIEXPORT void JNICALL
Java_com_jeffboody_GearsES2eclair_GearsES2eclair_NativeKeyUp(JNIEnv* env, jobject obj,
                                                             jint keycode, jint meta, jdouble ts)
{
	assert(env);
	LOGD("debug keycode=0x%X, meta=0x%X, ts=%lf", keycode, meta, ts);

	if(gears_renderer)
	{
		gears_event_t* e     = gears_eventq_dequeue(eventq);
		e->type              = GEARS_EVENT_KEYUP;
		e->ts                = ts;
		e->event_key.keycode = keycode;
		e->event_key.meta    = meta;
		gears_eventq_enqueue(eventq);
	}
}
