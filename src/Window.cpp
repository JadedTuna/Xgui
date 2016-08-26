#include "Widget.hpp"
#include "Window.hpp"


namespace Xgui
{
    Window::Window(Widget *parent, const String &title, int x, int y, size_t w, size_t h, Flags flags)
        : Widget(parent, x, y, w, h)
    {
        _init(flags, title);
    }

    Window::Window(Widget *parent, const String &title, const Point &pos, const Size &size, Flags flags)
        : Widget(parent, pos, size)
    {
        _init(flags, title);
    }

    Window::Window(Widget *parent, const String &title, const Rect &rect, Flags flags)
        : Widget(parent, rect)
    {
        _init(flags, title);
    }

    void Window::_init(Flags, const String &title)
    {
        if (!Xgui::topLevelWindow) {
            Xgui::setTopLevelWindow(this);
        }
        XStoreName(Xgui::display, _X11Window, title.c_str());
    }
}