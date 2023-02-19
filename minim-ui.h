typedef XEvent MuiEvent;
typedef XButtonPressedEvent MuiButtonPressedEvent;
typedef XKeyPressedEvent MuiKeyPressedEvent;
typedef XKeyReleasedEvent MuiKeyReleasedEvent;
typedef XExposeEvent MuiExposedEvent;
typedef XftColor Clr;

enum {
	WMNormalHints,
	WMAtomCount
}; /* Window Manager atoms */

typedef struct {
	Display *dpy;
	int scr;
	int w, h;
	Window main_win;
	GC gc;
	Clr **clrmodes;
} Mui;

static Atom wmatom[WMAtomCount];

/* initialization and setup */
Mui *mui_init(const char *displayname);
void mui_setup(Mui *mui, const int xposition, const int yposition, const unsigned int windowwidth, const unsigned int windowheight, const unsigned long backgroundcolor, const unsigned long foregroundcolor, const char *appname);

/* colors */
void mui_create_clr(Mui *mui, const char *clrname, Clr *clr);
Clr *mui_create_clrmode(Mui *mui, char *clrnames[], size_t clrcount);

/* actions */
void mui_show(Mui *mui);
void mui_setwindow_newsize(Mui *mui, const int newwidth, const int newheight);
void mui_setwindow_minmaxsize(Mui *mui, const int minwidth, const int minheight, const int maxwidth, const int maxheight);
void mui_setwindow_backgroundcolor(Mui *mui, const unsigned long color);
void mui_set_clrmode(Mui *mui, Clr *clrmode);

/* queries  */
void mui_getwindow_position(Mui *mui, int *xposition, int *yposition);
void mui_getwindow_size(Mui *mui, int *width, int *height);

/* drawing */
void mui_draw_rectangle(Mui *mui, const int xposition, const int yposition, const unsigned int width, const unsigned int height, const int isfilled, const int isinverted);

/* cleanup and finalization */
void mui_cleanup(Mui *mui);
void mui_fin(Mui *mui, int andcleanup);
