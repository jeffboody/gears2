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
#include "gear.h"
#include "a3d/a3d_glsm.h"
#include "a3d/a3d_shader.h"
#include "a3d/math/a3d_vec3f.h"
#include "a3d/math/a3d_vec4f.h"

#define LOG_TAG "gears"
#include "a3d/a3d_log.h"

/***********************************************************
* shaders                                                  *
***********************************************************/

// smooth shading + per-vertex normal + uniform color + uniform mvp
static const char* VSHADER =
	"attribute vec3 vertex;\n"
	"attribute vec3 normal;\n"
	"uniform   mat3 nm;\n"
	"uniform   mat4 mvp;\n"
	"varying   vec4 varying_normal;\n"
	"\n"
	"void main()\n"
	"{\n"
	"	varying_normal = vec4(normalize(nm * normal), 1.0);\n"
	"	gl_Position    = mvp * vec4(vertex, 1.0);\n"
	"}\n";

static const char* FSHADER =
	"#ifdef GL_ES\n"
	"precision mediump float;\n"
	"precision mediump int;\n"
	"#endif\n"
	"\n"
	"uniform vec4 color;\n"
	"varying vec4 varying_normal;\n"
	"\n"
	"void main()\n"
	"{\n"
	"	vec4 frag_color;\n"
	"	vec4 ambient       = vec4(0.2, 0.2, 0.2, 1.0);\n"
	"	vec3 light_position = vec3(5.0, 5.0, 10.0);\n"
	"	light_position      = normalize(light_position);\n"   // TODO - optimize
	"	\n"
	"	float ndotlp  = dot(vec3(varying_normal), light_position);\n"
	"	if(ndotlp > 0.0)\n"
	"	{\n"
	"		vec4 diffuse = vec4(ndotlp, ndotlp, ndotlp, 0.0);\n"
	"		frag_color   = color * (ambient + diffuse);\n"
	"	}\n"
	"	else\n"
	"	{\n"
	"		frag_color = color * ambient;\n"
	"	}\n"
	"	gl_FragColor = frag_color;\n"
	"}\n";

/***********************************************************
* private                                                  *
***********************************************************/

static void gear_angle(int i, int teeth, float* a0, float* a1, float* a2, float* a3)
{
	/*
	 *              +-----+      r2
	 *             /       \
	 *            /         \
	 *           /           \
	 *          /             \
	 *   +-----+               + r1
	 *
	 *         a3   a2   a1    a0
	 *
	 *   +-----+---------------+ r0
	 */

	float angle = i * 2.0f * M_PI / teeth;
	float da = 2.0f * M_PI / teeth / 4.0f;
	*a0 = angle;
	*a1 = angle + 1.0f * da;
	*a2 = angle + 2.0f * da;
	*a3 = angle + 3.0f * da;
}

