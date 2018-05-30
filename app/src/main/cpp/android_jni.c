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

static gears_renderer_t* gears_renderer = NULL;

/***********************************************************
* private                                                  *
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

	// success
	return;

	// failure
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
		gears_renderer_resize(gears_renderer, w, h);
	}
}

JNIEXPORT void JNICALL
Java_com_jeffboody_a3d_A3DNativeRenderer_NativeChangeDensity(JNIEnv* env, jobject obj, jfloat density)
{
	assert(env);
	LOGD("debug");

	if(gears_renderer)
	{
		gears_renderer_density(gears_renderer, density);
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
		gears_renderer_touch(gears_renderer,
		                     action, count, ts,
		                     x0, y0, x1, y1,
		                     x2, y2, x3, y3);
	}
}

JNIEXPORT void JNICALL
Java_com_jeffboody_GearsES2eclair_GearsES2eclair_NativeKeyDown(JNIEnv* env, jobject obj,
                                                               jint keycode, jint meta, jdouble ts)
{
	assert(env);
	LOGD("debug keycode=0x%X, meta=0x%X, ts=%lf", keycode, meta, ts);

	// ignore
}

JNIEXPORT void JNICALL
Java_com_jeffboody_GearsES2eclair_GearsES2eclair_NativeKeyUp(JNIEnv* env, jobject obj,
                                                             jint keycode, jint meta, jdouble ts)
{
	assert(env);
	LOGD("debug keycode=0x%X, meta=0x%X, ts=%lf", keycode, meta, ts);

	if(gears_renderer)
	{
		gears_renderer_keyPress(gears_renderer, keycode, meta);
	}
}
