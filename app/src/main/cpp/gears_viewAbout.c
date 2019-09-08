/*
 * Copyright (c) 2018 Jeff Boody
 */

#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include "gears_viewAbout.h"
#include "gears_overlay.h"
#include "gears_renderer.h"

#define LOG_TAG "gears"
#include "a3d/a3d_log.h"

/***********************************************************
* private                                                  *
***********************************************************/

static int clickGithub(a3d_widget_t* widget,
                       void* priv, int state,
                       float x, float y)
{
	assert(widget);
	assert(priv);
	LOGD("debug x=%f, y=%f", x, y);

	if(state == A3D_WIDGET_POINTER_UP)
	{
		gears_overlay_t*  overlay  = (gears_overlay_t*) priv;
		gears_renderer_t* renderer = overlay->renderer;
		gears_renderer_loadURL(renderer, "https://github.com/jeffboody/gears2/");
	}
	return 1;
}

static int clickIcons(a3d_widget_t* widget,
                      void* priv, int state,
                      float x, float y)
{
	assert(widget);
	assert(priv);
	LOGD("debug x=%f, y=%f", x, y);

	if(state == A3D_WIDGET_POINTER_UP)
	{
		gears_overlay_t*  overlay  = (gears_overlay_t*) priv;
		gears_renderer_t* renderer = overlay->renderer;
		gears_renderer_loadURL(renderer, "https://github.com/google/material-design-icons/");
	}
	return 1;
}

static int clickBarlow(a3d_widget_t* widget,
                       void* priv, int state,
                       float x, float y)
{
	assert(widget);
	assert(priv);
	LOGD("debug x=%f, y=%f", x, y);

	if(state == A3D_WIDGET_POINTER_UP)
	{
		gears_overlay_t*  overlay  = (gears_overlay_t*) priv;
		gears_renderer_t* renderer = overlay->renderer;
		gears_renderer_loadURL(renderer, "https://github.com/jpt/barlow/");
	}
	return 1;
}

static int clickExpat(a3d_widget_t* widget,
                      void* priv, int state,
                      float x, float y)
{
	assert(widget);
	assert(priv);
	LOGD("debug x=%f, y=%f", x, y);

	if(state == A3D_WIDGET_POINTER_UP)
	{
		gears_overlay_t*  overlay  = (gears_overlay_t*) priv;
		gears_renderer_t* renderer = overlay->renderer;
		gears_renderer_loadURL(renderer, "https://github.com/jeffboody/libexpat/");
	}
	return 1;
}

/***********************************************************
* public                                                   *
***********************************************************/