/***********************************************************
*  Generate a gear wheel.                                  *
*                                                          *
*  Input:  inner_radius - radius of hole at center         *
*          outer_radius - radius at center of teeth        *
*          width - width of gear                           *
*          teeth - number of teeth                         *
*          tooth_depth - depth of tooth                    *
***********************************************************/
static int gear_generate(gear_t* self,
                         float inner_radius, float outer_radius, float width,
                         int teeth, float tooth_depth)
{
	assert(self);
	LOGD("debug");

	a3d_glsm_t* glsm = a3d_glsm_new();
	if(glsm == NULL)
	{
		return 0;
	}

	int i;
	float r0, r1, r2, dz;
	float a0, a1, a2, a3;
	float u, v, len;
	
	r0 = inner_radius;
	r1 = outer_radius - tooth_depth / 2.0f;
	r2 = outer_radius + tooth_depth / 2.0f;
	dz = 0.5f * width;

	// TODO - generate texture coordinates for "shinny"

	// draw front face
	// GL_TRIANGLE_STRIP
	a3d_glsm_begin(glsm);
	a3d_glsm_normal3f(glsm, 0.0f, 0.0f, 1.0f);
	for(i = 0; i < teeth; i++)
	{
		gear_angle(i, teeth, &a0, &a1, &a2, &a3);

		a3d_glsm_vertex3f(glsm, r0 * cosf(a0), r0 * sinf(a0), dz);
		a3d_glsm_vertex3f(glsm, r1 * cosf(a0), r1 * sinf(a0), dz);
		a3d_glsm_vertex3f(glsm, r0 * cosf(a0), r0 * sinf(a0), dz);
		a3d_glsm_vertex3f(glsm, r2 * cosf(a1), r2 * sinf(a1), dz);
		a3d_glsm_vertex3f(glsm, r0 * cosf(a0), r0 * sinf(a0), dz);
		a3d_glsm_vertex3f(glsm, r2 * cosf(a2), r2 * sinf(a2), dz);
		a3d_glsm_vertex3f(glsm, r0 * cosf(a0), r0 * sinf(a0), dz);
		a3d_glsm_vertex3f(glsm, r1 * cosf(a3), r1 * sinf(a3), dz);
	}
	a3d_glsm_vertex3f(glsm, r0 * cosf(0.0f), r0 * sinf(0.0f), dz);
	a3d_glsm_vertex3f(glsm, r1 * cosf(0.0f), r1 * sinf(0.0f), dz);
	a3d_glsm_end(glsm);

	// buffer front face
	if(a3d_glsm_status(glsm) != A3D_GLSM_COMPLETE) goto fail;
	self->front_ec = glsm->ec;
	glBindBuffer(GL_ARRAY_BUFFER, self->front_vid);
	glBufferData(GL_ARRAY_BUFFER, 3 * glsm->ec * sizeof(GLfloat), glsm->vb, GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, self->front_nid);
	glBufferData(GL_ARRAY_BUFFER, 3 * glsm->ec * sizeof(GLfloat), glsm->nb, GL_STATIC_DRAW);

	// draw back face
	// GL_TRIANGLE_STRIP
	a3d_glsm_begin(glsm);
	a3d_glsm_normal3f(glsm, 0.0f, 0.0f, -1.0f);
	for(i = 0; i < teeth; i++)
	{
		gear_angle(i, teeth, &a0, &a1, &a2, &a3);

		a3d_glsm_vertex3f(glsm, r1 * cosf(a0), r1 * sinf(a0), -dz);
		a3d_glsm_vertex3f(glsm, r0 * cosf(a0), r0 * sinf(a0), -dz);
		a3d_glsm_vertex3f(glsm, r2 * cosf(a1), r2 * sinf(a1), -dz);
		a3d_glsm_vertex3f(glsm, r0 * cosf(a0), r0 * sinf(a0), -dz);
		a3d_glsm_vertex3f(glsm, r2 * cosf(a2), r2 * sinf(a2), -dz);
		a3d_glsm_vertex3f(glsm, r0 * cosf(a0), r0 * sinf(a0), -dz);
		a3d_glsm_vertex3f(glsm, r1 * cosf(a3), r1 * sinf(a3), -dz);
		a3d_glsm_vertex3f(glsm, r0 * cosf(a0), r0 * sinf(a0), -dz);
	}
	a3d_glsm_vertex3f(glsm, r1 * cosf(0.0f), r1 * sinf(0.0f), -dz);
	a3d_glsm_vertex3f(glsm, r0 * cosf(0.0f), r0 * sinf(0.0f), -dz);
	a3d_glsm_end(glsm);

	// buffer back face
	if(a3d_glsm_status(glsm) != A3D_GLSM_COMPLETE) goto fail;
	self->back_ec = glsm->ec;
	glBindBuffer(GL_ARRAY_BUFFER, self->back_vid);
	glBufferData(GL_ARRAY_BUFFER, 3 * glsm->ec * sizeof(GLfloat), glsm->vb, GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, self->back_nid);
	glBufferData(GL_ARRAY_BUFFER, 3 * glsm->ec * sizeof(GLfloat), glsm->nb, GL_STATIC_DRAW);

	// draw outward faces of teeth
	// GL_TRIANGLE_STRIP
	// repeated vertices are necessary to achieve flat shading in ES2
	a3d_glsm_begin(glsm);
	for(i = 0; i < teeth; i++)
	{
		gear_angle(i, teeth, &a0, &a1, &a2, &a3);

		if(i > 0)
		{
			a3d_glsm_vertex3f(glsm, r1 * cosf(a0), r1 * sinf(a0), dz);
			a3d_glsm_vertex3f(glsm, r1 * cosf(a0), r1 * sinf(a0), -dz);
		}

		u = r2 * cosf(a1) - r1 * cosf(a0);
		v = r2 * sinf(a1) - r1 * sinf(a0);
		len = sqrtf(u * u + v * v);
		u /= len;
		v /= len;

		a3d_glsm_normal3f(glsm, v, -u, 0.0f);
		a3d_glsm_vertex3f(glsm, r1 * cosf(a0), r1 * sinf(a0), dz);
		a3d_glsm_vertex3f(glsm, r1 * cosf(a0), r1 * sinf(a0), -dz);
		a3d_glsm_vertex3f(glsm, r2 * cosf(a1), r2 * sinf(a1), dz);
		a3d_glsm_vertex3f(glsm, r2 * cosf(a1), r2 * sinf(a1), -dz);

		a3d_glsm_normal3f(glsm, cosf(a0), sinf(a0), 0.0f);
		a3d_glsm_vertex3f(glsm, r2 * cosf(a1), r2 * sinf(a1), dz);
		a3d_glsm_vertex3f(glsm, r2 * cosf(a1), r2 * sinf(a1), -dz);
		a3d_glsm_vertex3f(glsm, r2 * cosf(a2), r2 * sinf(a2), dz);
		a3d_glsm_vertex3f(glsm, r2 * cosf(a2), r2 * sinf(a2), -dz);

		u = r1 * cosf(a3) - r2 * cosf(a2);
		v = r1 * sinf(a3) - r2 * sinf(a2);

		a3d_glsm_normal3f(glsm, v, -u, 0.0f);
		a3d_glsm_vertex3f(glsm, r2 * cosf(a2), r2 * sinf(a2), dz);
		a3d_glsm_vertex3f(glsm, r2 * cosf(a2), r2 * sinf(a2), -dz);
		a3d_glsm_vertex3f(glsm, r1 * cosf(a3), r1 * sinf(a3), dz);
		a3d_glsm_vertex3f(glsm, r1 * cosf(a3), r1 * sinf(a3), -dz);

		a3d_glsm_normal3f(glsm, cosf(a0), sinf(a0), 0.0f);
		a3d_glsm_vertex3f(glsm, r1 * cosf(a3), r1 * sinf(a3), dz);
		a3d_glsm_vertex3f(glsm, r1 * cosf(a3), r1 * sinf(a3), -dz);
	}
	a3d_glsm_vertex3f(glsm, r1 * cosf(0.0f), r1 * sinf(0.0f), dz);
	a3d_glsm_vertex3f(glsm, r1 * cosf(0.0f), r1 * sinf(0.0f), -dz);
	a3d_glsm_end(glsm);

	// buffer outward faces of teeth
	if(a3d_glsm_status(glsm) != A3D_GLSM_COMPLETE) goto fail;
	self->outward_ec = glsm->ec;
	glBindBuffer(GL_ARRAY_BUFFER, self->outward_vid);
	glBufferData(GL_ARRAY_BUFFER, 3 * glsm->ec * sizeof(GLfloat), glsm->vb, GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, self->outward_nid);
	glBufferData(GL_ARRAY_BUFFER, 3 * glsm->ec * sizeof(GLfloat), glsm->nb, GL_STATIC_DRAW);

	// draw inside radius cylinder
	// GL_TRIANGLE_STRIP
	a3d_glsm_begin(glsm);
	for(i = 0; i < teeth; i++)
	{
		gear_angle(i, teeth, &a0, &a1, &a2, &a3);

		a3d_glsm_normal3f(glsm, -cosf(a0), -sinf(a0), 0.0f);
		a3d_glsm_vertex3f(glsm, r0 * cosf(a0), r0 * sinf(a0), -dz);
		a3d_glsm_vertex3f(glsm, r0 * cosf(a0), r0 * sinf(a0), dz);
	}
	a3d_glsm_normal3f(glsm, -cosf(0.0f), -sinf(0.0f), 0.0f);
	a3d_glsm_vertex3f(glsm, r0 * cosf(0.0f), r0 * sinf(0.0f), -dz);
	a3d_glsm_vertex3f(glsm, r0 * cosf(0.0f), r0 * sinf(0.0f), dz);
	a3d_glsm_end(glsm);

	// buffer inside radius cylinder
	if(a3d_glsm_status(glsm) != A3D_GLSM_COMPLETE) goto fail;
	self->cylinder_ec = glsm->ec;
	glBindBuffer(GL_ARRAY_BUFFER, self->cylinder_vid);
	glBufferData(GL_ARRAY_BUFFER, 3 * glsm->ec * sizeof(GLfloat), glsm->vb, GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, self->cylinder_nid);
	glBufferData(GL_ARRAY_BUFFER, 3 * glsm->ec * sizeof(GLfloat), glsm->nb, GL_STATIC_DRAW);

	GLenum e = A3D_GL_GETERROR();
	if(e != GL_NO_ERROR)
		goto fail;

	// success
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	a3d_glsm_delete(&glsm);
	return 1;

	// failure
	fail:
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		a3d_glsm_delete(&glsm);
	return 0;
}

