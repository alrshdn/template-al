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
#include <string.h>

#include <X11/Xlib.h>
#include <X11/Xatom.h>


/* macros */
#define XPOS 		0
#define YPOS 		0
#define WIDTH 		720
#define HEIGHT 		1080
#define BORDERSIZE 	0
#define BORDERCOLOR 	0x00000000
#define BGCOLOR 	0x001C1C1C
#define APPNAME		"temp-al"

/* enums */
enum {
	WMNormalHints,
	WMLast
}; /* Window Manager Atoms */


/* structs */



/* function declarations */
static void buttonpress(XEvent *e);
static void cleanup(void);
static void expose(XEvent *e);
static void keypress(XEvent *e);
static void run(void);
static void setup(void);


/* variables */
static int isrunning;

static Display *dpy;

static int scr; /* screen number */
static int sw, sh; /* screen width and height*/

static Window root, temp_al;

static void (*handler[LASTEvent]) (XEvent *) = {
	[ButtonPress] 		= buttonpress,
        [Expose] 		= expose,
        [KeyPress] 		= keypress,
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


/* function implementations */
void
buttonpress(XEvent *e)
{
	XButtonPressedEvent *ev = &e->xbutton;
	printf("(%i, %i)\n", ev->x, ev->y);
}

void
cleanup(void)
{
	XUnmapWindow(dpy, root);
	XDestroyWindow(dpy, root);
}

void
expose(XEvent *e)
{
	XExposeEvent *ev = &e->xexpose;
	printf("Exposed\n");
}

void
keypress(XEvent *e)
{
	XKeyPressedEvent *ev = &e->xkey;
	printf("Keycode %u: %i\n", ev->keycode, ev->type);
}

void
quit(void)
{
	isrunning = 0;
}

void
run(void)
{	
	XEvent ev;
	
	isrunning = 1;
	
	XMapWindow(dpy, temp_al);
	XSync(dpy, 0);
	while (isrunning && !XNextEvent(dpy, &ev)) { 
		if (handler[ev.type]) {
			handler[ev.type](&ev);
		}
	}
}

void
setup(void)
{	
	XSetWindowAttributes wa;
		
	/* screen setup */
	scr = XDefaultScreen(dpy);
	sw = XDisplayWidth(dpy, scr);
	sh = XDisplayHeight(dpy, scr);
	root = XRootWindow(dpy, scr);

	/* atoms setup */
	wmatom[WMNormalHints] = XInternAtom(dpy, "WM_NORMAL_HINTS", False);	

	/* window setup */
	temp_al = XCreateSimpleWindow(
			dpy,
			root,
			XPOS, YPOS,
			WIDTH, HEIGHT,
			BORDERSIZE,
			BORDERCOLOR,
			BGCOLOR);
	wa.event_mask = KeyPressMask|ButtonPressMask|ExposureMask;
	XChangeWindowAttributes(dpy, temp_al, CWEventMask, &wa);

	XStoreName(dpy, temp_al, APPNAME);
}

int
main()
{
	dpy = XOpenDisplay(NULL);
	if (!dpy) {
		printf("temp-al: cannot open display!\n");
		return 1;
	}

	setup();
	run();
			
	cleanup();
	XCloseDisplay(dpy);
	return 0;
}
