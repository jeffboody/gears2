/*
 * Copyright (c) 2018 Jeff Boody
 */

#include <stdlib.h>
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
                       int state,
                       float x, float y)
{
	assert(widget);
	LOGD("debug x=%f, y=%f", x, y);

	if(state == A3D_WIDGET_POINTER_UP)
	{
		gears_overlay_t*  overlay  = (gears_overlay_t*) widget->priv;
		gears_renderer_t* renderer = overlay->renderer;
		gears_renderer_loadURL(renderer, "https://github.com/jeffboody/gears2/");
	}
	return 1;
}

static int clickIcons(a3d_widget_t* widget,
                      int state,
                      float x, float y)
{
	assert(widget);
	LOGD("debug x=%f, y=%f", x, y);

	if(state == A3D_WIDGET_POINTER_UP)
	{
		gears_overlay_t*  overlay  = (gears_overlay_t*) widget->priv;
		gears_renderer_t* renderer = overlay->renderer;
		gears_renderer_loadURL(renderer, "https://github.com/google/material-design-icons/");
	}
	return 1;
}

static int clickBarlow(a3d_widget_t* widget,
                       int state,
                       float x, float y)
{
	assert(widget);
	LOGD("debug x=%f, y=%f", x, y);

	if(state == A3D_WIDGET_POINTER_UP)
	{
		gears_overlay_t*  overlay  = (gears_overlay_t*) widget->priv;
		gears_renderer_t* renderer = overlay->renderer;
		gears_renderer_loadURL(renderer, "https://github.com/jpt/barlow/");
	}
	return 1;
}

/***********************************************************
* public                                                   *
***********************************************************/

