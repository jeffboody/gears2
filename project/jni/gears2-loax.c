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

/***********************************************************
* main                                                     *
***********************************************************/

int main(int argc, char** argv)
{
	int w = 0;
	int h = 0;
	int new_w;
	int new_h;
	loax_client_t* c = loax_client_new();
	if(c == NULL)
	{
		return EXIT_FAILURE;
	}

	// Initialize gears renderer
	gears_renderer = gears_renderer_new("whitrabt.texgz");
	if(gears_renderer == NULL)
	{
		return EXIT_FAILURE;
	}

	do
	{
		loax_client_size(c, &new_w, &new_h);
		if((w == new_w) && (h == new_h))
		{
			// no resize
		}
		else
		{
			w = new_w;
			h = new_h;
			gears_renderer_resize(gears_renderer, w, h);
		}
		gears_renderer_draw(gears_renderer);
	} while(loax_client_swapbuffers(c));

	gears_renderer_delete(&gears_renderer);
	loax_client_delete(&c);

	return EXIT_SUCCESS;
}
