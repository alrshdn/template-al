
#include <stdio.h>
#include <stdlib.h>
// #include <string.h>
#include <X11/Xlib.h>
// #include <X11/Xft/Xft.h>

#include "minim-ui.h"


Mui *
mui_setup(Display *display, int screen, Window window, unsigned int screenwidth, unsigned int screenheight)
{
	Mui *mui = calloc(1, sizeof(Mui));

	mui->dpy = display;
	mui->scr = screen;
	mui->main_win = window;
	mui->sw = screenwidth;
	mui->sh = screenheight;

	/* graphical context and state setup */
	XGCValues gcv;
	gcv.fill_style = FillSolid;
	gcv.line_width = 44;
	gcv.line_style = LineSolid;
	gcv.cap_style = CapButt;
	gcv.join_style = JoinMiter;
	gcv.foreground = 0x00FF0000;
	gcv.background = 0x00eFecFF;
	unsigned long vm = GCLineWidth|GCLineStyle|GCCapStyle|GCJoinStyle|GCForeground|GCBackground|GCFillStyle;
		
	mui->gc = XCreateGC(mui->dpy, mui->main_win, vm, &gcv);

	return mui;
}

void
mui_rectangle(Mui *mui, int xpos, int ypos, unsigned int width, unsigned int height, int isfilled, int isinverted)
{
	/*
	if (!mui || !mui->scheme)
		return;
	*/
	XSetForeground(mui->dpy, mui->gc, WhitePixel(mui->dpy, mui->scr));
	if (isfilled)
		XFillRectangle(mui->dpy, mui->main_win, mui->gc, xpos, ypos, width, height);
	else
		XDrawRectangle(mui->dpy, mui->main_win, mui->gc, xpos, ypos, width - 1, height - 1);
}

void
mui_cleanup(Mui *mui)
{
	XFreeGC(mui->dpy, mui->gc);
	free(mui);
}
