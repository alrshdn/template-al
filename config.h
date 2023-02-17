//static char *colors[][ClrCount];


static const Keyevent keyevents[] = {
	/* key				modifier			function			argument */
	{ XK_p,				ShiftMask,			draw_rect,			{.geo = {100, 120, 640, 480} } },
	{ XK_Return,			Mod1Mask|ShiftMask, 		quit,				{0} }
	};