gears_viewAbout_t* gears_viewAbout_new(struct gears_overlay_s* overlay,
                                       a3d_widget_click_fn clickBack)
{
	assert(overlay);
	assert(clickBack);

	a3d_vec4f_t clear =
	{
		.r = 0.0f,
		.g = 0.0f,
		.b = 0.0f,
		.a = 0.0f
	};

	a3d_vec4f_t white =
	{
		.r = 1.0f,
		.g = 1.0f,
		.b = 1.0f,
		.a = 1.0f
	};

	a3d_vec4f_t black =
	{
		.r = 0.0f,
		.g = 0.0f,
		.b = 0.0f,
		.a = 1.0f
	};

	a3d_vec4f_t dkgray =
	{
		.r = 0.4f,
		.g = 0.4f,
		.b = 0.4f,
		.a = 1.0f
	};

	a3d_vec4f_t ltgray =
	{
		.r = 0.95f,
		.g = 0.95f,
		.b = 0.95f,
		.a = 1.0f
	};

	a3d_vec4f_t blue =
	{
		.r = 0.04f,
		.g = 0.54f,
		.b = 0.89f,
		.a = 1.0f
	};

	a3d_listbox_t* listbox;
	listbox = a3d_listbox_new(overlay->screen,
	                          0,
	                          A3D_LISTBOX_ORIENTATION_VERTICAL,
	                          A3D_WIDGET_ANCHOR_TL,
	                          A3D_WIDGET_WRAP_STRETCH_PARENT,
	                          A3D_WIDGET_WRAP_SHRINK,
	                          A3D_WIDGET_STRETCH_NA,
	                          1.0f,
	                          A3D_WIDGET_BORDER_LARGE,
	                          A3D_WIDGET_LINE_NONE,
	                          &clear, &clear,
	                          NULL,
	                          NULL);
	if(listbox == NULL)
	{
		return NULL;
	}

	a3d_widget_scrollbar((a3d_widget_t*) listbox,
	                     &ltgray, &dkgray);

	gears_viewAbout_t* self;
	self = (gears_viewAbout_t*)
	       a3d_viewbox_new(overlay->screen,
	                       sizeof(gears_viewAbout_t),
	                       A3D_WIDGET_ANCHOR_TL,
	                       A3D_WIDGET_WRAP_SHRINK,
	                       A3D_WIDGET_WRAP_STRETCH_PARENT,
	                       A3D_WIDGET_STRETCH_NA,
	                       1.0f,
	                       A3D_WIDGET_BORDER_NONE,
	                       A3D_WIDGET_LINE_MEDIUM,
	                       &white, &blue, &clear,
	                       A3D_WIDGET_ANCHOR_TL,
	                       A3D_WIDGET_BORDER_MEDIUM,
	                       A3D_WIDGET_LINE_NONE,
	                       A3D_TEXT_STYLE_MEDIUM,
	                       &clear, &clear, &white,
	                       16,
	                       "$ic_arrow_back_white_24dp.texz",
	                       clickBack,
	                       (void*) overlay,
	                       (a3d_widget_t*) listbox,
	                       NULL);
	if(self == NULL)
	{
		goto fail_view;
	}
	self->listbox = listbox;

	a3d_text_t* text_intro;
	text_intro = a3d_text_new(overlay->screen,
	                          0,
	                          A3D_WIDGET_ANCHOR_TL,
	                          A3D_WIDGET_BORDER_MEDIUM,
	                          A3D_WIDGET_LINE_NONE,
	                          A3D_TEXT_STYLE_MEDIUM,
	                          &clear, &clear, &black,
	                          80,
	                          NULL,
	                          NULL);
	if(text_intro == NULL)
	{
		goto fail_text_intro;
	}
	a3d_text_font(text_intro, A3D_SCREEN_FONT_BOLD);
	self->text_intro = text_intro;

	a3d_text_t* text_icons;
	text_icons = a3d_text_new(overlay->screen,
	                          0,
	                          A3D_WIDGET_ANCHOR_TL,
	                          A3D_WIDGET_BORDER_MEDIUM,
	                          A3D_WIDGET_LINE_NONE,
	                          A3D_TEXT_STYLE_MEDIUM,
	                          &clear, &clear, &black,
	                          80,
	                          NULL,
	                          NULL);
	if(text_icons == NULL)
	{
		goto fail_text_icons;
	}
	a3d_text_font(text_icons, A3D_SCREEN_FONT_BOLD);
	self->text_icons = text_icons;

	a3d_text_t* text_barlow;
	text_barlow = a3d_text_new(overlay->screen,
	                           0,
	                           A3D_WIDGET_ANCHOR_TL,
	                           A3D_WIDGET_BORDER_MEDIUM,
	                           A3D_WIDGET_LINE_NONE,
	                           A3D_TEXT_STYLE_MEDIUM,
	                           &clear, &clear, &black,
	                           80,
	                           NULL,
	                           NULL);
	if(text_barlow == NULL)
	{
		goto fail_text_barlow;
	}
	a3d_text_font(text_barlow, A3D_SCREEN_FONT_BOLD);
	self->text_barlow = text_barlow;

	a3d_text_t* text_license;
	text_license = a3d_text_new(overlay->screen,
	                            0,
	                            A3D_WIDGET_ANCHOR_TL,
	                            A3D_WIDGET_BORDER_MEDIUM,
	                            A3D_WIDGET_LINE_NONE,
	                            A3D_TEXT_STYLE_MEDIUM,
	                            &clear, &clear, &black,
	                            80,
	                            NULL,
	                            NULL);
	if(text_license == NULL)
	{
		goto fail_text_license;
	}
	a3d_text_font(text_license, A3D_SCREEN_FONT_BOLD);
	self->text_license = text_license;

	a3d_textbox_t* textbox_intro;
	textbox_intro = a3d_textbox_new(overlay->screen,
	                                0,
	                                A3D_LISTBOX_ORIENTATION_VERTICAL,
	                                A3D_WIDGET_ANCHOR_TL,
	                                A3D_WIDGET_WRAP_STRETCH_PARENT,
	                                A3D_WIDGET_WRAP_SHRINK,
	                                A3D_WIDGET_STRETCH_NA,
	                                1.0f,
	                                A3D_WIDGET_BORDER_NONE,
	                                A3D_WIDGET_LINE_NONE,
	                                &clear, &clear,
	                                A3D_WIDGET_ANCHOR_TL,
	                                A3D_TEXT_WRAP_SHRINK,
	                                A3D_WIDGET_BORDER_MEDIUM,
	                                A3D_WIDGET_LINE_NONE,
	                                A3D_TEXT_STYLE_MEDIUM,
	                                &clear, &clear, &black,
	                                80);
	if(textbox_intro == NULL)
	{
		goto fail_textbox_intro;
	}
	self->textbox_intro = textbox_intro;

	a3d_textbox_t* textbox_icons;
	textbox_icons = a3d_textbox_new(overlay->screen,
	                                0,
	                                A3D_LISTBOX_ORIENTATION_VERTICAL,
	                                A3D_WIDGET_ANCHOR_TL,
	                                A3D_WIDGET_WRAP_STRETCH_PARENT,
	                                A3D_WIDGET_WRAP_SHRINK,
	                                A3D_WIDGET_STRETCH_NA,
	                                1.0f,
	                                A3D_WIDGET_BORDER_NONE,
	                                A3D_WIDGET_LINE_NONE,
	                                &clear, &clear,
	                                A3D_WIDGET_ANCHOR_TL,
	                                A3D_TEXT_WRAP_SHRINK,
	                                A3D_WIDGET_BORDER_MEDIUM,
	                                A3D_WIDGET_LINE_NONE,
	                                A3D_TEXT_STYLE_MEDIUM,
	                                &clear, &clear, &black,
	                                80);
	if(textbox_icons == NULL)
	{
		goto fail_textbox_icons;
	}
	self->textbox_icons = textbox_icons;

	a3d_textbox_t* textbox_barlow;
	textbox_barlow = a3d_textbox_new(overlay->screen,
	                                 0,
	                                 A3D_LISTBOX_ORIENTATION_VERTICAL,
	                                 A3D_WIDGET_ANCHOR_TL,
	                                 A3D_WIDGET_WRAP_STRETCH_PARENT,
	                                 A3D_WIDGET_WRAP_SHRINK,
	                                 A3D_WIDGET_STRETCH_NA,
	                                 1.0f,
	                                 A3D_WIDGET_BORDER_NONE,
	                                 A3D_WIDGET_LINE_NONE,
	                                 &clear, &clear,
	                                 A3D_WIDGET_ANCHOR_TL,
	                                 A3D_TEXT_WRAP_SHRINK,
	                                 A3D_WIDGET_BORDER_MEDIUM,
	                                 A3D_WIDGET_LINE_NONE,
	                                 A3D_TEXT_STYLE_MEDIUM,
	                                 &clear, &clear, &black,
	                                 80);
	if(textbox_barlow == NULL)
	{
		goto fail_textbox_barlow;
	}
	self->textbox_barlow = textbox_barlow;

	a3d_textbox_t* textbox_license;
	textbox_license = a3d_textbox_new(overlay->screen,
	                                  0,
	                                  A3D_LISTBOX_ORIENTATION_VERTICAL,
	                                  A3D_WIDGET_ANCHOR_TL,
	                                  A3D_WIDGET_WRAP_STRETCH_PARENT,
	                                  A3D_WIDGET_WRAP_SHRINK,
	                                  A3D_WIDGET_STRETCH_NA,
	                                  1.0f,
	                                  A3D_WIDGET_BORDER_NONE,
	                                  A3D_WIDGET_LINE_NONE,
	                                  &clear, &clear,
	                                  A3D_WIDGET_ANCHOR_TL,
	                                  A3D_TEXT_WRAP_SHRINK,
	                                  A3D_WIDGET_BORDER_MEDIUM,
	                                  A3D_WIDGET_LINE_NONE,
	                                  A3D_TEXT_STYLE_MEDIUM,
	                                  &clear, &clear, &black,
	                                  80);
	if(textbox_license == NULL)
	{
		goto fail_textbox_license;
	}
	self->textbox_license = textbox_license;

	a3d_textbox_t* linkbox_github;
	linkbox_github = a3d_textbox_new(overlay->screen,
	                                 0,
	                                 A3D_LISTBOX_ORIENTATION_VERTICAL,
	                                 A3D_WIDGET_ANCHOR_TL,
	                                 A3D_WIDGET_WRAP_STRETCH_PARENT,
	                                 A3D_WIDGET_WRAP_SHRINK,
	                                 A3D_WIDGET_STRETCH_NA,
	                                 1.0f,
	                                 A3D_WIDGET_BORDER_NONE,
	                                 A3D_WIDGET_LINE_NONE,
	                                 &clear, &clear,
	                                 A3D_WIDGET_ANCHOR_TL,
	                                 A3D_TEXT_WRAP_SHRINK,
	                                 A3D_WIDGET_BORDER_MEDIUM,
	                                 A3D_WIDGET_LINE_NONE,
	                                 A3D_TEXT_STYLE_MEDIUM,
	                                 &clear, &clear, &blue,
	                                 80);
	if(linkbox_github == NULL)
	{
		goto fail_linkbox_github;
	}
	self->linkbox_github = linkbox_github;
	a3d_widget_t* widget_github = (a3d_widget_t*) linkbox_github;
	a3d_widget_priv(widget_github, (void*) overlay);
	a3d_textbox_clickFn(linkbox_github, clickGithub);

	a3d_textbox_t* linkbox_icons;
	linkbox_icons = a3d_textbox_new(overlay->screen,
	                                0,
	                                A3D_LISTBOX_ORIENTATION_VERTICAL,
	                                A3D_WIDGET_ANCHOR_TL,
	                                A3D_WIDGET_WRAP_STRETCH_PARENT,
	                                A3D_WIDGET_WRAP_SHRINK,
	                                A3D_WIDGET_STRETCH_NA,
	                                1.0f,
	                                A3D_WIDGET_BORDER_NONE,
	                                A3D_WIDGET_LINE_NONE,
	                                &clear, &clear,
	                                A3D_WIDGET_ANCHOR_TL,
	                                A3D_TEXT_WRAP_SHRINK,
	                                A3D_WIDGET_BORDER_MEDIUM,
	                                A3D_WIDGET_LINE_NONE,
	                                A3D_TEXT_STYLE_MEDIUM,
	                                &clear, &clear, &blue,
	                                80);
	if(linkbox_icons == NULL)
	{
		goto fail_linkbox_icons;
	}
	self->linkbox_icons = linkbox_icons;
	a3d_widget_t* widget_icons = (a3d_widget_t*) linkbox_icons;
	a3d_widget_priv(widget_icons, (void*) overlay);
	a3d_textbox_clickFn(linkbox_icons, clickIcons);

	a3d_textbox_t* linkbox_barlow;
	linkbox_barlow = a3d_textbox_new(overlay->screen,
	                                 0,
	                                 A3D_LISTBOX_ORIENTATION_VERTICAL,
	                                 A3D_WIDGET_ANCHOR_TL,
	                                 A3D_WIDGET_WRAP_STRETCH_PARENT,
	                                 A3D_WIDGET_WRAP_SHRINK,
	                                 A3D_WIDGET_STRETCH_NA,
	                                 1.0f,
	                                 A3D_WIDGET_BORDER_NONE,
	                                 A3D_WIDGET_LINE_NONE,
	                                 &clear, &clear,
	                                 A3D_WIDGET_ANCHOR_TL,
	                                 A3D_TEXT_WRAP_SHRINK,
	                                 A3D_WIDGET_BORDER_MEDIUM,
	                                 A3D_WIDGET_LINE_NONE,
	                                 A3D_TEXT_STYLE_MEDIUM,
	                                 &clear, &clear, &blue,
	                                 80);
	if(linkbox_barlow == NULL)
	{
		goto fail_linkbox_barlow;
	}
	self->linkbox_barlow = linkbox_barlow;
	a3d_widget_t* widget_barlow = (a3d_widget_t*) linkbox_barlow;
	a3d_widget_priv(widget_barlow, (void*) overlay);
	a3d_textbox_clickFn(linkbox_barlow, clickBarlow);

	a3d_viewbox_textPrintf(&self->viewbox, "%s", "About");

	a3d_text_printf(text_intro, "%s", "Introduction");
	a3d_textbox_printf(textbox_intro, "%s", "Gears for Android is a heavily modified port of the famous \"gears\" demo");
	a3d_textbox_printf(textbox_intro, "%s", "to Android.");
	a3d_textbox_printf(textbox_intro, "%s", "");
	a3d_textbox_printf(textbox_intro, "%s", "The Gears demo is an open source project intended to help developers learn");
	a3d_textbox_printf(textbox_intro, "%s", "how to create OpenGL ES programs on Android. The Gears demo was originally");
	a3d_textbox_printf(textbox_intro, "%s", "written by Brian Paul as part of the Mesa3D project. My implementation includes");
	a3d_textbox_printf(textbox_intro, "%s", "variations for Java/OpenGL ES 1.x, Java/C/OpenGL ES 1.x and Java/C/OpenGL ES 2.0.");
	a3d_textbox_printf(textbox_intro, "%s", "I have also added several features not found in the original implementation");
	a3d_textbox_printf(textbox_intro, "%s", "including touch screen support, VBOs and an on-screen FPS counter.");
	a3d_textbox_printf(textbox_intro, "%s", "");
	a3d_textbox_printf(textbox_intro, "%s", "The FPS (frames-per-second) counter is often used as a benchmark metric for");
	a3d_textbox_printf(textbox_intro, "%s", "graphics programs. On Android the frame rate is limited by v-sync (typically");
	a3d_textbox_printf(textbox_intro, "%s", "60 FPS) which is the fastest rate that a display can refresh the screen. Since");
	a3d_textbox_printf(textbox_intro, "%s", "Gears is capable of rendering much faster than v-sync on most devices it provides");
	a3d_textbox_printf(textbox_intro, "%s", "limited benchmarking value.");
	a3d_textbox_printf(textbox_intro, "%s", "");
	a3d_textbox_printf(textbox_intro, "%s", "Send questions or comments to Jeff Boody at jeffboody@gmail.com.");
	a3d_textbox_printf(textbox_intro, "%s", "");
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

	a3d_list_t* list = listbox->list;
	a3d_list_enqueue(list, (const void*) text_intro);
	a3d_list_enqueue(list, (const void*) textbox_intro);
	a3d_list_enqueue(list, (const void*) linkbox_github);
	a3d_list_enqueue(list, (const void*) text_icons);
	a3d_list_enqueue(list, (const void*) textbox_icons);
	a3d_list_enqueue(list, (const void*) linkbox_icons);
	a3d_list_enqueue(list, (const void*) text_barlow);
	a3d_list_enqueue(list, (const void*) textbox_barlow);
	a3d_list_enqueue(list, (const void*) linkbox_barlow);
	a3d_list_enqueue(list, (const void*) text_license);
	a3d_list_enqueue(list, (const void*) textbox_license);

	// success
	return self;

	// failure
	fail_linkbox_barlow:
		a3d_textbox_delete(&linkbox_icons);
	fail_linkbox_icons:
		a3d_textbox_delete(&linkbox_github);
	fail_linkbox_github:
		a3d_textbox_delete(&textbox_license);
	fail_textbox_license:
		a3d_textbox_delete(&textbox_barlow);
	fail_textbox_barlow:
		a3d_textbox_delete(&textbox_icons);
	fail_textbox_icons:
		a3d_textbox_delete(&textbox_intro);
	fail_textbox_intro:
		a3d_text_delete(&text_license);
	fail_text_license:
		a3d_text_delete(&text_barlow);
	fail_text_barlow:
		a3d_text_delete(&text_icons);
	fail_text_icons:
		a3d_text_delete(&text_intro);
	fail_text_intro:
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
		// empty listbox
		a3d_listitem_t* iter;
		iter = a3d_list_head(self->listbox->list);
		while(iter)
		{
			a3d_list_remove(self->listbox->list, &iter);
		}

		a3d_textbox_delete(&self->linkbox_barlow);
		a3d_textbox_delete(&self->linkbox_icons);
		a3d_textbox_delete(&self->linkbox_github);
		a3d_textbox_delete(&self->textbox_license);
		a3d_textbox_delete(&self->textbox_barlow);
		a3d_textbox_delete(&self->textbox_icons);
		a3d_textbox_delete(&self->textbox_intro);
		a3d_text_delete(&self->text_license);
		a3d_text_delete(&self->text_barlow);
		a3d_text_delete(&self->text_icons);
		a3d_text_delete(&self->text_intro);
		a3d_listbox_delete(&self->listbox);
		a3d_viewbox_delete((a3d_viewbox_t**) _self);
	}
}