static int gear_load_shaders(gear_t* self)
{
	assert(self);
	LOGD("debug");

	self->program = a3d_shader_make_source(VSHADER, FSHADER);
	if(self->program == 0)
		return 0;

	self->attribute_vertex = glGetAttribLocation(self->program, "vertex");
	self->attribute_normal = glGetAttribLocation(self->program, "normal");
	self->uniform_color    = glGetUniformLocation(self->program, "color");
	self->uniform_nm       = glGetUniformLocation(self->program, "nm");
	self->uniform_mvp      = glGetUniformLocation(self->program, "mvp");

	GLenum e = A3D_GL_GETERROR();
	if(e != GL_NO_ERROR)
		goto fail_error;

	// success
	return 1;

	// failure
	fail_error:
		glDeleteProgram(self->program);
	return 0;
}

/***********************************************************
* public                                                   *
***********************************************************/

gear_t* gear_new(const a3d_vec4f_t* color,
                 float inner_radius, float outer_radius, float width,
                 int teeth, float tooth_depth)
{
	LOGD("debug r=%f, g=%f, b=%f, a=%f", color->x, color->y, color->z, color->w);
	LOGD("debug inner_radius=%f, outer_radius=%f, width=%f", inner_radius, outer_radius, width);
	LOGD("debug teeth=%i, tooth_depth=%f", teeth, tooth_depth);

	gear_t* self = (gear_t*) malloc(sizeof(gear_t));
	if(self == NULL)
	{
		LOGE("malloc failed");
		return NULL;
	}

	// initialize gear
	a3d_vec4f_copy(color, &self->color);
	glGenBuffers(1, &self->front_vid);
	glGenBuffers(1, &self->front_nid);
	glGenBuffers(1, &self->back_vid);
	glGenBuffers(1, &self->back_nid);
	glGenBuffers(1, &self->outward_vid);
	glGenBuffers(1, &self->outward_nid);
	glGenBuffers(1, &self->cylinder_vid);
	glGenBuffers(1, &self->cylinder_nid);
	if(gear_generate(self,
	                 inner_radius, outer_radius, width,
                     teeth, tooth_depth) == 0)
	{
		LOGE("gear_generate failed");
		goto fail_gear_generate;
	}

	// initialize shader state
	if(gear_load_shaders(self) == 0)
		goto fail_shader;

	// success
	return self;

	// failure
	fail_shader:
		// gear_generate only fills in buffers
	fail_gear_generate:
		glDeleteBuffers(1, &self->cylinder_nid);
		glDeleteBuffers(1, &self->cylinder_vid);
		glDeleteBuffers(1, &self->outward_nid);
		glDeleteBuffers(1, &self->outward_vid);
		glDeleteBuffers(1, &self->back_nid);
		glDeleteBuffers(1, &self->back_vid);
		glDeleteBuffers(1, &self->front_nid);
		glDeleteBuffers(1, &self->front_vid);
		free(self);
	return NULL;
}

