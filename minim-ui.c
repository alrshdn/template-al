#include <stdio.h>
#include <stdlib.h>
// #include <string.h>
#include <X11/Xlib.h>
#include <X11/Xft/Xft.h>

#include "minim-ui.h"

Mui *
mui_init(const char *displayname)
{
	Mui *mui = calloc(1, sizeof(Mui));

	if (!(mui->dpy = XOpenDisplay(displayname))) {
		printf("Mui: Error: Could not initialize connection to X server for opening display.\n");
		return NULL;
	}
	
	return mui;	
}

void
mui_setup(Mui *mui,
		const int xposition, const int yposition,
		const unsigned int windowwidth, const unsigned int windowheight,
		const unsigned long backgroundcolor, const unsigned long foregroundcolor,
		const char *appname)
{
	XSetWindowAttributes wa;
	XGCValues gcv;
	unsigned long wvm, gcvm;

	/* screen setup */
	mui->scr = XDefaultScreen(mui->dpy);

	/* main window and main window's attributes setup */
	mui->w = windowwidth;
	mui->h = windowheight;
	wa.background_pixel = backgroundcolor;
	wa.event_mask = KeyPressMask|KeyReleaseMask
		|ButtonPressMask
		|ExposureMask|StructureNotifyMask;
	wvm = CWBackPixel|CWEventMask;
	mui->main_win = XCreateWindow(mui->dpy,
			DefaultRootWindow(mui->dpy),
			xposition, yposition,
			windowwidth, windowheight,
			0,					/* border width */
			DefaultDepth(mui->dpy, mui->scr),
			InputOutput,
			DefaultVisual(mui->dpy, mui->scr),
			wvm,
			&wa);

	XStoreName(mui->dpy, mui->main_win, appname);

	/* graphical context and state setup */
	gcv.line_width = 44;
	gcv.line_style = LineSolid;
	gcv.cap_style = CapButt;
	gcv.join_style = JoinMiter;
	gcv.background = backgroundcolor;
	gcv.foreground = foregroundcolor;
	gcv.fill_style = FillSolid;
	gcvm = GCLineWidth|GCLineStyle
		|GCCapStyle|GCJoinStyle
		|GCForeground|GCBackground
		|GCFillStyle;
		
	mui->gc = XCreateGC(mui->dpy, mui->main_win, gcvm, &gcv);

	/* atoms setup */
	wmatom[WMNormalHints] = XInternAtom(mui->dpy, "WM_NORMAL_HINTS", False);
}

void
mui_create_color(Mui *mui,
		char *colorname,
		MuiColor *color)
{
	printf("MUI_CREATE_COLOR %s\n", colorname);
	XftColorAllocName(mui->dpy,
			DefaultVisual(mui->dpy, mui->scr),
			DefaultColormap(mui->dpy, mui->scr),
			colorname, color);
}

MuiColor *
mui_create_colormode(Mui *mui,
		char *colornames[],
		const size_t colorcount)
{
	size_t i;
	MuiColor *color = calloc(colorcount, sizeof(MuiColor));

	for (i = 0; i < colorcount; i++)
		mui_create_color(mui, colornames[i], color);

	return color;
}

void
mui_create_colormodes(Mui *mui,
		char *colors[][ClrIdCount],
		const size_t colormodecount, const size_t colorcount)
{
	size_t i;

	mui->colormodes = calloc(colormodecount, sizeof(MuiColor *));
	mui->colormodecount = colormodecount;

	for (i = 0; i < colormodecount; i++)
		mui->colormodes[i] = mui_create_colormode(mui, colors[i], colorcount);
}

void
mui_show(Mui *mui)
{
	if (XMapWindow(mui->dpy, mui->main_win))
		XSync(mui->dpy, False);
}

void
mui_setwindow_newsize(Mui *mui, const int newwidth, const int newheight)
{
	mui->w = newwidth;
	mui->h = newheight;
	XResizeWindow(mui->dpy, mui->main_win, mui->w, mui->h);
	XSync(mui->dpy, False);
}

void
mui_setwindow_minmaxsize(Mui *mui, const int minwidth, const int minheight, const int maxwidth, const int maxheight)
{
	XSizeHints *h = XAllocSizeHints();
	h->min_width = minwidth;
	h->min_height = minheight;
	h->max_width = maxwidth;
	h->max_height = maxheight;
	h->flags = PMinSize|PMaxSize;

	XSetWMSizeHints(mui->dpy, mui->main_win, h, wmatom[WMNormalHints]);
}

void
mui_setwindow_backgroundcolor(Mui *mui, const unsigned long color)
{
	XSetWindowBackground(mui->dpy, mui->main_win, color);
}

void mui_getwindow_position(Mui *mui, int *xposition, int *yposition)
{
	XWindowAttributes wa;
	XGetWindowAttributes(mui->dpy, mui->main_win, &wa);

	*xposition = wa.x;
	*yposition = wa.y;
}

void mui_getwindow_size(Mui *mui, int *width, int *height)
{
	/*
	XWindowAttributes wa;
	XGetWindowAttributes(mui->dpy, mui->main_win, &wa);

	*width = wa.width;
	*height = wa.height;
	*/
	*width = mui->w;
	*height = mui->h;
}

void
mui_draw_rectangle(Mui *mui,
		const int xposition, const int yposition,
		const unsigned int width, const unsigned int height,
		const int isfilled, const int isinverted,
		const int colormode)
{
	/*
	if (!mui || !mui->scheme)
		return;
	*/
	XSetForeground(mui->dpy, mui->gc, isinverted ? mui->colormodes[colormode][ClrIdBackground].pixel : mui->colormodes[colormode][ClrIdForeground].pixel);
	if (isfilled)
		XFillRectangle(mui->dpy, mui->main_win, mui->gc, xposition, yposition, width, height);
	else
		XDrawRectangle(mui->dpy, mui->main_win, mui->gc, xposition, yposition, width - 1, height - 1);
}

void
mui_cleanup(Mui *mui)
{
	size_t i;

	mui->scr = 0;
	mui->w = 0;
	mui->h = 0;

	XUnmapWindow(mui->dpy, mui->main_win);
	XDestroyWindow(mui->dpy, mui->main_win);

	XFreeGC(mui->dpy, mui->gc);
	
	for (i = 0; i < mui->colormodecount; i++)
		free(mui->colormodes[i]);
	free(mui->colormodes);
}

void
mui_fin(Mui *mui, int andcleanup)
{
	if (andcleanup)
		mui_cleanup(mui);

	XCloseDisplay(mui->dpy);
	free(mui);
}
