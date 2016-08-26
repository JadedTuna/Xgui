#include "Xgui.hpp"
#include "Event.hpp"
#include "Window.hpp"
#include "Screen.hpp"
#include "Xgui_defs.hpp"

#include <X11/Xlib.h>
#include <X11/XKBlib.h>
#ifdef _XGUI_USE_XDBE
    #include <X11/extensions/Xdbe.h>
#endif
#include <unordered_map>
#include <assert.h>
#include <iostream>
#include <iomanip>


/* variables */
namespace Xgui
{
    Display *display = NULL;
    Screen *screen = NULL;
    Window *topLevelWindow = NULL;
    std::unordered_map<int, Widget *> widgets;
}

/* functions */
namespace Xgui
{
    bool xdbeEnabled(void)
    {
#ifdef _XGUI_USE_XDBE
        static bool enabled = false;
        static bool tried = false;
        if (!tried) {
            tried = true;
            int major_version_return, minor_version_return;
            if (!XdbeQueryExtension(display, &major_version_return, &minor_version_return)) {
                /* the server does not support the Xdbe extension */
                return false;
            }

            Drawable root = RootWindow(display, DefaultScreen(display));
            int num_screens = 1;

            /* iterate over all visuals supported by Xdbe and see if our visual is supported */
            XdbeScreenVisualInfo *info = XdbeGetVisualInfo(display, &root, &num_screens);
            if (!info) {
                /* an error occured */
                return false;
            }

            for (int i=0; i < info->count; i++) {
                if (info->visinfo[i].visual == DefaultVisual(display, DefaultScreen(display))->visualid) {
                    /* Xdbe supports our screen visual */
                    enabled = true;
                    return true;
                }
            }
        }

        return enabled;
#else /* compiling without Xdbe */
        return false;
#endif /* _XGUI_USE_XDBE */
    }

    bool init(void)
    {
        display = XOpenDisplay(NULL);
        if (!display) {
            return false;
        }

        screen = new Screen(display);
        return true;
    }

