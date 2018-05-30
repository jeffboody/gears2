/*
 * Copyright (c) 2018 Jeff Boody
 */

#ifndef gears_viewAbout_H
#define gears_viewAbout_H

#include "a3d/widget/a3d_viewbox.h"
#include "a3d/widget/a3d_listbox.h"
#include "a3d/widget/a3d_textbox.h"
#include "a3d/widget/a3d_text.h"

struct gears_overlay_s;

typedef struct
{
	a3d_viewbox_t  viewbox;
	a3d_listbox_t* listbox;

	// headings
	a3d_text_t* text_intro;
	a3d_text_t* text_icons;
	a3d_text_t* text_barlow;
	a3d_text_t* text_expat;
	a3d_text_t* text_license;

	// paragraphs
	a3d_textbox_t* textbox_intro;
	a3d_textbox_t* textbox_icons;
	a3d_textbox_t* textbox_barlow;
	a3d_textbox_t* textbox_expat;
	a3d_textbox_t* textbox_license;

	// links
	a3d_textbox_t* linkbox_github;
	a3d_textbox_t* linkbox_icons;
	a3d_textbox_t* linkbox_barlow;
	a3d_textbox_t* linkbox_expat;
} gears_viewAbout_t;

gears_viewAbout_t* gears_viewAbout_new(struct gears_overlay_s* overlay,
                                       a3d_widget_click_fn clickBack);
void               gears_viewAbout_delete(gears_viewAbout_t** _self);

#endif
