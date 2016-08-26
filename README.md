# Xgui
Xgui is a GUI library for Linux based on Xlib and several extensions.

# Requirements
The goal of the library is to use only `Xlib` and X extensions for as long as possible, aka being able to compile it on pretty much any Linux installation with a more or less modern X server. Current requirements are:
* `Xlib` - base library
* `Xft` - FreeType support
* `Xrender` - antialiased rendering
* `Xext` - additional extensions (e.g. `Xdbe` for double-buffering support)

# Progress
As for now, this library can't be used for real-world application because of lack of widgets - I've been mainly working on the TextView widget, which is required for another project I am working on, based on this library - a text editor for writers. In the future I am planning on adding quite a couple more widgets, like `Label`, `ImageView`, etc.

# Compatibility
Compatibility with later/earlier versions is not guaranteed as library is still in heavy development. Once I have enough base functionality and widgets I will of course try to maintain API compatibility, but probably not ABI compatibility.

# Setting up
Library requires a little setting up before usage, namely, executing `genkeycodes-x` which is a Python script to generate `KeyCodes.hpp` from `<X11/keysymdef.h>`. `KeyCodes.hpp` is a temporary file used to profile keycode access to applications. In the future I am going to replace it with a smaller and easier to use version.

# How-to
To understand how the library works it is advised to go thru the source code and check out test programs in the `tests/` folder. General idea is:
* You call `Xgui::init()` which initializes the library
* You create widgets, bind callbacks, etc
* You either call `Xgui::run()` and obtain the exit code or you keep calling `Xgui::wait()` in a loop

`Xgui::run()` will enter a mainloop that listens for events. If you don't want the program to enter a loop you can instead call `Xgui::wait()` which will process only the next event and then return a code, with `-1` meaning success and anything else being the return code for the application (means all the widgets were destroyed).

# Helping and ideas
Any ideas of how this library can be improved are very welcome! Also any info about optimizing any of the widgets (especially `TextView`) would be really great.
