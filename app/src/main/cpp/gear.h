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

#ifndef gear_H
#define gear_H

#include "a3d/a3d_GL.h"
#include "a3d/math/a3d_mat4f.h"
#include "a3d/math/a3d_vec4f.h"

typedef struct
{
	// color
	a3d_vec4f_t color;

	// geometry
	//  ec: element count
	// vid: vertex id
	// nid: normal id
	GLsizei front_ec;
	GLuint  front_vid;
	GLuint  front_nid;
	GLsizei back_ec;
	GLuint  back_vid;
	GLuint  back_nid;
	GLsizei outward_ec;
	GLuint  outward_vid;
	GLuint  outward_nid;
	GLsizei cylinder_ec;
	GLuint  cylinder_vid;
	GLuint  cylinder_nid;

	// shader state
	GLuint program;
	GLint  attribute_vertex;
	GLint  attribute_normal;
	GLint  uniform_color;
	GLint  uniform_nm;
	GLint  uniform_mvp;
} gear_t;

gear_t* gear_new(const a3d_vec4f_t* color,
                 float inner_radius, float outer_radius, float width,
                 int teeth, float tooth_depth);
void    gear_delete(gear_t** _self);
void    gear_draw(gear_t* self, a3d_mat4f_t* mvp, a3d_mat4f_t* mvm);

#endif
