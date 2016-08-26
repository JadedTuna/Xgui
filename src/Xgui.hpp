#ifndef XGUI_XGUI_HPP
#define XGUI_XGUI_HPP
#include "Widget.hpp"
#include "Window.hpp"
#include "Screen.hpp"
#include "KeyCodes.hpp"
#include "Xgui_defs.hpp"

#include <X11/Xlib.h>
#include <unordered_map>


namespace Xgui
{
    extern Display *display;
    extern Screen *screen;
    extern std::unordered_map<int, Widget *> widgets;

    inline void addWidget(Widget *widget) { widgets[widget->X11Window()] = widget; }
    bool xdbeEnabled(void);     /* check if X server supports the Xdbe extension */
    bool init(void);            /* init Xgui */
    int wait(void);             /* wait for the next event */
    int run(void);              /* enter event loop */
}

#endif