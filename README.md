dwm - dynamic window manager
============================
This is my personal dwm build with the following patches applied:
- **[cool-autostart](https://dwm.suckless.org/patches/cool_autostart/)** - Auto-start programs on dwm launch
- **[systray](https://dwm.suckless.org/patches/systray/)** - System tray support
- **[vanitygaps](https://dwm.suckless.org/patches/vanitygaps/)** - Configurable gaps between windows

Original dwm by suckless: https://dwm.suckless.org/

Requirements
------------
In order to build dwm you need the Xlib header files.

Installation
------------
Edit config.mk to match your local setup (dwm is installed into
the /usr/local namespace by default).

Afterwards enter the following command to build and install dwm (if
necessary as root):

    make clean install

Running dwm
-----------
Add the following line to your .xinitrc to start dwm using startx:

    exec dwm

In order to connect dwm to a specific display, make sure that
the DISPLAY environment variable is set correctly, e.g.:

    DISPLAY=foo.bar:1 exec dwm

(This will start dwm on display :1 of the host foo.bar.)

In order to display status info in the bar, you can do something
like this in your .xinitrc:

    while xsetroot -name "`date` `uptime | sed 's/.*,//'`"
    do
    	sleep 1
    done &
    exec dwm

Configuration
-------------
The configuration of dwm is done by creating a custom config.h
and (re)compiling the source code.
