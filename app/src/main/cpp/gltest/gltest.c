/*
 * Copyright (c) 2018 Jeff Boody
 */

#include "gltest.h"
#include <GLES2/gl2.h>

void gltest_clear(void)
{
	glClearColor(1.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}
