/* See LICENSE file for copyright and license details. */
#include <X11/XF86keysym.h>

/* appearance */
static const unsigned int borderpx  = 1;        /* border pixel of windows */
static const unsigned int snap      = 32;       /* snap pixel */
static const unsigned int gappih    = 20;
static const unsigned int gappiv    = 10;
static const unsigned int gappoh    = 10;
static const unsigned int gappov    = 30;
static       int smartgaps          = 0;
static const int FORCE_VSPLIT       = 1;        /* nrowgrid layout: force two clients to always split vertically */
#include "vanitygaps.c"

/* system tray */
static const unsigned int systraypinning = 0;   /* 0: sloppy systray follows selected monitor, >0: pin systray to monitor X */
static const unsigned int systrayspacing = 2;   /* systray spacing */
static const int systraypinningfailfirst = 1;   /* 1: if pinning fails, display systray on the first monitor, False: display systray on the last monitor*/
static const int systrayonleft = 0;             /* show system tray on left (shows on right if 0) */
static const int showsystray        = 1;        /* 0 means no systray */
/* end system tray */

static const int showbar            = 1;        /* 0 means no bar */
static const int topbar             = 1;        /* 0 means bottom bar */
static const char *fonts[]          = { "monospace:size=10" };
static const char dmenufont[]       = "monospace:size=10";
static const char col_gray1[]       = "#2b3339";  // everforest bg
static const char col_gray2[]       = "#374247";  // everforest border
static const char col_gray3[]       = "#d3c6aa";  // everforest fg
static const char col_gray4[]       = "#2b3339";  // dark text for selected
static const char col_orange[]      = "#a7c080";  // orange  


static const char *colors[][3]      = {
	/*               fg         bg         border   */
	[SchemeNorm] = { col_gray3, col_gray1, col_gray2 },
	[SchemeSel]  = { col_gray4, col_orange, col_orange },
};

/*autostart*/
static const char *const autostart[] = {
    "picom", NULL,
    "sh", "-c", "$HOME/.dotfiles/scripts/wallpaper-rotator.sh", NULL,
    "xrandr", "--output", "DP-1", "--mode", "2560x1440", "--rate", "144", NULL,
    "pasystray", NULL,
    "blueman-applet", NULL,
    "nm-applet", NULL,
    "dwmblocks", NULL,
    NULL /* terminate */
};

/* tagging */
static const char *tags[] = { "1", "2", "3", "4", "5", "6", "7", "8", "9" };

static const Rule rules[] = {
	/* xprop(1):
	 *	WM_CLASS(STRING) = instance, class
	 *	WM_NAME(STRING) = title
	 */
	/* class      instance    title       tags mask     isfloating   monitor */
	{ "Gimp",     NULL,       NULL,       0,            1,           -1 },
	{ "Firefox",  NULL,       NULL,       1 << 8,       0,           -1 },
};

/* layout(s) */
static const float mfact     = 0.55; /* factor of master area size [0.05..0.95] */
static const int nmaster     = 1;    /* number of clients in master area */
static const int resizehints = 1;    /* 1 means respect size hints in tiled resizals */
static const int lockfullscreen = 1; /* 1 will force focus on the fullscreen window */

static const Layout layouts[] = {
	/* symbol     arrange function */
	{ "[]=",      tile },    /* first entry is default */
	{ "><>",      NULL },    /* no layout function means floating behavior */
	{ "[M]",      monocle },
};

/* key definitions */
#define MODKEY Mod4Mask
#define TAGKEYS(KEY,TAG) \
	{ MODKEY,                       KEY,      view,           {.ui = 1 << TAG} }, \
	{ MODKEY|ControlMask,           KEY,      toggleview,     {.ui = 1 << TAG} }, \
	{ MODKEY|ShiftMask,             KEY,      tag,            {.ui = 1 << TAG} }, \
	{ MODKEY|ControlMask|ShiftMask, KEY,      toggletag,      {.ui = 1 << TAG} },

/* helper for spawning shell commands in the pre dwm-5.0 fashion */
#define SHCMD(cmd) { .v = (const char*[]){ "/bin/sh", "-c", cmd, NULL } }

/* commands */
static char dmenumon[2] = "0"; /* component of dmenucmd, manipulated in spawn() */
static const char *dmenucmd[] = { "dmenu_run", "-m", dmenumon, "-fn", dmenufont, "-nb", col_gray1, "-nf", col_gray3, "-sb", col_orange, "-sf", col_gray4, NULL };
static const char *termcmd[]  = { "kitty", NULL };
static const char *filemanagercmd[] = { "nautilus", NULL };
static const char *browsercmd[] = { "firefox", NULL };
static const char *spotifycmd[] = { "kitty", "-e", "spotify_player", NULL };
static const char *steamcmd[] = { "flatpak", "run", "com.valvesoftware.Steam", NULL };
static const char *discordcmd[] = { "flatpak", "run", "com.discordapp.Discord", NULL };
static const char *upvol[] = { 
    "sh", "-c", 
    "wpctl set-volume -l 1 @DEFAULT_AUDIO_SINK@ 5%+ && "
    "pkill -RTMIN+10 dwmblocks", 
    NULL 
};

static const char *downvol[] = { 
    "sh", "-c", 
    "wpctl set-volume -l 1 @DEFAULT_AUDIO_SINK@ 5%- && "
    "pkill -RTMIN+10 dwmblocks", 
    NULL 
};