gears_viewAbout_t* gears_viewAbout_new(struct gears_overlay_s* overlay,
                                       a3d_widget_clickFn clickBack)
{
	assert(overlay);
	assert(clickBack);

	a3d_widgetLayout_t layout_listbox =
	{
		.border     = A3D_WIDGET_BORDER_LARGE,
		.wrapx      = A3D_WIDGET_WRAP_STRETCH_PARENT,
		.wrapy      = A3D_WIDGET_WRAP_SHRINK,
		.aspectx    = A3D_WIDGET_ASPECT_DEFAULT,
		.aspecty    = A3D_WIDGET_ASPECT_DEFAULT,
		.stretchx   = 1.0f,
		.stretchy   = 1.0f
	};

	a3d_widgetScroll_t scroll_listbox =
	{
		.scroll_bar = 1,
		.color_scroll0 =
		{
			// ltgray
			.r = 0.95f,
			.g = 0.95f,
			.b = 0.95f,
			.a = 1.0f
		},
		.color_scroll1 =
		{
			// dkgray
			.r = 0.4f,
			.g = 0.4f,
			.b = 0.4f,
			.a = 1.0f
		}
	};

	a3d_widgetFn_t fn;
	memset(&fn, 0, sizeof(a3d_widgetFn_t));

	a3d_listbox_t* listbox;
	listbox = a3d_listbox_new(overlay->screen, 0,
	                          &layout_listbox,
	                          &scroll_listbox, &fn,
	                          A3D_LISTBOX_ORIENTATION_VERTICAL);
	if(listbox == NULL)
	{
		return NULL;
	}

	a3d_widgetLayout_t layout_about =
	{
		.border   = A3D_WIDGET_BORDER_MEDIUM,
		.wrapx    = A3D_WIDGET_WRAP_SHRINK,
		.wrapy    = A3D_WIDGET_WRAP_STRETCH_PARENT,
		.aspectx  = A3D_WIDGET_ASPECT_DEFAULT,
		.aspecty  = A3D_WIDGET_ASPECT_DEFAULT,
		.stretchx = 1.0f,
		.stretchy = 1.0f
	};

	a3d_widgetFn_t fn_about =
	{
		.priv     = (void*) overlay,
		.click_fn = clickBack
	};

	a3d_viewboxStyle_t viewbox_style =
	{
		.widget_style =
		{
			.color_header =
			{
				.r = 0.04f,
				.g = 0.54f,
				.b = 0.89f,
				.a = 1.0f
			},
			.color_body =
			{
				.r = 1.0f,
				.g = 1.0f,
				.b = 1.0f,
				.a = 1.0f
			}

		},
		.text_style =
		{
			.font_type = A3D_TEXT_FONTTYPE_BOLD,
			.size      = A3D_TEXT_SIZE_MEDIUM,
			.spacing   = A3D_TEXT_SPACING_SMALL,
			.color     =
			{
				.r = 1.0f,
				.g = 1.0f,
				.b = 1.0f,
				.a = 1.0f,
			}
		}
	};

	gears_viewAbout_t* self;
	self = (gears_viewAbout_t*)
	       a3d_viewbox_new(overlay->screen,
	                       sizeof(gears_viewAbout_t),
	                       &layout_about, &fn_about,
	                       &viewbox_style, 1,
	                       (a3d_widget_t*) listbox, NULL);
	if(self == NULL)
	{
		goto fail_view;
	}
	self->listbox = listbox;

	a3d_viewbox_t* viewbox = (a3d_viewbox_t*) self;
	if(a3d_viewbox_spriteLoad(viewbox, 0,
	                          "$ic_arrow_back_white_24dp.texz") == 0)
	{
		goto fail_sprite;
	}

	a3d_textStyle_t text_style_heading =
	{
		.font_type = A3D_TEXT_FONTTYPE_BOLD,
		.size      = A3D_TEXT_SIZE_MEDIUM,
		.spacing   = A3D_TEXT_SPACING_MEDIUM,
		.color     =
		{
			.r = 0.0f,
			.g = 0.0f,
			.b = 0.0f,
			.a = 1.0f,
		}
	};

	a3d_textFn_t text_fn_heading;
	memset(&text_fn_heading, 0, sizeof(a3d_textFn_t));

	a3d_text_t* text_intro;
	text_intro = a3d_text_new(overlay->screen, 0,
	                          &text_style_heading,
	                          &text_fn_heading);
	if(text_intro == NULL)
	{
		goto fail_text_intro;
	}
	self->text_intro = text_intro;

	a3d_text_t* text_icons;
	text_icons = a3d_text_new(overlay->screen, 0,
	                          &text_style_heading,
	                          &text_fn_heading);
	if(text_icons == NULL)
	{
		goto fail_text_icons;
	}
	self->text_icons = text_icons;

	a3d_text_t* text_barlow;
	text_barlow = a3d_text_new(overlay->screen, 0,
	                           &text_style_heading,
	                           &text_fn_heading);
	if(text_barlow == NULL)
	{
		goto fail_text_barlow;
	}
	self->text_barlow = text_barlow;

	a3d_text_t* text_expat;
	text_expat = a3d_text_new(overlay->screen, 0,
	                          &text_style_heading,
	                          &text_fn_heading);
	if(text_expat == NULL)
	{
		goto fail_text_expat;
	}
	self->text_expat = text_expat;

	a3d_text_t* text_license;
	text_license = a3d_text_new(overlay->screen, 0,
	                            &text_style_heading,
	                            &text_fn_heading);
	if(text_license == NULL)
	{
		goto fail_text_license;
	}
	self->text_license = text_license;

	a3d_widgetLayout_t layout_textbox =
	{
		.wrapx    = A3D_WIDGET_WRAP_STRETCH_PARENT,
		.wrapy    = A3D_WIDGET_WRAP_SHRINK,
		.aspectx  = A3D_WIDGET_ASPECT_DEFAULT,
		.aspecty  = A3D_WIDGET_ASPECT_DEFAULT,
		.stretchx = 1.0f,
		.stretchy = 1.0f
	};

	a3d_textStyle_t style_textbox =
	{
		.font_type = A3D_TEXT_FONTTYPE_REGULAR,
		.size      = A3D_TEXT_SIZE_MEDIUM,
		.spacing   = A3D_TEXT_SPACING_SMALL,
		.color     =
		{
			.r = 0.0f,
			.g = 0.0f,
			.b = 0.0f,
			.a = 1.0f,
		}
	};

	a3d_widgetScroll_t scroll_textbox;
	memset(&scroll_textbox, 0, sizeof(a3d_widgetScroll_t));

	a3d_widgetFn_t fn_textbox;
	memset(&fn_textbox, 0, sizeof(a3d_widgetFn_t));

	a3d_textbox_t* textbox_intro;
	textbox_intro = a3d_textbox_new(overlay->screen, 0,
	                                &layout_textbox,
	                                &scroll_textbox,
	                                &fn_textbox,
	                                &style_textbox);
	if(textbox_intro == NULL)
	{
		goto fail_textbox_intro;
	}
	self->textbox_intro = textbox_intro;

	a3d_textbox_t* textbox_icons;
	textbox_icons = a3d_textbox_new(overlay->screen, 0,
	                                &layout_textbox,
	                                &scroll_textbox,
	                                &fn_textbox,
	                                &style_textbox);
	if(textbox_icons == NULL)
	{
		goto fail_textbox_icons;
	}
	self->textbox_icons = textbox_icons;

	a3d_textbox_t* textbox_barlow;
	textbox_barlow = a3d_textbox_new(overlay->screen, 0,
	                                 &layout_textbox,
	                                 &scroll_textbox,
	                                 &fn_textbox,
	                                 &style_textbox);
	if(textbox_barlow == NULL)
	{
		goto fail_textbox_barlow;
	}
	self->textbox_barlow = textbox_barlow;

	a3d_textbox_t* textbox_expat;
	textbox_expat = a3d_textbox_new(overlay->screen, 0,
	                                &layout_textbox,
	                                &scroll_textbox,
	                                &fn_textbox,
	                                &style_textbox);
	if(textbox_expat == NULL)
	{
		goto fail_textbox_expat;
	}
	self->textbox_expat = textbox_expat;

	a3d_textbox_t* textbox_license;
	textbox_license = a3d_textbox_new(overlay->screen, 0,
	                                  &layout_textbox,
	                                  &scroll_textbox,
	                                  &fn_textbox,
	                                  &style_textbox);
	if(textbox_license == NULL)
	{
		goto fail_textbox_license;
	}
	self->textbox_license = textbox_license;

	a3d_textStyle_t style_linkbox =
	{
		.font_type = A3D_TEXT_FONTTYPE_REGULAR,
		.size      = A3D_TEXT_SIZE_MEDIUM,
		.spacing   = A3D_TEXT_SPACING_SMALL,
		.color     =
		{
			.r = 0.04f,
			.g = 0.54f,
			.b = 0.89f,
			.a = 1.0f
		}
	};

	a3d_widgetFn_t fn_link =
	{
		.priv     = (void*) overlay,
		.click_fn = clickGithub
	};

	a3d_textbox_t* linkbox_github;
	linkbox_github = a3d_textbox_new(overlay->screen, 0,
	                                 &layout_textbox,
	                                 &scroll_textbox,
	                                 &fn_link,
	                                 &style_linkbox);
	if(linkbox_github == NULL)
	{
		goto fail_linkbox_github;
	}
	self->linkbox_github = linkbox_github;

	a3d_textbox_t* linkbox_icons;
	fn_link.click_fn = clickIcons;
	linkbox_icons = a3d_textbox_new(overlay->screen, 0,
	                                &layout_textbox,
	                                &scroll_textbox,
	                                &fn_link,
	                                &style_linkbox);
	if(linkbox_icons == NULL)
	{
		goto fail_linkbox_icons;
	}
	self->linkbox_icons = linkbox_icons;

	a3d_textbox_t* linkbox_barlow;
	fn_link.click_fn = clickBarlow;
	linkbox_barlow = a3d_textbox_new(overlay->screen, 0,
	                                 &layout_textbox,
	                                 &scroll_textbox,
	                                 &fn_link,
	                                 &style_linkbox);
	if(linkbox_barlow == NULL)
	{
		goto fail_linkbox_barlow;
	}
	self->linkbox_barlow = linkbox_barlow;

	a3d_textbox_t* linkbox_expat;
	fn_link.click_fn = clickExpat;
	linkbox_expat = a3d_textbox_new(overlay->screen, 0,
	                                &layout_textbox,
	                                &scroll_textbox,
	                                &fn_link,
	                                &style_linkbox);
	if(linkbox_expat == NULL)
	{
		goto fail_linkbox_expat;
	}
	self->linkbox_expat = linkbox_expat;

	a3d_viewbox_textPrintf(&self->viewbox, "%s", "About");

	a3d_text_printf(text_intro, "%s", "Introduction");
	a3d_textbox_printf(textbox_intro, "%s", "Gears for Android is a heavily modified port of the famous \"gears\" demo");
	a3d_textbox_printf(textbox_intro, "%s", "to Android.");
	a3d_textbox_printf(textbox_intro, "%s", "");
	a3d_textbox_printf(textbox_intro, "%s", "The Gears demo is an open source project intended to help developers learn");
	a3d_textbox_printf(textbox_intro, "%s", "how to create OpenGL ES programs on Android. The Gears demo was originally");
	a3d_textbox_printf(textbox_intro, "%s", "written by Brian Paul as part of the Mesa3D project. My implementation");
	a3d_textbox_printf(textbox_intro, "%s", "includes variations for Java/OpenGL ES 1.x, Java/C/OpenGL ES 1.x and");
	a3d_textbox_printf(textbox_intro, "%s", "Java/C/OpenGL ES 2.0. I have also added several features not found in the");
	a3d_textbox_printf(textbox_intro, "%s", "original implementation including touch screen support, VBOs and an");
	a3d_textbox_printf(textbox_intro, "%s", "on-screen FPS counter.");
	a3d_textbox_printf(textbox_intro, "%s", "");
	a3d_textbox_printf(textbox_intro, "%s", "The FPS (frames-per-second) counter is often used as a benchmark metric for");
	a3d_textbox_printf(textbox_intro, "%s", "graphics programs. On Android the frame rate is limited by v-sync (typically");
	a3d_textbox_printf(textbox_intro, "%s", "60 FPS) which is the fastest rate that a display can refresh the screen.");
	a3d_textbox_printf(textbox_intro, "%s", "Since Gears is capable of rendering much faster than v-sync on most devices");
	a3d_textbox_printf(textbox_intro, "%s", "it provides limited benchmarking value.");
	a3d_textbox_printf(textbox_intro, "%s", "");
	a3d_textbox_printf(textbox_intro, "%s", "Send questions or comments to Jeff Boody at jeffboody@gmail.com.");
	a3d_textbox_printf(linkbox_github, "%s", "https://github.com/jeffboody/gears2/");
	a3d_textbox_printf(linkbox_github, "%s", "");

	a3d_text_printf(text_icons, "%s", "Material Design Icons");
	a3d_textbox_printf(textbox_icons, "%s", "By Google, under the Apache License, Version 2.0.");
	a3d_textbox_printf(linkbox_icons, "%s", "https://github.com/google/material-design-icons/");
	a3d_textbox_printf(linkbox_icons, "%s", "");

	a3d_text_printf(text_barlow, "%s", "Barlow Font");
	a3d_textbox_printf(textbox_barlow, "%s", "Font support is provided by The Barlow Project Authors,");
	a3d_textbox_printf(textbox_barlow, "%s", "under the SIL Open Font License, Version 1.1.");
	a3d_textbox_printf(linkbox_barlow, "%s", "https://github.com/jpt/barlow/");
	a3d_textbox_printf(linkbox_barlow, "%s", "");

	a3d_text_printf(text_expat, "%s", "Expat XML Parser");
	a3d_textbox_printf(textbox_expat, "%s", "XML parsing provided by the Expat XML Parser");
	a3d_textbox_printf(textbox_expat, "%s", "under the MIT license.");
	a3d_textbox_printf(linkbox_expat, "%s", "https://github.com/jeffboody/libexpat/");
	a3d_textbox_printf(linkbox_expat, "%s", "");

	a3d_text_printf(text_license, "%s", "Gears Source License");
	a3d_textbox_printf(textbox_license, "%s", "Copyright (c) 2009-2011 Jeff Boody");
	a3d_textbox_printf(textbox_license, "%s", "");
	a3d_textbox_printf(textbox_license, "%s", "Gears for Android is a heavily modified port of the famous \"gears\" demo. ");
	a3d_textbox_printf(textbox_license, "%s", "As such, it is a derived work subject to the license requirements (below) ");
	a3d_textbox_printf(textbox_license, "%s", "of the original work.");
	a3d_textbox_printf(textbox_license, "%s", "");
	a3d_textbox_printf(textbox_license, "%s", "Copyright (c) 1999-2001 Brian Paul All Rights Reserved.");
	a3d_textbox_printf(textbox_license, "%s", "");
	a3d_textbox_printf(textbox_license, "%s", "Permission is hereby granted, free of charge, to any person obtaining a");
	a3d_textbox_printf(textbox_license, "%s", "copy of this software and associated documentation files (the \"Software\"),");
	a3d_textbox_printf(textbox_license, "%s", "to deal in the Software without restriction, including without limitation");
	a3d_textbox_printf(textbox_license, "%s", "the rights to use, copy, modify, merge, publish, distribute, sublicense,");
	a3d_textbox_printf(textbox_license, "%s", "and/or sell copies of the Software, and to permit persons to whom the");
	a3d_textbox_printf(textbox_license, "%s", "Software is furnished to do so, subject to the following conditions:");
	a3d_textbox_printf(textbox_license, "%s", "");
	a3d_textbox_printf(textbox_license, "%s", "The above copyright notice and this permission notice shall be included");
	a3d_textbox_printf(textbox_license, "%s", "in all copies or substantial portions of the Software.");
	a3d_textbox_printf(textbox_license, "%s", "");
	a3d_textbox_printf(textbox_license, "%s", "THE SOFTWARE IS PROVIDED \"AS IS\", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR");
	a3d_textbox_printf(textbox_license, "%s", "IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,");
	a3d_textbox_printf(textbox_license, "%s", "FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE");
	a3d_textbox_printf(textbox_license, "%s", "AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER");
	a3d_textbox_printf(textbox_license, "%s", "LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,");
	a3d_textbox_printf(textbox_license, "%s", "OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN");
	a3d_textbox_printf(textbox_license, "%s", "THE SOFTWARE.");

	a3d_list_t* widgets = a3d_listbox_widgets(listbox);
	a3d_list_enqueue(widgets, (const void*) text_intro);
	a3d_list_enqueue(widgets, (const void*) textbox_intro);
	a3d_list_enqueue(widgets, (const void*) linkbox_github);
	a3d_list_enqueue(widgets, (const void*) text_icons);
	a3d_list_enqueue(widgets, (const void*) textbox_icons);
	a3d_list_enqueue(widgets, (const void*) linkbox_icons);
	a3d_list_enqueue(widgets, (const void*) text_barlow);
	a3d_list_enqueue(widgets, (const void*) textbox_barlow);
	a3d_list_enqueue(widgets, (const void*) linkbox_barlow);
	a3d_list_enqueue(widgets, (const void*) text_expat);
	a3d_list_enqueue(widgets, (const void*) textbox_expat);
	a3d_list_enqueue(widgets, (const void*) linkbox_expat);
	a3d_list_enqueue(widgets, (const void*) text_license);
	a3d_list_enqueue(widgets, (const void*) textbox_license);

	// success
	return self;

	// failure
	fail_linkbox_expat:
		a3d_textbox_delete(&linkbox_barlow);
	fail_linkbox_barlow:
		a3d_textbox_delete(&linkbox_icons);
	fail_linkbox_icons:
		a3d_textbox_delete(&linkbox_github);
	fail_linkbox_github:
		a3d_textbox_delete(&textbox_license);
	fail_textbox_license:
		a3d_textbox_delete(&textbox_expat);
	fail_textbox_expat:
		a3d_textbox_delete(&textbox_barlow);
	fail_textbox_barlow:
		a3d_textbox_delete(&textbox_icons);
	fail_textbox_icons:
		a3d_textbox_delete(&textbox_intro);
	fail_textbox_intro:
		a3d_text_delete(&text_license);
	fail_text_license:
		a3d_text_delete(&text_expat);
	fail_text_expat:
		a3d_text_delete(&text_barlow);
	fail_text_barlow:
		a3d_text_delete(&text_icons);
	fail_text_icons:
		a3d_text_delete(&text_intro);
	fail_text_intro:
	fail_sprite:
		a3d_viewbox_delete((a3d_viewbox_t**) &self);
	fail_view:
		a3d_listbox_delete(&listbox);
	return NULL;
}

void gears_viewAbout_delete(gears_viewAbout_t** _self)
{
	assert(_self);

	gears_viewAbout_t* self = *_self;
	if(self)
	{
		a3d_listbox_clear(self->listbox);
		a3d_textbox_delete(&self->linkbox_expat);
		a3d_textbox_delete(&self->linkbox_barlow);
		a3d_textbox_delete(&self->linkbox_icons);
		a3d_textbox_delete(&self->linkbox_github);
		a3d_textbox_delete(&self->textbox_license);
		a3d_textbox_delete(&self->textbox_expat);
		a3d_textbox_delete(&self->textbox_barlow);
		a3d_textbox_delete(&self->textbox_icons);
		a3d_textbox_delete(&self->textbox_intro);
		a3d_text_delete(&self->text_license);
		a3d_text_delete(&self->text_expat);
		a3d_text_delete(&self->text_barlow);
		a3d_text_delete(&self->text_icons);
		a3d_text_delete(&self->text_intro);
		a3d_listbox_delete(&self->listbox);
		a3d_viewbox_delete((a3d_viewbox_t**) _self);
	}
}
