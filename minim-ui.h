
enum {ClrIdBackground, ClrIdForeground, ClrIdBorder, ClrIdCount}; /* colors indices */


//typedef XftColor Clr;

typedef struct {
	Display *dpy;
	int scr;
	Window main_win;
	unsigned int sw, sh;
	Drawable drw;
	GC gc;
	//Clr *clrs;
} Mui;


Mui *mui_setup(Display *display, int screen, Window window, unsigned int screenwidth, unsigned int screenheight);

void mui_rectangle(Mui *mui, int xpos, int ypos, unsigned int width, unsigned int height, int isfilled, int isinvert);

void mui_cleanup(Mui *mui);