void gear_delete(gear_t** _self)
{
	// *_self can be null
	assert(_self);

	gear_t* self = *_self;
	if(self)
	{
		LOGD("debug");
		glDeleteProgram(self->program);
		glDeleteBuffers(1, &self->cylinder_nid);
		glDeleteBuffers(1, &self->cylinder_vid);
		glDeleteBuffers(1, &self->outward_nid);
		glDeleteBuffers(1, &self->outward_vid);
		glDeleteBuffers(1, &self->back_nid);
		glDeleteBuffers(1, &self->back_vid);
		glDeleteBuffers(1, &self->front_nid);
		glDeleteBuffers(1, &self->front_vid);
		free(self);
		*_self = NULL;
	}
}

void gear_draw(gear_t* self, a3d_mat4f_t* mvp, a3d_mat4f_t* mvm)
{
	assert(self);
	LOGD("debug");

	a3d_mat3f_t nm;
	a3d_mat4f_normalmatrix(mvm, &nm);

	glUseProgram(self->program);
	glEnableVertexAttribArray(self->attribute_vertex);
	glEnableVertexAttribArray(self->attribute_normal);

	glUniform4fv(self->uniform_color, 1, (GLfloat*) &self->color);
	glUniformMatrix3fv(self->uniform_nm, 1, GL_FALSE, (GLfloat*) &nm);
	glUniformMatrix4fv(self->uniform_mvp, 1, GL_FALSE, (GLfloat*) mvp);

	// front teeth
	glBindBuffer(GL_ARRAY_BUFFER, self->front_vid);
	glVertexAttribPointer(self->attribute_vertex, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glBindBuffer(GL_ARRAY_BUFFER, self->front_nid);
	glVertexAttribPointer(self->attribute_normal, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glDrawArrays(GL_TRIANGLE_STRIP, 0, self->front_ec);

	// back teeth
	glBindBuffer(GL_ARRAY_BUFFER, self->back_vid);
	glVertexAttribPointer(self->attribute_vertex, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glBindBuffer(GL_ARRAY_BUFFER, self->back_nid);
	glVertexAttribPointer(self->attribute_normal, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glDrawArrays(GL_TRIANGLE_STRIP, 0, self->back_ec);

	// outward teeth
	glBindBuffer(GL_ARRAY_BUFFER, self->outward_vid);
	glVertexAttribPointer(self->attribute_vertex, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glBindBuffer(GL_ARRAY_BUFFER, self->outward_nid);
	glVertexAttribPointer(self->attribute_normal, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glDrawArrays(GL_TRIANGLE_STRIP, 0, self->outward_ec);

	// cylinder
	glBindBuffer(GL_ARRAY_BUFFER, self->cylinder_vid);
	glVertexAttribPointer(self->attribute_vertex, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glBindBuffer(GL_ARRAY_BUFFER, self->cylinder_nid);
	glVertexAttribPointer(self->attribute_normal, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glDrawArrays(GL_TRIANGLE_STRIP, 0, self->cylinder_ec);

	glDisableVertexAttribArray(self->attribute_normal);
	glDisableVertexAttribArray(self->attribute_vertex);
	glUseProgram(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}
