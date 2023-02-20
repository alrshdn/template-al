typedef XEvent MuiEvent;
typedef XftColor MuiColor;
typedef XButtonPressedEvent MuiButtonPressedEvent;
typedef XKeyPressedEvent MuiKeyPressedEvent;
typedef XKeyReleasedEvent MuiKeyReleasedEvent;
typedef XExposeEvent MuiExposedEvent;

enum {
	WMNormalHints,
	WMAtomCount
}; /* Window Manager atoms */
enum {ClrIdBackground, ClrIdForeground, ClrIdBorder, ClrIdCount}; /* colors indices for accessing different colormodes' elements */

typedef struct {
	Display *dpy;
	int scr;
	int w, h;
	Window main_win;
	GC gc;
	MuiColor **colormodes;
	size_t colormodecount;
} Mui;

static Atom wmatom[WMAtomCount];

/* initialization and setup */
Mui *mui_init(const char *displayname);
void mui_setup(Mui *mui, const int xposition, const int yposition, const unsigned int windowwidth, const unsigned int windowheight, const unsigned long backgroundcolor, const unsigned long foregroundcolor, const char *appname);

/* colors */
void mui_create_color(Mui *mui, char *colorname, MuiColor *color);
MuiColor *mui_create_colormode(Mui *mui, char *colornames[], const size_t colorcount);
void mui_create_colormodes(Mui *mui, char *colors[][3], const size_t colormodecount, const size_t colorcount);

/* actions */
void mui_show(Mui *mui);
void mui_setwindow_newsize(Mui *mui, const int newwidth, const int newheight);
void mui_setwindow_minmaxsize(Mui *mui, const int minwidth, const int minheight, const int maxwidth, const int maxheight);
void mui_setwindow_backgroundcolor(Mui *mui, const unsigned long color);

/* queries  */
void mui_getwindow_position(Mui *mui, int *xposition, int *yposition);
void mui_getwindow_size(Mui *mui, int *width, int *height);

/* drawing */
void mui_draw_rectangle(Mui *mui, const int xposition, const int yposition, const unsigned int width, const unsigned int height, const int isfilled, const int isinverted, const int colormode);

/* cleanup and finalization */
void mui_cleanup(Mui *mui);
void mui_fin(Mui *mui, int andcleanup);
