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
#include "gears_renderer.h"

#define LOG_TAG "gears"
#include "a3d/a3d_log.h"

/***********************************************************
* private                                                  *
***********************************************************/

static gears_renderer_t* gears_renderer = NULL;

/***********************************************************
* public                                                   *
***********************************************************/

JNIEXPORT void JNICALL Java_com_jeffboody_a3d_A3DNativeRenderer_NativeCreate(JNIEnv* env)
{
	assert(env);
	LOGD("debug");

	if(gears_renderer != NULL)
	{
		LOGE("renderer already exists");
		return;
	}

	if(a3d_GL_load() == 0)
	{
		LOGE("a3d_GL_load failed");
		return;
	}

	gears_renderer = gears_renderer_new();
	if(gears_renderer == NULL)
	{
		a3d_GL_unload();
	}
}

JNIEXPORT void JNICALL Java_com_jeffboody_a3d_A3DNativeRenderer_NativeDestroy(JNIEnv* env)
{
	assert(env);
	LOGD("debug");

	if(gears_renderer)
	{
		gears_renderer_delete(&gears_renderer);
		a3d_GL_unload();
	}
}

JNIEXPORT void JNICALL Java_com_jeffboody_a3d_A3DNativeRenderer_NativeChangeSurface(JNIEnv* env, jobject  obj, jint w, jint h)
{
	assert(env);
	LOGD("debug");

	if(gears_renderer)
	{
		gears_renderer_resize(gears_renderer, w, h);
	}
}

JNIEXPORT void JNICALL Java_com_jeffboody_a3d_A3DNativeRenderer_NativeDraw(JNIEnv* env)
{
	assert(env);
	LOGD("debug");

	if(gears_renderer)
	{
		a3d_GL_frame_begin();
		gears_renderer_draw(gears_renderer);
		a3d_GL_frame_end();
	}
}

JNIEXPORT int JNICALL Java_com_jeffboody_a3d_A3DNativeRenderer_NativeClientVersion(JNIEnv* env)
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

JNIEXPORT void JNICALL Java_com_jeffboody_GearsES2eclair_GearsES2eclair_NativeRotate(JNIEnv* env, jobject  obj, jfloat dx, jfloat dy)
{
	assert(env);
	LOGD("debug dx=%f, dy=%f", (float) dx, (float) dy);

	if(gears_renderer)
	{
		gears_renderer_rotate(gears_renderer, (float) dx, (float) dy);
	}
}

JNIEXPORT void JNICALL Java_com_jeffboody_GearsES2eclair_GearsES2eclair_NativeScale(JNIEnv* env, jobject  obj, jfloat ds)
{
	assert(env);
	LOGD("debug ds=%f", (float) ds);

	if(gears_renderer)
	{
		gears_renderer_scale(gears_renderer, (float) ds);
	}
}

JNIEXPORT void JNICALL Java_com_jeffboody_GearsES2eclair_GearsES2eclair_NativeRoll(JNIEnv* env, jobject  obj, jfloat roll)
{
	assert(env);
	LOGD("debug roll=%f", (float) roll);

	if(gears_renderer)
	{
		gears_renderer_roll(gears_renderer, (float) roll);
	}
}
