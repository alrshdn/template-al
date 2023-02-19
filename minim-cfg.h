static char unselbgmenucolor[]				= "#222222";
static char unselfgmenucolor[]				= "#bbbbbb";
static char unselbordermenucolor[]			= "#444444";

static char selbgmenucolor[]				= "#005577";
static char selfgmenucolor[]				= "#eeeeee";
static char selbordermenucolor[]			= "#005577";


static char *colors[][ClrIdCount] = {
	[ClrModeMenuUnsel] 	= {unselbgmenucolor, unselfgmenucolor, unselbordermenucolor},
	[ClrModeMenuSel] 	= {selbgmenucolor, selfgmenucolor, selbordermenucolor}
};


static const Keyevent keyevents[] = {
	/* key				modifier			callback			argument */
	{ XK_p,				ShiftMask,			drawmenu,			{0} },
	{ XK_q,				ControlMask,	 		quit,				{0} }
};