static const char *mutevol[] = { 
    "sh", "-c", 
    "wpctl set-mute -l 1 @DEFAULT_AUDIO_SINK@ toggle && "
    "pkill -RTMIN+10 dwmblocks", 
    NULL 
};
static const char *medianext[] = { "playerctl", "next", NULL };
static const char *mediaprev[] = { "playerctl", "previous", NULL };
static const char *mediaplay[] = { "playerctl", "play-pause", NULL };


static const Key keys[] = {
	/* modifier                     key        function        argument */
	{ MODKEY,                       XK_p,      spawn,          {.v = dmenucmd } },
	{ MODKEY|ShiftMask,             XK_Return, spawn,          {.v = termcmd } },
	{ MODKEY,                       XK_s,      spawn,          {.v = spotifycmd } },
	{ MODKEY|ShiftMask,             XK_s, spawn,          {.v = steamcmd } },
	{ MODKEY|ShiftMask,             XK_d, spawn,          {.v = discordcmd } },
	{ MODKEY,                       XK_e,      spawn,          {.v = filemanagercmd } },
	{ MODKEY,                       XK_b,      spawn,          {.v = browsercmd } },
	{ MODKEY|ShiftMask,             XK_b,      togglebar,      {0} },
	{ MODKEY,                       XK_j,      focusstack,     {.i = +1 } },
	{ MODKEY,                       XK_k,      focusstack,     {.i = -1 } },
	{ MODKEY,                       XK_i,      incnmaster,     {.i = +1 } },
	{ MODKEY,                       XK_d,      incnmaster,     {.i = -1 } },
	{ MODKEY,                       XK_h,      setmfact,       {.f = -0.05} },
	{ MODKEY,                       XK_l,      setmfact,       {.f = +0.05} },
	{ MODKEY,                       XK_Return, zoom,           {0} },
	{ MODKEY,                       XK_Tab,    view,           {0} },
	{ MODKEY,                       XK_q,      killclient,     {0} },
	{ MODKEY,                       XK_t,      setlayout,      {.v = &layouts[0]} },
	{ MODKEY,                       XK_f,      setlayout,      {.v = &layouts[1]} },
	{ MODKEY,                       XK_m,      setlayout,      {.v = &layouts[2]} },
	{ MODKEY,                       XK_space,  setlayout,      {0} },
	{ MODKEY|ShiftMask,             XK_space,  togglefloating, {0} },
	{ MODKEY,                       XK_0,      view,           {.ui = ~0 } },
	{ MODKEY|ShiftMask,             XK_0,      tag,            {.ui = ~0 } },
	{ MODKEY,                       XK_comma,  focusmon,       {.i = -1 } },
	{ MODKEY,                       XK_period, focusmon,       {.i = +1 } },
	{ MODKEY|ShiftMask,             XK_comma,  tagmon,         {.i = -1 } },
	{ MODKEY|ShiftMask,             XK_period, tagmon,         {.i = +1 } },
	TAGKEYS(                        XK_1,                      0)
	TAGKEYS(                        XK_2,                      1)
	TAGKEYS(                        XK_3,                      2)
	TAGKEYS(                        XK_4,                      3)
	TAGKEYS(                        XK_5,                      4)
	TAGKEYS(                        XK_6,                      5)
	TAGKEYS(                        XK_7,                      6)
	TAGKEYS(                        XK_8,                      7)
	TAGKEYS(                        XK_9,                      8)
	{ MODKEY|ShiftMask,             XK_q,      quit,           {0} },
    { 0, XF86XK_AudioRaiseVolume, spawn, {.v = upvol } },
    { 0, XF86XK_AudioLowerVolume, spawn, {.v = downvol } },
    { 0, XF86XK_AudioMute,        spawn, {.v = mutevol } },
    { 0, XF86XK_AudioNext,  spawn, {.v = medianext } },
    { 0, XF86XK_AudioPrev,  spawn, {.v = mediaprev } },
    { 0, XF86XK_AudioPlay,  spawn, {.v = mediaplay } },
};

/* button definitions */
/* click can be ClkTagBar, ClkLtSymbol, ClkStatusText, ClkWinTitle, ClkClientWin, or ClkRootWin */
static const Button buttons[] = {
	/* click                event mask      button          function        argument */
	{ ClkLtSymbol,          0,              Button1,        setlayout,      {0} },
	{ ClkLtSymbol,          0,              Button3,        setlayout,      {.v = &layouts[2]} },
	{ ClkWinTitle,          0,              Button2,        zoom,           {0} },
	{ ClkStatusText,        0,              Button2,        spawn,          {.v = termcmd } },
	{ ClkClientWin,         MODKEY,         Button1,        movemouse,      {0} },
	{ ClkClientWin,         MODKEY,         Button2,        togglefloating, {0} },
	{ ClkClientWin,         MODKEY,         Button3,        resizemouse,    {0} },
	{ ClkTagBar,            0,              Button1,        view,           {0} },
	{ ClkTagBar,            0,              Button3,        toggleview,     {0} },
	{ ClkTagBar,            MODKEY,         Button1,        tag,            {0} },
	{ ClkTagBar,            MODKEY,         Button3,        toggletag,      {0} },
};