    int wait(void)
    {
        /* get next event */
        XEvent event;
        XNextEvent(display, &event);

        Widget *widget = widgets[event.xany.window];

        /* handle event */
        switch (event.type) {
            /*case KeymapNotify:
            {
                / *
                 * Keyboard mapping was changed
                 * /
                //std::cout << "mapping";
                if (1 || event.xmapping.request == MappingKeyboard) {
                    XRefreshKeyboardMapping(&event.xmapping);
                    //std::cout << ".keyboard";
                }
                //std::cout << std::endl;
                break;
            }*/
            case DestroyNotify:
            {
                /*
                 * Some window was destroyed
                 * this is temporary handling code and it is pretty bad
                 * as it only checks if destroyed window was a toplevel one
                 */
                std::cout << "damn" << std::endl;
                if (widget == topLevelWindow) {
                    std::cout << "toplevel" << std::endl;
                    XCloseDisplay(Xgui::display);
                    return 0;
                }
                break;
            }
            case ClientMessage:
            {
                /*
                 * Message received from somewhere else.
                 * For now we only check if it is a WM_DELETE_WINDOW 
                 */
                Atom WM_DELETE_WINDOW = XInternAtom(Xgui::display, "WM_DELETE_WINDOW", False);
                if (static_cast<Atom>(event.xclient.data.l[0]) == WM_DELETE_WINDOW) {
                    XCloseDisplay(Xgui::display);
                    return 0;
                }
                break;
            }
            case Expose:
            {
                /*
                 * A window that covered part of our window has moved away, exposing part (or all) of our window.
                 * Our window was raised above other windows.
                 * Our window mapped for the first time.
                 * Our window was de-iconified.
                 */
                if (event.xexpose.count > 0) {
                    /* there are more expose events to process,
                     * we can safely ignore this one
                     */
                    break;
                }
                PaintEventData data = {0};
                Event ev = {EventType::Paint, {.paintdata=data}};
                widget->event(&ev);
                break;
            }
            case ButtonPress:
            {
                /*
                 * Mouse button pressed.
                 */
                if (event.xbutton.button < 4) {
                    /* send event only of a button was pressed,
                     * buttons 4+ are scroll events, see `case ButtonRelease' */
                    MouseEventData data = {Point(event.xbutton.x, event.xbutton.y),
                        static_cast<Xgui::MouseButton>(event.xbutton.button)};
                    Event ev = {EventType::MousePress, {.mousedata=data}};
                    widget->event(&ev);
                }
                break;
            }
            case ButtonRelease:
            {
                /*
                 * Mouse button released.
                 */
                if (event.xbutton.button < 4) {
                    /* normal button release event */
                    MouseEventData data = {Point(event.xbutton.x, event.xbutton.y),
                        static_cast<Xgui::MouseButton>(event.xbutton.button)};
                    Event ev = {EventType::MouseRelease, {.mousedata=data}};
                    widget->event(&ev);
                } else {
                    /* scroll event */
                    ScrollEventData::Direction direction = ScrollEventData::Up;
                    switch (event.xbutton.button) {
                        case 4: /* up */
                            direction = ScrollEventData::Up;
                            break;
                        case 5: /* down */
                            direction = ScrollEventData::Down;
                            break;
                        case 6: /* left */
                            direction = ScrollEventData::Left;
                            break;
                        case 7: /* right */
                            direction = ScrollEventData::Right;
                            break;
                    }
                    ScrollEventData data = {direction};
                    Event ev = {EventType::Scroll, {.scrolldata=data}};
                    widget->event(&ev);
                }
                break;
            }
            case KeyPress:
            {
                KeySym keysym;
                char string[256] = {0};
                //keysym = XLookupKeysym(&event.xkey, 3);
                XLookupString(&event.xkey, string, 256, &keysym, NULL);
                if (keysym == NoSymbol) {
                    //std::cout << "[press] Key has no symbol: 0x" << std::setbase(16) << keysym << std::endl; 
                } else {
                    char buf[256] = {0};
                    int overflow = 0;
                    XkbTranslateKeySym(Xgui::display, &keysym, 0, buf, 256, &overflow);
                    assert(!overflow); /* can't imagine a single character that will take more than 256 bytes */
                    
                    //std::cout << "[press] 0x" << std::setbase(16) << keysym << " (" << buf << ")" << std::endl;
                    KeyboardEventData data = {keysym, 0};
                    if (event.xkey.state & ShiftMask)       data.mods |= Mod::ModShift;
                    if (event.xkey.state & LockMask)        data.mods |= Mod::ModLock;
                    if (event.xkey.state & ControlMask)     data.mods |= Mod::ModControl;
                    if (event.xkey.state & Mod1Mask)        data.mods |= Mod::ModAlt;
                    Event ev = {EventType::KbdPress, {.keyboarddata=data}};
                    widget->event(&ev);
                }
                break;
            }
            case KeyRelease:
            {
                KeySym keysym;
                char string[256] = {0};
                //keysym = XLookupKeysym(&event.xkey, 3);
                XLookupString(&event.xkey, string, 256, &keysym, NULL);
                if (keysym == NoSymbol) {
                    //std::cout << "[release] Key has no symbol: 0x" << std::setbase(16) << keysym << std::endl; 
                } else {
                    char buf[256] = {0};
                    int overflow = 0;
                    XkbTranslateKeySym(Xgui::display, &keysym, 0, buf, 256, &overflow);
                    assert(!overflow); /* can't imagine a single character that will take more than 256 bytes */
                    
                    //std::cout << "[release] 0x" << std::setbase(16) << keysym << " (" << buf << ")" << std::endl;
                    KeyboardEventData data = {keysym, 0};
                    if (event.xkey.state & ShiftMask)       data.mods |= Mod::ModShift;
                    if (event.xkey.state & LockMask)        data.mods |= Mod::ModLock;
                    if (event.xkey.state & ControlMask)     data.mods |= Mod::ModControl;
                    if (event.xkey.state & Mod1Mask)        data.mods |= Mod::ModAlt;
                    Event ev = {EventType::KbdRelease, {.keyboarddata=data}};
                    widget->event(&ev);
                }
                break;
            }
        }

        return -1;
    }

    int run(void)
    {
        for (;;) {
            int code = wait();
            if (code != -1) {
                /* quit the application */
                return code;
            }
        }
    }

}