/* Controls
 *	Conversations menu:
 * 		l: move to next page
 * 		h: move to previous page
 * 		k: scroll up
 * 		j: scroll down
 *	Chat:
 *		Vim bindings
 *	
 */

#include <stdlib.h>
#include <stdio.h>
#include <stdarg.h>
#include <string.h>

#include <X11/Xlib.h>
#include <X11/Xatom.h>
#include <X11/keysym.h>
#include <X11/Xft/Xft.h>
#include <X11/Xutil.h>

#include "minim-ui.h"


/* macros */
#define WINDOW_XPOS		100
#define WINDOW_YPOS		100
#define WINDOW_WIDTH 		2000
#define WINDOW_HEIGHT 		1200
#define WINDOW_MIN_WIDTH	0
#define WINDOW_MIN_HEIGHT	0
#define WINDOW_MAX_WIDTH	0
#define WINDOW_MAX_HEIGHT	0
#define BORDER_WIDTH 		200
#define BORDER_COLOR 		0x000000
#define BACKGROUND_COLOR	0x458588
#define FOREGROUND_COLOR	0xeadbb2
#define APP_NAME		"temp-al"

#define LENGTH(X)		(sizeof X / sizeof X[0])

#define DRAW_BACKGROUND(WIDTH, HEIGHT)	mui_draw_rectangle(mui, 0, 0, WIDTH, HEIGHT, 1, 1, ClrModeMenuSel)


/* enums */
enum {
	ClrModeMenuUnsel,
	ClrModeMenuSel,
}; /* color modes */


/* unions */
typedef union {
	int i;
	unsigned int ui;
	float f;
	const int geo[4];
	const void *v;
} Arg;


/* structs */
typedef struct {
	KeySym keysym;
	unsigned int mod;
	void (*callback)(const Arg *);
	const Arg arg;
} Keyevent;


/* function declarations */
static void buttonpress(MuiEvent *e);
static void cleanup(void);
static void configurenotify(MuiEvent *e);
static void drawmenu(const Arg *arg);
static void expose(MuiEvent *e);
static void grapkeys(void);
static void keypress(MuiEvent *e);
static void keyrelease(MuiEvent *e);
static void quit(const Arg* arg);
static void resizerequest(MuiEvent *e);
static void run(void);
static void setup(void);


/* variables */
static int isrunning;

static Mui *mui; /* minimal user interface */

static void (*eventhandler[LASTEvent]) (MuiEvent *) = {
	[ButtonPress] 		= buttonpress,
        [Expose] 		= expose,
        [KeyPress] 		= keypress,
        [KeyRelease] 		= keyrelease,
        [ConfigureNotify] 	= configurenotify,
	/*
        [ResizeRequest] 	= resizerequest,
	[CreateNotify]		= createnotify,
        [ClientMessage] 	= clientmessage,
        [ConfigureRequest] 	= configurerequest,
        [DestroyNotify] 	= destroynotify,
        [EnterNotify] 		= enternotify,
        [FocusIn] 		= focusin,
        [MappingNotify] 	= mappingnotify,
        [MapRequest] 		= maprequest,
        [MotionNotify] 		= motionnotify,
        [PropertyNotify] 	= propertynotify,
	[UnmapNotify] 		= unmapnotify
	*/
};


#include "minim-cfg.h"


/* function implementations */
void
buttonpress(MuiEvent *e)
{
	MuiButtonPressedEvent *ev = &e->xbutton;
}

void
cleanup(void)
{
	printf("LOG: started cleanup\n");
	printf("LOG: finished cleanup\n");
}

void
drawmenu(const Arg *arg)
{
	mui_draw_rectangle(mui, 400, 520, mui->w/5, mui->h/5, True, True, ClrModeMenuSel);
	mui_draw_rectangle(mui, 100, 120, mui->w/5, mui->h/5, True, True, ClrModeMenuUnsel);

	/* background */
	//int w, h;
	//mui_getwindow_size(mui, &w, &h);
	//DRAW_BACKGROUND(w, h);
}

void
expose(MuiEvent *e)
{
	MuiExposedEvent *ev = &e->xexpose;	
	if (ev->count == 0) {
		drawmenu(0);
	}
}

void
grabkeys(void)
{
	size_t i, j;
	unsigned int modifiers[] = { 0, LockMask };
	KeyCode code;

	XUngrabKey(mui->dpy, AnyKey, AnyModifier, mui->main_win);
	for (i = 0; i < LENGTH(keyevents); i++)
		if ((code = XKeysymToKeycode(mui->dpy, keyevents[i].keysym)))
			for (j = 0; j < LENGTH(modifiers); j++)
				XGrabKey(mui->dpy, code, keyevents[i].mod | modifiers[j], mui->main_win,
					True, GrabModeAsync, GrabModeAsync);
}



void
keypress(MuiEvent *e)
{
	unsigned int i;
	MuiKeyPressedEvent *ev = &e->xkey;	
	for (i = 0; i < LENGTH(keyevents); i++)
		if (ev->keycode == XKeysymToKeycode(mui->dpy, keyevents[i].keysym)
				&& ev->state == keyevents[i].mod
				&& keyevents[i].callback)
			keyevents[i].callback(&(keyevents[i].arg));
}

void
keyrelease(MuiEvent *e)
{
	MuiKeyReleasedEvent *ev = &e->xkey;
}

void
quit(const Arg *arg)
{
	isrunning = 0;
}

void
configurenotify(MuiEvent *e)
{
	XConfigureEvent *ev = &e->xconfigure;
	if (ev->width != mui->w || ev->height != mui->h) {
		printf("RESIZE EVENT: (%i, %i)\n", ev->width, ev->height);
		mui_setwindow_newsize(mui, ev->width, ev->height);
	}
}

void
run(void)
{
	MuiEvent ev;
	
	mui_show(mui);
	isrunning = 1;

	while (isrunning && !XNextEvent(mui->dpy, &ev)) {
		if (eventhandler[ev.type]) {
			eventhandler[ev.type](&ev);
		}
	}
}

void
setup(void)
{
	mui_setwindow_minmaxsize(mui, 1080, 720, 1440, 1080);

	/* setup color scheme */
	mui_create_colormodes(mui, colors, LENGTH(colors), ClrIdCount);
}

int
main(void)
{
	/* initialize */
	if (!(mui = mui_init(NULL))) {
		printf("temp-al: cannot open display!\n");
		return 1;
	}

	/* setup minimal user interface */
	mui_setup(mui, WINDOW_XPOS, WINDOW_YPOS, WINDOW_WIDTH, WINDOW_HEIGHT, BACKGROUND_COLOR, FOREGROUND_COLOR, APP_NAME);

	/* minim-al program */
	setup();
	run();
	cleanup();

	/* cleanup minimal user interface */
	mui_cleanup(mui);
	
	/* finalize */	
	mui_fin(mui, False);	
	return 0;
}
