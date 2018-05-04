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
#include "gltest/gltest.h"

/***********************************************************
* private                                                  *
***********************************************************/

/***********************************************************
* public                                                   *
***********************************************************/

JNIEXPORT void JNICALL Java_com_jeffboody_a3d_A3DNativeRenderer_NativeCreate(JNIEnv* env)
{
}

JNIEXPORT void JNICALL Java_com_jeffboody_a3d_A3DNativeRenderer_NativeDestroy(JNIEnv* env)
{
}

JNIEXPORT void JNICALL Java_com_jeffboody_a3d_A3DNativeRenderer_NativeChangeSurface(JNIEnv* env, jobject  obj, jint w, jint h)
{
}

JNIEXPORT void JNICALL Java_com_jeffboody_a3d_A3DNativeRenderer_NativeDraw(JNIEnv* env)
{
	gltest_clear();
}

JNIEXPORT int JNICALL Java_com_jeffboody_a3d_A3DNativeRenderer_NativeClientVersion(JNIEnv* env)
{
	assert(env);
	return 2;
}

JNIEXPORT void JNICALL Java_com_jeffboody_GearsES2eclair_GearsES2eclair_NativeRotate(JNIEnv* env, jobject  obj, jfloat dx, jfloat dy)
{
	assert(env);
}

JNIEXPORT void JNICALL Java_com_jeffboody_GearsES2eclair_GearsES2eclair_NativeScale(JNIEnv* env, jobject  obj, jfloat ds)
{
	assert(env);
}

JNIEXPORT void JNICALL Java_com_jeffboody_GearsES2eclair_GearsES2eclair_NativeRoll(JNIEnv* env, jobject  obj, jfloat roll)
{
	assert(env);
}
