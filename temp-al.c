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

#include "minim-ui.h"


/* macros */
#define XPOS 		100
#define YPOS 		100
#define WIDTH 		2000
#define HEIGHT 		1200
#define BORDERWIDTH 	200
#define BORDERCOLOR 	0xFF000000
#define BGCOLOR 	0x00458588
#define APPNAME		"temp-al"

#define LENGTH(X)               (sizeof X / sizeof X[0])

/* enums */
enum {
	WMNormalHints,
	WMLast
}; /* Window Manager Atoms */

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
static void buttonpress(XEvent *e);
static void cleanup(void);
static void draw_rect(const Arg *arg);
static void expose(XEvent *e);
static void grapkeys(void);
static void keypress(XEvent *e);
static void keyrelease(XEvent *e);
static void quit(const Arg* arg);
static void run(void);
static void setup(void);


/* variables */
static int isrunning;

static Display *dpy;

static int scr; /* screen number */
static int sw, sh; /* screen width and height*/

static Window root, temp_al;

static Mui *mui; /* minimal user interface */

static void (*eventhandler[LASTEvent]) (XEvent *) = {
	[ButtonPress] 		= buttonpress,
        [Expose] 		= expose,
        [KeyPress] 		= keypress,
        [KeyRelease] 		= keyrelease,
	/*
	[CreateNotify]		= createnotify,
        [ClientMessage] 	= clientmessage,
        [ConfigureRequest] 	= configurerequest,
        [ConfigureNotify] 	= configurenotify,
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

static Atom wmatom[WMLast];


#include "config.h"


/* function implementations */
void
buttonpress(XEvent *e)
{
	XButtonPressedEvent *ev = &e->xbutton;
	printf("EVENT: mouse coordinates (%i, %i)\n", ev->x, ev->y);
	//XDrawPoint(dpy, temp_al, gc, ev->x, ev->y);
}

void
cleanup(void)
{
	printf("LOG: started cleanup\n");
	XUnmapWindow(dpy, root);
	XDestroyWindow(dpy, root);
	mui_cleanup(mui);
	printf("LOG: finished cleanup\n");
}

void
expose(XEvent *e)
{
	XExposeEvent *ev = &e->xexpose;
	printf("EVENT: Exposed\n");

	//mui_rectangle(mui, 100, 120, 640, 480, 1, 1);
}

void
grabkeys(void)
{
	unsigned int i, j;
	unsigned int modifiers[] = { 0, LockMask };
	KeyCode code;

	XUngrabKey(dpy, AnyKey, AnyModifier, temp_al);
	for (i = 0; i < LENGTH(keyevents); i++)
		if ((code = XKeysymToKeycode(dpy, keyevents[i].keysym)))
			for (j = 0; j < LENGTH(modifiers); j++)
				XGrabKey(dpy, code, keyevents[i].mod | modifiers[j], temp_al,
					True, GrabModeAsync, GrabModeAsync);
}

void draw_rect(const Arg *arg)
{
	mui_rectangle(mui, arg->geo[0], arg->geo[1], arg->geo[2], arg->geo[3], 1, 1);
}


void
keypress(XEvent *e)
{
	unsigned int i;
	XKeyPressedEvent *ev = &e->xkey;
	
	//printf("p+shift: %i, %i, %i\n", ev->keycode == XKeysymToKeycode(dpy, keyevents[0].keysym), ev->state == keyevents[0].mod, keyevents[0].callback);
	//printf("return+mod3+shift: %i, %i, %i\n", ev->keycode == XKeysymToKeycode(dpy, keyevents[1].keysym),
	//						ev->state == keyevents[1].mod,
	//						keyevents[1].callback);

	for (i = 0; i < LENGTH(keyevents); i++)
		if (ev->keycode == XKeysymToKeycode(dpy, keyevents[i].keysym)
				&& ev->state == keyevents[i].mod
				&& keyevents[i].callback)
			keyevents[i].callback(&(keyevents[i].arg));
}

void
keyrelease(XEvent *e)
{
	XKeyReleasedEvent *ev = &e->xkey;
	//printf("EVENT: Keycode %i, type %i (key release). XK_q = %i\n", ev->keycode, ev->type, XK_q);
}

void
quit(const Arg *arg)
{
	isrunning = 0;
}

void
run(void)
{
	printf("LOG: run (from main)\n");
	XEvent ev;

	isrunning = 1;

	XSync(dpy, False);
	while (isrunning && !XNextEvent(dpy, &ev)) {
		if (eventhandler[ev.type]) {
			eventhandler[ev.type](&ev);
		}
	}
}

void
setup(void)
{
	printf("LOG: setup (from main)\n");
	XSetWindowAttributes wa;

	/* screen setup */
	scr = XDefaultScreen(dpy);
	sw = XDisplayWidth(dpy, scr);
	sh = XDisplayHeight(dpy, scr);

	/* fetching root window */
	root = DefaultRootWindow(dpy);

	/* main window and main window's attributes setup */
	wa.background_pixel = BGCOLOR;
	wa.border_pixel = BlackPixel(dpy, scr);
	wa.event_mask = KeyPressMask|KeyReleaseMask|ButtonPressMask|ExposureMask;
	unsigned long valuemask = CWEventMask|CWBorderPixel|CWBackPixel;
	temp_al = XCreateWindow(dpy, root,
			XPOS, YPOS,
			WIDTH, HEIGHT,
			BORDERWIDTH,
			DefaultDepth(dpy, scr),
			InputOutput, 
			DefaultVisual(dpy, scr),
			valuemask,
			&wa);

	XStoreName(dpy, temp_al, APPNAME);
	XMapWindow(dpy, temp_al);

	/* minimal user interface setup */
	mui = mui_setup(dpy, scr, temp_al, WIDTH, HEIGHT);

	/* atoms setup */
	wmatom[WMNormalHints] = XInternAtom(dpy, "WM_NORMAL_HINTS", False);
}

int
main(void)
{
	dpy = XOpenDisplay(NULL);
	if (!dpy) {
		printf("temp-al: cannot open display!\n");
		return 1;
	}

	setup();
	printf("LOG: main (from startup)\n");
	run();

	cleanup();
	XCloseDisplay(dpy);
	return 0;
}
